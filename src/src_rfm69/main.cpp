#include <Arduino.h>

#include "../config.h"

#include <RH_RF69.h>
#include <SPI.h>

#define RF69_FREQ 868.0

#define RFM69_CS 10 // Pin CS (NSS)
#define RFM69_INT 2 // Pin de interrupción (DIO0)
#define RFM69_RST 9 // Pin de reset (opcional)

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

	pinMode(RFM69_RST, OUTPUT);
	digitalWrite(RFM69_RST, LOW);

	digitalWrite(RFM69_RST, HIGH);
	delay(100);
	digitalWrite(RFM69_RST, LOW);
	delay(100);

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
	Serial.println("Sending...");
	const char *message = "Hola desde el transmisor";
	rf69.send((uint8_t *)message, strlen(message));
	if (waitForPacketSent(1000)) {
		Serial.println("Mensaje enviado");
	} else {
		Serial.println("Error : pinDIO not connected");
		// Puedes intentar reiniciar el módulo aquí si es necesario
	}
}
