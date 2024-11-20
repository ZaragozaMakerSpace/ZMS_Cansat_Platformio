#include "../../src/utils.h"
#include "../config.h"
#include <Arduino.h>
#include <unity.h>

#define PRINTDEBUG true
#define SERIALDEBUG Serial

#include <RHDatagram.h>
#include <RH_RF69.h>
#include <SPI.h>

uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
RH_RF69 rf69(RFM69_CS, RFM69_INT);

RHDatagram rf69_manager(rf69, MY_ADDRESS);

bool setupRadio() {
	bool radioOk = false;
	pinMode(RFM69_RST, OUTPUT);
	digitalWrite(RFM69_RST, LOW);

	// Manual Reset Radio
	digitalWrite(RFM69_RST, HIGH);
	delay(10);
	digitalWrite(RFM69_RST, LOW);
	if (rf69_manager.init()) {
		radioOk = true;
	}
	if (!rf69.setFrequency(RF69_FREQ)) {
		DUMPSLN("setFrequency failed \n CHECK RADIO TX/RX");
	}

	rf69.setTxPower(20); // 14-20 range must be true for 69HCW

	uint8_t key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
					 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	rf69.setEncryptionKey(key);

	pinMode(LED, OUTPUT);
	return radioOk;
}

void test_radio_connection() {
	bool radioConnected = setupRadio();
	TEST_ASSERT_EQUAL(radioConnected, true);
}

void setup() {
	Serial.begin(115200);
	Serial.println("TEST RADIO");
	UNITY_BEGIN();
	RUN_TEST(test_radio_connection);

	UNITY_END();
}

void loop() {}