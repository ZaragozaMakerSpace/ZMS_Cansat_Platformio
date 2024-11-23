struct SensorData {
	uint16_t idPacket;
	float accelX, accelY, accelZ;
	float pressure;
	float altitude;
} data;

void eepromGet() {
	for (uint8_t i = 0; i < 1024 / sizeof(data); i++) {
		EEPROM.get(i * sizeof(data), data);
		DUMP("#Packet: ", data.idPacket);
		DUMP(" accX: ", data.accelX)
		DUMP(" accY: ", data.accelY)
		DUMP(" accZ: ", data.accelZ)
		DUMP(" pressure: ", data.pressure)
		DUMP(" altitude: ", data.altitude)
	}
}

void eepromSave() {
	EEPROMWrite(eeAddress, data);
	eeAddress = ((data.idPacket) / 4) * sizeof(data);

	// EEPROM.put(eeAddress, miniPaquete);
	// eeAddress = ((miniPaquete.idPaquete) / 4) * sizeof(miniPaquete);
	// DUMPSLN("grabando");

	// DUMPSLN("No se graba");
}