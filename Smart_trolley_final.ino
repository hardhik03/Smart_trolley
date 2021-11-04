#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#include <SPI.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <addons/TokenHelper.h> /* Provide the token generation process info. */
#include <addons/RTDBHelper.h> /* Provide the RTDB payload printing info and other helper functions.*/

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Hardhik"
#define WIFI_PASSWORD "12345678"

/* 2. Define the API Key */
#define API_KEY "AIzaSyBuWRPd7MFVAmn_3KpQI0ZteQJefd3VTzE"

/* 3. Define the RTDB URL */
#define DATABASE_URL "strolley-f0e4a-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "h.bunnypvv@gmail.com"
#define USER_PASSWORD "hardhik2203"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1
MFRC522 mfrc522(SS_PIN, RST_PIN); 

char input[12];
int count = 0;
int a;
int p1=0,p2=0,p3=0,p4=0;
int c1=0,c2=0,c3=0,c4=0;

double total = 0;
int count_prod = 0;
//char data1[12] = {'50 EE 77 2C'};
//char data2[12] = {'F0 DD 0D 19'};
//char data3[12] = {'15 DC EC 2A'};

int statuss = 0;
int out = 0;

void setup()
{

  Serial.begin(115200);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();
  Serial.println("...Welcome...");
  Serial.println("Smart Trolley - IOT J");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);  
  Serial.println("Start Shopping - Happy Shopping");

}

void loop()
{
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    }
    //Show UID on serial monitor
    Serial.println();
    //Serial.print(" UID tag :");
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
       Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
       Serial.print(mfrc522.uid.uidByte[i], HEX);
       content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
       content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    Serial.println();

    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
      sendDataPrevMillis = millis();  
      if (content.substring(1) == "50 EE 77 2C")
      {
        Serial.print("Milk Added");
        Serial.printf(Firebase.setString(fbdo, "/shop/itemadded", "Milk") ? "ok" : fbdo.errorReason().c_str());
        Serial.print("Price(Rs):24.00 ");
        Serial.printf(Firebase.setFloat(fbdo, "/shop/itemvalue", 24.00) ? "ok" : fbdo.errorReason().c_str());
        p1++;
        delay(1000);
        total = total + 24.00;
        Serial.print("Total = ");
        Serial.println(total);
        Serial.printf(Firebase.setFloat(fbdo, "/shop/total", total) ? "ok" : fbdo.errorReason().c_str());
        count_prod++;
      }
    
      else if (content.substring(1) == "F0 DD 0D 19")
      {
        Serial.print("Pen Added");      
        Serial.printf(Firebase.setString(fbdo, "/shop/itemadded", "Pen") ? "ok" : fbdo.errorReason().c_str());
        Serial.println("Price(Rs):10.00 ");
        Serial.printf(Firebase.setFloat(fbdo, "/shop/itemvalue", 10.00) ? "ok" : fbdo.errorReason().c_str());
        p1++;
        delay(1000);
        total = total + 10.00;
        Serial.print("Total = ");
        Serial.println(total);
        Serial.printf(Firebase.setFloat(fbdo, "/shop/total", total) ? "ok" : fbdo.errorReason().c_str());
        count_prod++;
      }
    
      else if (content.substring(1) == "15 DC EC 2A")
      {
        Serial.print("Dark Fantasy Added");            
        Serial.printf(Firebase.setString(fbdo, "/shop/itemadded", "Dark Fantasy") ? "ok" : fbdo.errorReason().c_str());
        Serial.println("Price(Rs):50.00 ");
        Serial.printf(Firebase.setFloat(fbdo, "/shop/itemvalue", 50.00) ? "ok" : fbdo.errorReason().c_str());
        p1++;
        delay(1000);
        total = total + 50.00;
        Serial.print("Total = ");
        Serial.println(total);
        Serial.printf(Firebase.setFloat(fbdo, "/shop/total", total) ?  "ok" : fbdo.errorReason().c_str());
        count_prod++;
      }
    
      else {
        Serial.println("Item not available");
        Serial.printf(Firebase.setString(fbdo, "/shop/itemadded", "No Item") ?  "ok" : fbdo.errorReason().c_str());
        Serial.printf(Firebase.setFloat(fbdo, "/shop/itemvalue", 00.00) ?  "ok" : fbdo.errorReason().c_str());
      }    
  }
}
