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

	if (millis() - timer > 1000) {
		float ax = mpu.getAccX(), ay = mpu.getAccY(), az = mpu.getAccZ();
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
		timer = millis();
	}
}