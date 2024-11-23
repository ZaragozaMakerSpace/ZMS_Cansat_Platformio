#include <Arduino.h>

#include "../config.h"

#ifdef EEPROM_SAVEDATA
#include "../eeprom.h"
#include "datamodel.h"
#endif

#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps612.h"

#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

#define OUTPUT_READABLE_YAWPITCHROLL
#define INTERRUPT_PIN 2
MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;	// set true if DMP init was successful
uint8_t mpuIntStatus;	// holds actual interrupt status byte from MPU
uint8_t devStatus;		// return status (0 = success, !0 = error)
uint16_t packetSize;	// expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;		// count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;		 // [w, x, y, z] quaternion container
VectorInt16 aa;		 // [x, y, z] accel sensor measurements
VectorInt16 aaReal;	 // [x, y, z] gravity-free accel sensor measurements
VectorInt16 aaWorld; // [x, y, z] world-frame accel sensor measurements
VectorFloat gravity; // [x, y, z] gravity vector
float euler[3];		 // [psi, theta, phi]    Euler angle container
float ypr[3];		 // [yaw, pitch, roll]

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false; // indicates MPU interrupt pin has gone high
void dmpDataReady() { mpuInterrupt = true; }

void setup() {
	if (PRINTDEBUG)
		SERIALDEBUG.begin(SERIALBAUDS);
	DUMPSLN("Cansat MPU6050");

	mySoftwareSerial.begin(9600);

	Wire.begin();
	pinMode(INTERRUPT_PIN, INPUT);

	// verify connection
	mpu.initialize();
	DUMPLN("Status ", mpu.testConnection());

	// load and configure the DMP
	DUMPSLN("Initializing DMP...");
	devStatus = mpu.dmpInitialize();

	// supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXGyroOffset(51);
	mpu.setYGyroOffset(8);
	mpu.setZGyroOffset(21);
	mpu.setXAccelOffset(1150);
	mpu.setYAccelOffset(-50);
	mpu.setZAccelOffset(1060);
	if (devStatus == 0) {
		// Calibration Time: generate offsets and calibrate our MPU6050
		mpu.CalibrateAccel(6);
		mpu.CalibrateGyro(6);
		mpu.PrintActiveOffsets();
		// turn on the DMP, now that it's ready
		DUMPSLN("Enabling DMP...");
		mpu.setDMPEnabled(true);

		// enable Arduino interrupt detection
		DUMP("Enabling interrupt detection (Arduino external interrupt ",
			 digitalPinToInterrupt(INTERRUPT_PIN));
		DUMPSLN(")...");
		attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady,
						RISING);
		mpuIntStatus = mpu.getIntStatus();

		// set our DMP Ready flag so the main loop() function knows it's okay to
		// use it
		Serial.println(F("DMP ready! Waiting for first interrupt..."));
		dmpReady = true;

		// get expected DMP packet size for later comparison
		packetSize = mpu.dmpGetFIFOPacketSize();
	} else {
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		DUMP("DMP Initialization failed : ", devStatus)
	}

	if (!myDFPlayer.begin(mySoftwareSerial)) {
		Serial.println("Unable to begin:");
		Serial.println("1.Please recheck the connection!");
		Serial.println("2.Please insert the SD card!");
		while (true)
			;
	}
	Serial.println("DFPlayer Mini online.");

	myDFPlayer.volume(10); // Set volume value (0~30).
	myDFPlayer.play(1);	   // Play the first MP3 file.
}

void loop() {

	if (!dmpReady)
		return;
	if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
		// From Quaternion to YPR
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		DUMP("ypr\t", ypr[0] * 180 / M_PI);
		DUMP("\t", ypr[1] * 180 / M_PI);
		DUMP("\t", ypr[2] * 180 / M_PI);
	}
}