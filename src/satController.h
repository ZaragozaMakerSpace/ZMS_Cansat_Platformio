void satLaunched() {
	if (paquete.acMaxima > acc_umbral) {
		long tiempoParaLanzar = millis() - launchTimerController;
		DUMP("Time for launch ", tiempoParaLanzar);
		if (tiempoParaLanzar > launchTimer) {
			// EEPROM RESET
			eeAddress = 0;
			paquete.idPaquete = 0;
			cansatLanzado = true;
			DUMPSLN("CANSAT LANZADO");
			// Enviar dato por radio de cansat lanzado
			char radiopacket[64]; // Tenemos una cadena de texto
			sprintf(radiopacket, "%s",
					String("ENVIADO")
						.c_str()); // Metemos en la cadena de texto la parte que
								   // nos interesa del mensaje(tipo String)
			rf69_manager.sendto((uint8_t *)radiopacket, strlen(radiopacket),
								DEST_ADDRESS);
		}
	}
}