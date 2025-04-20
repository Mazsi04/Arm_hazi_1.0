#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

byte transmitt_byte[11] = {72,101,108,108,111,32,87,111,114,108,100};
char *transmitt_char = "Hello World";

Adafruit_BME280 bme; // I2C interfész

void setup() {

    Serial.begin(9600);
    
    if (ELECHOUSE_cc1101.getCC1101()){        // Check the CC1101 Spi connection.
    Serial.println("Connection OK");
    }else{
    Serial.println("Connection Error");
    }
 
    ELECHOUSE_cc1101.Init();              // must be set to initialize the cc1101!
    ELECHOUSE_cc1101.setCCMode(1);       // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(812.50);   // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setSyncMode(2);  // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
 // ELECHOUSE_cc1101.setPA(10);      // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
    ELECHOUSE_cc1101.setCrc(1);     // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.

    Serial.println("Tx Mode");

      // BME280 inicializálása, ellenőrizzük, hogy a szenzor megfelelően működik-e
  if (!bme.begin(0x76)) {  // Ha szükséges, módosítsd a címet 0x77-re!
    Serial.println("Hiba: BME280 szenzor nem található!");
    while (1); // Végtelen ciklus, ha nincs szenzor
  }
}

void loop() {

  float temperature = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;  // hPa-ban
  float humidity = bme.readHumidity();
    // Átalakítjuk a float értékeket karakterlánccá a dtostrf() segítségével
  char tempStr[10];
  char presStr[10];
  char humStr[10];
  dtostrf(temperature, 4, 2, tempStr);  // 4 karakter szélesség, 2 tizedesjegy
  dtostrf(pressure, 4, 2, presStr);
  dtostrf(humidity, 4, 2, humStr);
//3 different methods to send data without gdo
//When sending, we give a little time to completely transmit the message (time in millis).
//You can shorten the time. It depends on the data rate and the packet length. Just try it out for fine tuning.
   char msg[50];
  snprintf(msg, sizeof(msg), "T:%s", tempStr);
  Serial.println(msg);
  ELECHOUSE_cc1101.SendData(msg,50);
  delay(500);

  snprintf(msg, sizeof(msg), "P:%s",presStr);
  Serial.println(msg);
  ELECHOUSE_cc1101.SendData(msg,50);
  delay(500);

  snprintf(msg, sizeof(msg), "H:%s",humStr);
  Serial.println(msg);
  ELECHOUSE_cc1101.SendData(msg,50);
  delay(500);



}
