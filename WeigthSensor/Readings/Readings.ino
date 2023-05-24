#include <Arduino.h>
#include "HX711.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

const char* ssid = "Galaxy S9+35d5";
const char* password = "ezgi1229";
// const char* ssid = "RoyalNet_Mumtaz";
// const char* password = "Mirokhan997";
const char* serverUrl = "http://3.106.170.176:3000/addTempWeight";

WiFiClient wifiClient;
HTTPClient http;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 13;

HX711 scale;
String UID = " D9 B9 B1 D5";


void setup() {
  Serial.begin(9600);

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
   
  delay(2500);

  Serial.println("HX711 Demo");
  Serial.println("Initializing the scale");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  
  Serial.println(scale.read()); 
  delay(2500);     // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  delay(2500);

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)
  
  //scale.set_scale(458.4075471698);
  scale.set_scale(458.3158490566);
                        // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");
  delay(50);
  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale
 

  Serial.println("Readings:");
}

void loop() {
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  int newReading = scale.get_units(10);
  // Serial.println("Previous Reading " + previousReading);
  // Serial.println("New Reading " + newReading);
  Serial.println("\t| average:\t");
  Serial.println(newReading);    
  if ((WiFi.status() == WL_CONNECTED)) {
  http.begin(wifiClient, serverUrl);
  StaticJsonDocument<200> doc;
  doc["UID"] = UID;
  doc["weight"] = newReading;
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
  }
  
  delay(2500);
  scale.power_down();             // put the ADC in sleep mode
  delay(5000);
  scale.power_up();
}

