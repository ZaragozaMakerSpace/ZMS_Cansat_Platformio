#ifdef __BMP280_H__

void readBMP() {
	DUMP("Temperatura = ", bmp.readTemperature());
	DUMPSLN(" *C");

	DUMP("Presión = ", bmp.readPressure());
	DUMPSLN(" Pa");

	DUMP("Altitud aproximada = ", bmp.readAltitude(1013.25));
	DUMPSLN(" m");
}

#endif