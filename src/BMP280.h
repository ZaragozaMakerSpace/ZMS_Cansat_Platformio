#include <Adafruit_BMP280.h> //Sensor de temperatura
#include <Arduino.h>		 //incluimos las librerias de arduino
#include <Wire.h>			 //Librería del protocolo I2C

// Declaracion de variables
Adafruit_BMP280 bmp; // I2C Obejo e Instancia (definición)

bool setupBMP() {
	bool status = bmp.begin(0x76);

	if (status) {
		bmp.setSampling(
			Adafruit_BMP280::MODE_NORMAL,  /* Operating Mode. */
			Adafruit_BMP280::SAMPLING_X2,  /* Temp. oversampling */
			Adafruit_BMP280::SAMPLING_X16, /* Pressure oversampling */
			Adafruit_BMP280::FILTER_X16,   /* Filtering. */
			Adafruit_BMP280::STANDBY_MS_500);
	}
	return status;

} // Fin de la función de inicialización del barómetro

void radioInfo() {
	paquete.idPaquete++; // incrementamos el valor de paquete a enviar
	paquete.temperatura = bmp.readTemperature();
	paquete.altitud = bmp.readAltitude(1013.25);

	String strPaquete =
		datosRadio();	  // Crea el paquete completo con la información;
	char radiopacket[64]; // Tenemos una cadena de texto

	sprintf(radiopacket, "%s",
			strPaquete.c_str()); // Metemos en la cadena de texto la parte que
								 // nos interesa del mensaje(tipo String)
	DUMP("enviando ", radiopacket);

	// Enviar el mensaje al DESTINO (DEST_ADDRESS)

	rf69_manager.sendto((uint8_t *)radiopacket, strlen(radiopacket),
						DEST_ADDRESS);
	setupDatos();
}