#include <Arduino.h>
#include <unity.h>

#include "../../utils.h"
#include "imu.cpp"

void test_imu_connection() {}

void setup() {
	if (PRINTDEBUG)
		SERIALDEBUG.begin(115200);
	UNITY_BEGIN();
	RUN_TEST(test_imu_connection);
	UNITY_END();
}

void loop() {}