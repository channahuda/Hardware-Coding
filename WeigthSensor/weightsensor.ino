// Calibrating the load cell
#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 13;

HX711 scale;

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(); 

}

void loop() {
   
  if (scale.is_ready()) {
      
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
   
  } 
  else {
    Serial.println("HX711 not found.");
  //  Serial.println(scale.get_last_error());
    Serial.println(scale.is_ready());
  }
  delay(1000);
}

//calibration factor will be the (reading)/(known weight)
// calibration factor = 458.3158490566