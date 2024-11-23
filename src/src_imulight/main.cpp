#include <Arduino.h>

#include "../config.h"

#ifdef EEPROM_SAVEDATA
#include "../eeprom.h"
#include "datamodel.h"
#endif

#include "Wire.h"
#include <MPU6050_light.h>
#include <RH_RF69.h>
#include <SPI.h>

RH_RF69 rf69(RFM69_CS, RFM69_INT);

// #include "imu.cpp"
#include "../radio.h"

MPU6050 mpu(Wire);
unsigned long timer = 0;
unsigned long readingInterval = 100;

#define NUM_READINGS 30
float readings[NUM_READINGS]; // almacena las lecturas para calcular el promedio
int readIndex = 0;			  // el índice de la lectura actual
float total = 0;			  // la suma total de las lecturas
float average = 0;			  // el promedio
float acc_zero_threshold = 0.2;

void calculateIMU(float ax, float ay, float az) {
	total = total - readings[readIndex];
	readings[readIndex] = sqrt(ax * ax + ay * ay + az * az) -
						  9.81; // aceleración total menos la gravedad
	total = total + readings[readIndex];
	readIndex = (readIndex + 1) % NUM_READINGS;
	average = total / NUM_READINGS;

	DUMPLN("Average acceleration: ", average);

	// se aproxima a cero (ajustar umbral) en acc_zero_threshold
	if (abs(average) < acc_zero_threshold) {
		DUMPSLN("Apogee detected!");
		// sendMsg();
	}
}

void setup() {
	if (PRINTDEBUG)
		SERIALDEBUG.begin(SERIALBAUDS);
	DUMPSLN("Cansat IMULIGHT");
	Wire.begin();
	// setupRadio();
	byte status = mpu.begin();
	if (status == 0) {
		DUMPSLN("MPU6050 connection successful");
	} else {
		DUMPSLN("MPU6050 connection failed");
	}

	DUMPSLN("Calculating offsets, do not move MPU6050");

	/*
	 * case 0: // range = +- 2 g
	 * case 1: // range = +- 4 g
	 * case 2: // range = +- 8 g
	 * case 3: // range = +- 16 g
	 */
	mpu.setAccConfig(3);
	mpu.calcOffsets(true, true);

	// Offset to calculate before launch
	mpu.setAccOffsets(0, 0, -8.83);
	DUMPSLN("Done!\n");
}

void loop() {

	mpu.update();
	if (millis() - timer > readingInterval) {
		float ax = mpu.getAccX(), ay = mpu.getAccY(), az = mpu.getAccZ();
		if (DEBUG) {
			DUMP("TEMPERATURE: ", mpu.getTemp());
			DUMP("ACC X: ", ax);
			DUMP("\tY: ", ay);
			DUMPLN("\tZ: ", az);

			DUMP("GYRO      X: ", mpu.getGyroX());
			DUMP("\tY: ", mpu.getGyroY());
			DUMPLN("\tZ: ", mpu.getGyroZ());

			DUMP("ACC ANGLE X: ", mpu.getAccAngleX());
			DUMPLN("\tY: ", mpu.getAccAngleY());

			DUMP("ANGLE     X: ", mpu.getAngleX());
			DUMP("\tY: ", mpu.getAngleY());
			DUMP("\tZ: ", mpu.getAngleZ());
			DUMPSLN("=====================================================");
		}
		timer = millis();
		calculateIMU(ax, ay, az);
	}
}
