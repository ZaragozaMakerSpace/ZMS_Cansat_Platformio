#include <Arduino.h>

#include "../config.h"

#include <RH_RF69.h>
#include <SPI.h>

RH_RF69 rf69(RFM69_CS, RFM69_INT);

bool waitForPacketSent(uint16_t timeout) {
	unsigned long startTime = millis();
	Serial.println(digitalRead(RFM69_INT));
	while (rf69.waitPacketSent(0)) {
		if (millis() - startTime > timeout) {
			Serial.println("Tiempo de espera agotado al enviar el paquete");
			return false;
		}
	}
	return true;
}

void setup(void) {
	if (PRINTDEBUG)
		SERIALDEBUG.begin(SERIALBAUDS);
	DUMPSLN("Cansat ADAFRUIT");

	if (!rf69.init()) {
		Serial.println("RF69 Init Failed");
	}

	if (!rf69.setFrequency(RF69_FREQ)) {
		Serial.println("FREQUENCY ERROR");
	}

	// Configura la potencia de transmisión (14 dBm)
	rf69.setTxPower(14, true);

	Serial.println("Transmisor RF69 listo");
}

void loop() {
	if (rf69.available()) {
		// Buffer para almacenar el mensaje entrante
		uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
		uint8_t len = sizeof(buf);

		if (rf69.recv(buf, &len)) {
			// Mensaje recibido con éxito
			Serial.print("Received message: ");
			Serial.println((char *)buf);
		} else {
			// Error en la recepción
			Serial.println("Receive failed");
		}
	}
}
