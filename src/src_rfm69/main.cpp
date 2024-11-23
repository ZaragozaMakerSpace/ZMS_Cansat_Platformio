#include <Arduino.h>

#include "../config.h"

#include <RH_RF69.h>
#include <SPI.h>

RH_RF69 rf69(RFM69_CS, RFM69_INT);

#include "../radio.h"

void setup(void) {
	if (PRINTDEBUG)
		SERIALDEBUG.begin(SERIALBAUDS);
	DUMPSLN("Cansat ADAFRUIT");

	setupRadio();
}

void loop() {
	Serial.println("Sending...");
	sendMsg();
}
