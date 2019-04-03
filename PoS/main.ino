#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <U8g2lib.h>
#include <Keypad.h>
#include <ArduinoJson.h>
#include <string.h>

char wifiSSID[] = "YOUR-WIFI-SSID";
char wifiPASS[] = "YOUR-WIFI-PASS";

const char* host = "api.opennode.co";
const int httpsPort = 443;
String amount = "";
String apikey = "MERCHANTS-OPENNODE-INVOICE-API-KEY";
String description = "cardpayment"; //invoice description
String hints = "false";
String price;
String hexvalues = "";
String macaroontmp;
String macaroon;
String data_lightning_invoice_payreq = "";
String data_status = "unpaid";
String data_id = "";
int counta = 0;

#include "apicalls.h"

#define RST_PIN         22           // Configurable, see typical pin layout above
#define SS_PIN          5          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 17, /* data=*/ 4, /* cs=*/ 15, /* dc=*/ 2, /* reset=*/ 16);

//Set keypad
const byte rows = 4; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[rows] = {32, 33, 25, 26}; //connect to the row pinouts of the keypad
byte colPins[cols] = {27, 14, 12, 13}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
int checker = 0;
char maxdig[20];


void keypadamount(){

   u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(0,20,"Sats:");
    } while ( u8g2.nextPage() );
  
  while (checker < 20){

   char key = keypad.getKey();

   if (key != NO_KEY){
    
   String virtkey = String(key);
   
   if (virtkey == "*"){
    memset(maxdig, 0, 20);
    checker = 20;
   }

   if (virtkey == "#"){
      u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(0,20,"...");
    } while ( u8g2.nextPage() );
    
    checker = 20;
   }
   else{

    maxdig[checker] = key;
    checker++;
    
    Serial.println(maxdig);

    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(0,20,"Sats:");
      u8g2.drawStr(0,40, maxdig);

    } while ( u8g2.nextPage() );

   }
   }

  }
  checker = 0;
}


void cardcheck() {
  
    u8g2.firstPage();
    do {
       u8g2.setFont(u8g2_font_ncenB14_tr);
       u8g2.drawStr(0,20,"Tap card");
       }  while ( u8g2.nextPage() ); 
    
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

   //some variables we need
    byte block;
    byte len = 18; 
    //len = 18;
    MFRC522::StatusCode status;

    //-------------------------------------------

    // Look for new cards
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
     return;
    }

    Serial.println(F("**Card Detected:**"));

   //-------------------------------------------FIRST 16 BYTE PART OF API KEY

    byte buffer2[18];
    block = 1;

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    status = mfrc522.MIFARE_Read(block, buffer2, &len);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Reading failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    //PRINT first bit
    for (uint8_t i = 0; i < 16; i++) {
      Serial.write(buffer2[i] );
      macaroontmp +=  char(buffer2[i]);
     }

    //---------------------------------------- LAST 16 BYTE PART OF API KEY

    byte buffer3[18];
    block = 2;

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    status = mfrc522.MIFARE_Read(block, buffer3, &len);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("Reading failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
  
    //PRINT last bit
    for (uint8_t i = 0; i < 16; i++) {
      Serial.write(buffer3[i] );
      macaroontmp += char(buffer3[i]);
    }
    
    //----------------------------------------

    Serial.println(F("\n**End Reading**\n"));

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  
    if(macaroontmp.length() > 6){
    
      //Format the API key and put in a String
      macaroon = macaroontmp.substring(0, 8) + "-" + macaroontmp.substring(8, 12) + "-" + macaroontmp.substring(12, 16) + "-" + macaroontmp.substring(16, 20) + "-" + macaroontmp.substring(20, 32);
      macaroontmp = "";
      Serial.println(macaroon);
      makepayment(macaroon);

                 counta = 121;
          u8g2.firstPage();
          do {
             u8g2.setFont(u8g2_font_ncenB14_tr);
             u8g2.drawStr(0,20,"Processing");
          }  while ( u8g2.nextPage() ); 
          
          checkpayment(data_id);
         
        if (data_status != "unpaid"){
                   u8g2.firstPage();
          do {
             u8g2.setFont(u8g2_font_ncenB14_tr);
             u8g2.drawStr(0,20,"Paid!");
          }  while ( u8g2.nextPage() ); 
         delay(2000);
       }
    
       else{
                   u8g2.firstPage();
          do {
            u8g2.setFont(u8g2_font_ncenB14_tr);
            u8g2.drawStr(0,20,"error!");
          }  while ( u8g2.nextPage() ); 
          delay(3000);
          counta = 121;
       }
        
     return;
    }
}

//*****************************************************************************************//

void setup() {
  Serial.begin(9600);                                           
  SPI.begin();                                                  
  mfrc522.PCD_Init();                                              
  u8g2.begin();


  WiFi.begin(wifiSSID, wifiPASS);
  while (WiFi.status() != WL_CONNECTED) {
   
     u8g2.firstPage();
       do {
         u8g2.setFont(u8g2_font_ncenB14_tr);
         u8g2.drawStr(0,20,"connecting...");
      } while ( u8g2.nextPage() );
      
   delay(100);
   }
   
  u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(0,20,"connected");
    } while ( u8g2.nextPage() );
    delay(1000);
}

void loop() {

  while (*maxdig == 0){
   keypadamount();
   amount = String(maxdig);
  }

  fetchpayment(amount);
  

  while (counta < 80){
   cardcheck();
   delay(200);
   counta++;
   Serial.print(".");
  }
  counta = 0;
  memset(maxdig, 0, 20);
}
