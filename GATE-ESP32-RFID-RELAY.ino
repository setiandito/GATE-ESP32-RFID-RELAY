#include <SPI.h>
#include <MFRC522.h>

// PIN KE MFRC522
#define SS_PIN 5
#define RST_PIN 22
// PIN OUTPUT ESP32 KE INPUT RELAY
#define RELAY 27
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  pinMode(RELAY, OUTPUT); 
  digitalWrite(RELAY, HIGH);

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println(F("Scan Kartu RFID"));
}
 
void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Cek tipe kartu MIFARE
  if (piccType == MFRC522::PICC_TYPE_UNKNOWN ||  
    piccType == MFRC522::PICC_TYPE_ISO_14443_4) {
    Serial.println(F("OK"));

  // Menghidupkan relay
    digitalWrite(RELAY, LOW);
    delay(500);  
    digitalWrite(RELAY, HIGH);
    Serial.println(F("on-off relay"));
    Serial.println(F("jeda 5dtk"));
    Serial.println();

  // Jeda selama 5 detik
    delay(5000); 
  }
 
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


