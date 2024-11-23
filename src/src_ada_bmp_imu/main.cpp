#include <Arduino.h>

#include "../config.h"

#ifdef EEPROM_SAVEDATA
#include "../eeprom.h"
#include "datamodel.h"
#endif

#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;

#include "../bmp_utils.cpp"
#include "../imu_utils.cpp"

void setup() {
	if (PRINTDEBUG)
		SERIALDEBUG.begin(SERIALBAUDS);
	DUMPSLN("Cansat ADA BMP280 + IMU");

	bool status = bmp.begin(0x76);
	if (status) {
		bmp.setSampling(
			Adafruit_BMP280::MODE_NORMAL,  /* Operating Mode. */
			Adafruit_BMP280::SAMPLING_X2,  /* Temp. oversampling */
			Adafruit_BMP280::SAMPLING_X16, /* Pressure oversampling */
			Adafruit_BMP280::FILTER_X16,   /* Filtering. */
			Adafruit_BMP280::STANDBY_MS_500);
	} else {
		DUMPSLN("No se pudo encontrar el sensor BMP280!");
	}

	// IMU Initialization
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
	readBMP();
	readIMU();
}