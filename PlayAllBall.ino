#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <MFRC522.h>
#include <string.h>

#define TFT_SS        9
#define TFT_RST       -1 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC        7

#define MFR_RST       -1
#define MFR_SS        10

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_SS, TFT_DC, TFT_RST);
MFRC522 mfrc522(MFR_SS, MFR_RST);  // Create MFRC522 instance

MFRC522::MIFARE_Key key; 
/*
 * Typical pin layout used:
 * --------------------------------------
 *             MFRC522      Arduino      
 *             Reader/PCD   Uno     
 * Signal      Pin          Pin          
 * --------------------------------------
 * RST/Reset   RST          9           
 * SPI SS      SDA(SS)      10           
 * SPI MOSI    MOSI         11 / ICSP-4  
 * SPI MISO    MISO         12 / ICSP-1   
 * SPI SCK     SCK          13 / ICSP-3
*/
// Init array that will store new NUID 
byte nuidPICC[4];


// color definitions
const uint16_t  Display_Color_Black        = 0x0000;
const uint16_t  Display_Color_Blue         = 0x001F;
const uint16_t  Display_Color_Red          = 0xF800;
const uint16_t  Display_Color_Green        = 0x07E0;
const uint16_t  Display_Color_Cyan         = 0x07FF;
const uint16_t  Display_Color_Magenta      = 0xF81F;
const uint16_t  Display_Color_Yellow       = 0xFFE0;
const uint16_t  Display_Color_White        = 0xFFFF;

// The colors we actually want to use
uint16_t        Display_Text_Color         = Display_Color_White;
uint16_t        Display_Background_Color    = Display_Color_Black;

unsigned long t = 0;

void setup() {
  // put your setup code here, to run once:

  digitalWrite(TFT_SS, HIGH);
  digitalWrite(MFR_SS, HIGH);

  digitalWrite(MFR_SS, LOW);

  Serial.begin(9600);    // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  digitalWrite(MFR_SS, HIGH);

  

  digitalWrite(TFT_SS, LOW);
  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.setFont();
  tft.fillScreen(Display_Background_Color);
  tft.setTextColor(Display_Text_Color);
  tft.setTextSize(1);

  digitalWrite(TFT_SS, HIGH);

}

void loop() {

  digitalWrite(TFT_SS, LOW);
  // put your main code here, to run repeatedly:
  tft.setCursor(0, 0);

  tft.setTextColor(Display_Background_Color);

  tft.print("beans" + String(t));

  tft.setCursor(0, 0);

  tft.setTextColor(Display_Text_Color);

  t = millis() / 1000;

  tft.print("beans" + String(t));
  printHexTFT(mfrc522.uid.uidByte, mfrc522.uid.size);

  digitalWrite(TFT_SS, HIGH);




  digitalWrite(MFR_SS, LOW);

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }


 
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = mfrc522.uid.uidByte[i];
  }

  // Halt PICC
  mfrc522.PICC_HaltA();

  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

  digitalWrite(MFR_SS, HIGH);

}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHexTFT(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    tft.print(buffer[i] < 0x10 ? " 0" : " ");
    tft.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
