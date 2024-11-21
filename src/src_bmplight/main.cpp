#include <Arduino.h>

#include "../config.h"
#include <BMP280_DEV.h>
#include <Wire.h>

float temperature, pressure, altitude;
BMP280_DEV bmp280; // Instantiate BMP280_DEV object

void setup() {
	if (PRINTDEBUG)
		SERIALDEBUG.begin(SERIALBAUDS);
	DUMPSLN("Cansat BMP280 Light");
	bmp280.begin(); // Default initialisation, place the BMP280 into SLEEP_MODE
	// bmp280.setPresOversampling(OVERSAMPLING_X4);    // Set the pressure
	// oversampling to X4 bmp280.setTempOversampling(OVERSAMPLING_X1);    // Set
	// the temperature oversampling to X1 bmp280.setIIRFilter(IIR_FILTER_4); //
	// Set the IIR filter to setting 4
}

void loop() {
	bmp280.startForcedConversion(); // Start BMP280 forced conversion (if we're
									// in SLEEP_MODE)
	if (bmp280.getMeasurements(temperature, pressure, altitude)) {
		Serial.print(temperature);
		Serial.print(F("*C   "));
		Serial.print(pressure);
		Serial.print(F("hPa   "));
		Serial.print(altitude);
		Serial.println(F("m"));
	}
}