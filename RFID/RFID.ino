#define SS_PIN 4   //D2
#define RST_PIN 5  //D1

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>


const char* ssid = "Galaxy S9+35d5";
const char* password = "ezgi1229";

const char* serverUrl = "http://3.106.170.176:3000/verifytagid";

WiFiClient wifiClient;
HTTPClient http;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
int statuss = 0;
int out = 0;

String tagID = "";

void setup() {
  Serial.begin(9600);  // Initiate a serial communication
  delay(10);
  SPI.begin();         // Initiate  SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522

  // Connect to WiFi
  Serial.println();
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
    Serial.print("Connecting to WiFi...");
    Serial.println(WiFi.status());
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



void loop() {
  // put your main code here, to run repeatedly:
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  tagID = content;
  Serial.println(tagID);
  mfrc522.PICC_HaltA();

  if ((WiFi.status() == WL_CONNECTED)) {
    http.begin(wifiClient, serverUrl);
    StaticJsonDocument<200> doc;
    doc["UID"] = tagID;
    String payload;
    serializeJson(doc, payload);
    Serial.print(payload);

    http.begin(wifiClient, serverUrl);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.PUT(payload);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String responseBody = http.getString();
    Serial.println(responseBody);
    http.end();
    // http.begin(wifiClient, serverUrl);
    //   http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // http.addHeader("Content-Type", "application/json");
    // http.addHeader("Accept", "*/*");
    // http.addHeader("Accept-Encoding", "gzip, deflate, br");
    // http.addHeader("Connection", "keep-alive");
    // http.addHeader("User-Agent", "PostmanRuntime/7.31.3");

    // http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS, 10);
    
    // http.setFollowRedirectsLimit(10);
    // http.setRedirect(true);
    // http.setFollowRedirects(followRedirects_t follow);
    // http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  // http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    
    // int httpResponseCode = http.GET();

    
    // http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    // http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    // String responseBody = http.getString();
    // Serial.print("HTTP Response code: ");
    // Serial.println(httpResponseCode);
    // Serial.println(responseBody);
   // http.end();
  }
  delay(10000);
}
