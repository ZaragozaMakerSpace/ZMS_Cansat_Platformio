#include <Arduino.h>

#include "../config.h"

#ifdef EEPROM_SAVEDATA
#include "../eeprom.h"
#include "datamodel.h"
#endif

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

#include "../imu_utils.cpp"

void setup(void) {
	if (PRINTDEBUG)
		SERIALDEBUG.begin(SERIALBAUDS);
	DUMPSLN("Cansat ADAFRUIT");
	while (!Serial)
		delay(10);

	if (!mpu.begin()) {
		DUMPSLN("Failed to find MPU6050 chip");
		while (1) {
			delay(10);
		}
	}

	setupIMU();
	imuConfigs();
}

void loop() {
#ifdef IMU_INTERRUPTION
	if (mpu.getMotionInterruptStatus()) {
#endif
		/* Get new sensor events with the readings */
		sensors_event_t a, g, temp;
		mpu.getEvent(&a, &g, &temp);

		/* Print out the values */
		DUMP("Acceleration X: ", a.acceleration.x);
		DUMP(", Y: ", a.acceleration.y);
		DUMP(", Z: ", a.acceleration.z);
		DUMPSLN(" m/s^2");

		DUMP("Rotation X: ", g.gyro.x);
		DUMP(", Y: ", g.gyro.y);
		DUMP(", Z: ", g.gyro.z);
		DUMPSLN(" rad/s");

		DUMP("Temperature: ", temp.temperature);
		DUMPSLN(" degC");
#ifdef IMU_INTERRUPTION
	}
#endif
	delay(10);
}
