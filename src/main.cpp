#include <Arduino.h>

#include "config.h"
#include "radio.h"

#include "BMP280.h"
#include "eeprom.h"

#include <Ticker.h> //Sistema no bloqueante

#define LIMITE_RSSI -70

uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

bool radioOK = false;
bool falloRadio = false;
bool barometroOK = false;
bool imuOK = false;
bool cansatLanzado = false;
float acc_umbral = 25;
unsigned long launchTimer = 2000;
unsigned long launchTimerController = 0;

#include "imu.h"
Ticker radioTicker(radioInfo, 1000, 0, MILLIS);

#include "satController.h"
Ticker launchedTicker(satLaunched, 100, 0, MILLIS);

void setup() {

	// inicializacion puerto serie// Dont put this on the stack:
	Serial.begin(9600);

	DUMPSLN("Cansat ZMS");
	DUMPSLN(__DATE__);
	DUMPSLN(__FILE__);
	//  Pin Initialization
	pinMode(LED, OUTPUT);
	radioOK = setupRadio();
	barometroOK = setupBMP();
	imuOK = setupIMU();
	setupDatos(); // Ponemos los datos a valores iniciales.
	// radioTicker.start(); // Inicializamos el evento no bloqueante
	// eepromGet();
}

void loop() {
	calcularIMU();
	radioTicker.update();
	if (!cansatLanzado && launchedTicker.state() == STOPPED &&
		acLauncher > acc_umbral) {
		launchTimerController = millis();
		launchedTicker.start();
		DUMPSLN("TICKER LAUNCH ACTIVADO");
	}
	if (!cansatLanzado && launchedTicker.state() == RUNNING &&
		acLauncher < acc_umbral) {
		launchedTicker.stop();
		DUMPSLN("RESET DE IMU TICKER");
	}
	if (!cansatLanzado && launchedTicker.state() == RUNNING)
		launchedTicker.update();

	if (rf69_manager.available()) {
		uint8_t len = sizeof(buf);
		uint8_t from;
		if (paquete.idPaquete % 4 == 0) {
			eepromSave();
		}
		if (paquete.idPaquete % 3 == 0) {
			acLauncher = 0;
		}
		// Comprobamos si recibimos señal de la estación de tierra.
		if (rf69_manager.recvfrom(buf, &len, &from)) {
			buf[len] = 0;
			DUMP("Got packet from #", from);
			DUMP(" [RSSI :", rf69.lastRssi());
			if (rf69.lastRssi() < -40 && !falloRadio) {
				falloRadio = true;
				salvar = true;
				eeAddress = 0;
				paquete.idPaquete = 0;
			}
			String datos = String((char *)buf);
			DUMPV(datos);
			if (datos == "salvar") {
				// EEPROM RESET
				eeAddress = 0;
				paquete.idPaquete = 0;
				salvar = true;
			}
		}
	}
}