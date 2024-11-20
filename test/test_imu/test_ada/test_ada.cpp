#include <Arduino.h>
#include <unity.h>

#include "../../utils.h"
#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

unsigned long timer = 0;

void test_imu_connection() {
	byte status = mpu.begin();
	if (status == 0) {
		DUMPSLN("MPU6050 connection successful");
	} else {
		DUMPSLN("MPU6050 connection failed");
	}

	TEST_ASSERT_TRUE(status == 0);
}

void test_imu_data() {
	mpu.update();
	float ax = mpu.getAccX(), ay = mpu.getAccY(), az = mpu.getAccZ();
	TEST_ASSERT_FLOAT_WITHIN(2.0, 0.0, ax);
	TEST_ASSERT_FLOAT_WITHIN(2.0, 0.0, ay);
	TEST_ASSERT_FLOAT_WITHIN(2.0, 0.0, az - 9.81);
}

void setup() {
	if (PRINTDEBUG)
		SERIALDEBUG.begin(SERIALBAUDS);
	Wire.begin();

	UNITY_BEGIN();
	RUN_TEST(test_imu_connection);

	DUMPSLN("Calculating offsets, do not move MPU6050");

	mpu.calcOffsets(true, true); // gyro and accelero
	DUMPSLN("Done!\n");
	UNITY_END();
}

void loop() {
	mpu.update();

	if (millis() - timer > 1000) { // print data every second
		Serial.print(F("TEMPERATURE: "));
		Serial.println(mpu.getTemp());
		Serial.print(F("ACCELERO  X: "));
		Serial.print(mpu.getAccX());
		Serial.print("\tY: ");
		Serial.print(mpu.getAccY());
		Serial.print("\tZ: ");
		Serial.println(mpu.getAccZ());

		Serial.print(F("GYRO      X: "));
		Serial.print(mpu.getGyroX());
		Serial.print("\tY: ");
		Serial.print(mpu.getGyroY());
		Serial.print("\tZ: ");
		Serial.println(mpu.getGyroZ());

		Serial.print(F("ACC ANGLE X: "));
		Serial.print(mpu.getAccAngleX());
		Serial.print("\tY: ");
		Serial.println(mpu.getAccAngleY());

		Serial.print(F("ANGLE     X: "));
		Serial.print(mpu.getAngleX());
		Serial.print("\tY: ");
		Serial.print(mpu.getAngleY());
		Serial.print("\tZ: ");
		Serial.println(mpu.getAngleZ());
		Serial.println(
			F("=====================================================\n"));
		timer = millis();
	}
}