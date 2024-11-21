#ifdef _ADAFRUIT_MPU6050_H

void imuConfigs() {
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	DUMPSLN("Accelerometer range set to: ");
	switch (mpu.getAccelerometerRange()) {
	case MPU6050_RANGE_2_G:
		DUMPSLN("+-2G");
		break;
	case MPU6050_RANGE_4_G:
		DUMPSLN("+-4G");
		break;
	case MPU6050_RANGE_8_G:
		DUMPSLN("+-8G");
		break;
	case MPU6050_RANGE_16_G:
		DUMPSLN("+-16G");
		break;
	}
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);

	DUMPSLN("Gyro range set to: ");
	switch (mpu.getGyroRange()) {
	case MPU6050_RANGE_250_DEG:
		DUMPSLN("+- 250 deg/s");
		break;
	case MPU6050_RANGE_500_DEG:
		DUMPSLN("+- 500 deg/s");
		break;
	case MPU6050_RANGE_1000_DEG:
		Serial.println("+- 1000 deg/s");
		break;
	case MPU6050_RANGE_2000_DEG:
		DUMPSLN("+- 2000 deg/s");
		break;
	}

	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
	DUMPSLN("Filter bandwidth set to: ");
	switch (mpu.getFilterBandwidth()) {
	case MPU6050_BAND_260_HZ:
		DUMPSLN("260 Hz");
		break;
	case MPU6050_BAND_184_HZ:
		DUMPSLN("184 Hz");
		break;
	case MPU6050_BAND_94_HZ:
		DUMPSLN("94 Hz");
		break;
	case MPU6050_BAND_44_HZ:
		DUMPSLN("44 Hz");
		break;
	case MPU6050_BAND_21_HZ:
		Serial.println("21 Hz");
		break;
	case MPU6050_BAND_10_HZ:
		Serial.println("10 Hz");
		break;
	case MPU6050_BAND_5_HZ:
		Serial.println("5 Hz");
		break;
	}
}

void setupIMU() {
	mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
	mpu.setMotionDetectionThreshold(1);
	mpu.setMotionDetectionDuration(20);

#ifdef IMU_INTERRUPTION
	// Keep it latched.  Will turn off when reinitialized.
	mpu.setInterruptPinLatch(true);
	mpu.setInterruptPinPolarity(true);
	mpu.setMotionInterrupt(true);
#endif
}

void readIMU() {
	if (mpu.getMotionInterruptStatus()) {
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
	}
}

#endif