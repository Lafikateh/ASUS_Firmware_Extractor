all:
	@$(CC) -std=c99 -Os -s -o ASUS_Firmware_Extractor Main.c

release:
	@$(CC) -std=c99 -Os -s -o ASUS_Firmware_Extractor Main.c

debug:
	@$(CC) -std=c99 -g3 -o ASUS_Firmware_Extractor Main.c

clean:
	@$(RM) ASUS_Firmware_Extractor
