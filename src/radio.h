#include <RHDatagram.h>
#include <RH_RF69.h>
#include <SPI.h>

/*//Tamaño del mensaje
  #define RH_RF69_MAX_ENCRYPTABLE_PAYLOAD_LEN 64 //tamaño del mensaje
  codificable #define RH_RF69_HEADER_LEN 4 //tamaño de la cabecera #define
  RH_RF69_MAX_MESSAGE_LEN (RH_RF69_MAX_ENCRYPTABLE_PAYLOAD_LEN -
  RH_RF69_HEADER_LEN)
*/

uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

// Creamos un "objeto" radio con el trabajaremos.
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Control de los mensajes, entrantes y salientes
// Solo acepta los de su dirección.
RHDatagram rf69_manager(rf69, MY_ADDRESS);

int16_t packetnum = 0; // Contador de mensajes recibidos

// declaracion variables

bool setupRadio() {
	bool radioOk = false;

	pinMode(RFM69_RST, OUTPUT);
	digitalWrite(RFM69_RST, LOW);

	DUMPSLN("Transmisón desde el satélite");

	// manual reset para activar la radio.
	digitalWrite(RFM69_RST, HIGH);
	delay(10);
	digitalWrite(RFM69_RST, LOW);
	delay(10);

	if (rf69_manager.init()) {
		radioOk = true;
	}

	if (!rf69.setFrequency(RF69_FREQ)) {
		DUMPSLN("setFrequency failed \n CHECK RADIO TX/RX");
	}

	rf69.setTxPower(20); // rango de 14-20 arg must be true for 69HCW

	// Tenemos una clave de cifrado para los mensajes, debe ser la misma en el
	// emisor y el receptor
	uint8_t key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
					 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	rf69.setEncryptionKey(key);

	pinMode(LED, OUTPUT);

	DUMP("RFM69 radio @", (int)RF69_FREQ);
	DUMPSLN(" MHz");
	return radioOk;
}

String datosRadio() {
	String s = "";
	// Construimos un mensaje de texto sumando los valores interpretados como un
	// objeto String (NOTA: String NO ES IGUAL cadena de texto)
	s = s + paquete.idPaquete + ";" + paquete.temperatura + ";" +
		paquete.altitud + ";" + paquete.acMaxima + ";" + paquete.acMinima +
		"\n";
	return s;
}

void radioSendInfo() {
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

	/* rf69_manager.sendto((uint8_t *)radiopacket, strlen(radiopacket),
						DEST_ADDRESS);
	*/
	// rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
	// rf69.waitPacketSent();
	setupDatos();
}

void getRadioInfo() {
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
				// falloRadio = true;
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
	} else {
		DUMPSLN("RF69 Manager is not available")
	}
}