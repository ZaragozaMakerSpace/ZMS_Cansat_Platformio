#include <EEPROM.h>
uint8_t eeAddress = 0;
void eepromSave() {
	if (salvar) {
		miniPaquete.idPaquete = (int16_t)(paquete.idPaquete);
		miniPaquete.altitud = (int8_t)(paquete.altitud / 10);
		miniPaquete.temperatura = (int8_t)paquete.temperatura;
		miniPaquete.acMaxima = (int8_t)acMaxima4;
		miniPaquete.acMinima = (int8_t)acMinima4;
		EEPROM.put(eeAddress, miniPaquete);
		eeAddress = ((miniPaquete.idPaquete) / 4) * sizeof(miniPaquete);
		DUMPSLN("grabando");
		acMaxima4 = 0;
		acMinima4 = 255;
	} else {
		DUMPSLN("No se graba");
	}
}

void eepromGet() {
	String s = "";
	for (uint8_t i = 0; i < 1024 / sizeof(miniPaquete); i++) {
		EEPROM.get(i * sizeof(miniPaquete), miniPaquete);
		s += String(i) + ":" + String(miniPaquete.idPaquete) + "," +
			 String(miniPaquete.altitud) + "," +
			 String(miniPaquete.temperatura) + "," +
			 String(miniPaquete.acMaxima) + "," + String(miniPaquete.acMinima);
		DUMPV(s);
		s = "";
	}
}