#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 imu;

bool setupIMU() { return imu.begin(); }

void calcularIMU() {
	sensors_event_t a, g, temp;
	imu.getEvent(&a, &g, &temp);

	float ax = a.acceleration.x;
	float ay = a.acceleration.y;
	float az = a.acceleration.z;
	float aceleracion = sqrt(pow(ax, 2) + pow(ay, 2) + pow(az, 2));
	aceleracion < paquete.acMinima ? paquete.acMinima = aceleracion : 1;
	aceleracion > paquete.acMaxima ? paquete.acMaxima = aceleracion : 1;
	aceleracion > acLauncher ? acLauncher = aceleracion : 1;
	aceleracion > acMaxima4 ? acMaxima4 = aceleracion : 1;
	aceleracion < acMinima4 ? acMinima4 = aceleracion : 1;
}