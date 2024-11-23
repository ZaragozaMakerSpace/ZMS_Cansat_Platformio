#ifndef _CANSAT_CONFIG_H
#define _CANSAT_CONFIG_H
/*******************************************************/
/************ Configuración de la Radio  ***************/
/*******************************************************/

// Hay que indicar la frecuenca de nuestra radio
#define RF69_FREQ 868.0 // esto es un comentario 868//915.0

// Cada radio tiene un identificador
#define DEST_ADDRESS 1 // Esta es el identificador de la estación base.

// Cada emisor deberá tener una dirección única
#define MY_ADDRESS 2

// Pines donde están conectadas nuestras placas, definidos por el fabricante

#define RFM69_INT 2 // Pin de interrupción (DIO0)
#define RFM69_CS 10 // Pin CS (NSS)
#define RFM69_RST 9 //
#define LED 13

/*******************************************************/
/**************** Configuración EEPROM  *******************/
/*******************************************************/

#define EEPROM_SAVEDATA

/*******************************************************/
/**************** Configuración IMU  *******************/
/*******************************************************/

// #define IMU_INTERRUPTION
#define IMU_INTERRUPT_PIN 2 // Avoid Pin 2 connected to Radio

/*******************************************************/
/************ Configuración LED Pixels  ****************/
/*******************************************************/

#define PIXEL_PIN 9
#define NUMPIXELS 7
#define PIXEL_INTERVAL 250
#define PIXEL_BRIGHTNESS 50
unsigned long previousMillis = 0;
int currentLED = 0;

// ------------------ SERIAL CONFIG --------------------//
#define DEBUG false
#define PRINTDEBUG true
#define SERIALDEBUG Serial
#define SERIALBAUDS 115200

#define DUMPS(s)                                                               \
	{ SERIALDEBUG.print(F(s)); }
#define DUMPSLN(s)                                                             \
	{ SERIALDEBUG.println(F(s)); }
#define DUMPPRINTLN()                                                          \
	{ SERIALDEBUG.println(); }
#define DUMPV(v)                                                               \
	{ SERIALDEBUG.print(v); }
#define DUMPLN(s, v)                                                           \
	{                                                                          \
		DUMPS(s);                                                              \
		DUMPV(v);                                                              \
		DUMPPRINTLN();                                                         \
	}
#define DUMP(s, v)                                                             \
	{                                                                          \
		DUMPS(s);                                                              \
		DUMPV(v);                                                              \
	}

#endif