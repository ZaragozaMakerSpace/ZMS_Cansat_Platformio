#include <Arduino.h>

#include "../config.h"

#include "../BMP280.h"
#include "../eeprom.h"

#define LIMITE_RSSI -70

bool radioOK = false;
bool falloRadio = false;
bool barometroOK = false;
bool imuOK = false;
bool cansatLanzado = false;
float acc_umbral = 25;
unsigned long launchTimer = 2000;
unsigned long launchTimerController = 0;

#include "../radio.h"
#include <Ticker.h> //Sistema no bloqueante

#include "../imu.h"
Ticker sendRadioTicker(radioSendInfo, 1000, 0, MILLIS);
Ticker getRadioTicker(getRadioInfo, 1000, 0, MILLIS);

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
	setupDatos();			 // Ponemos los datos a valores iniciales.
	sendRadioTicker.start(); // Inicializamos el evento no bloqueante
	// eepromGet();
	getRadioTicker.start();
}

void loop() {
	calcularIMU();
	// sendRadioTicker.update();
	getRadioTicker.update();
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
}