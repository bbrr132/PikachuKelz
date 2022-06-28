#include <WiFiManager.h>
#define TRIGGER_PIN 1
int timeout = 60; // seconds to run for
#define apName "Pikachu"

void startWM(){
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  // put your setup code here, to run once:
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  WiFiManager wm;
  //wm.resetSettings();
  bool res;
  res = wm.autoConnect(apName);
  if(!res) {
     Serial.println("Failed to connect");
     // ESP.restart();
  } 
}

void checkWM(){
  if ( digitalRead(TRIGGER_PIN) == LOW) {
    WiFiManager wm;    
    //wm.resetSettings();
    wm.setConfigPortalTimeout(timeout);
    if (!wm.startConfigPortal(apName)) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      ESP.restart();
      delay(5000);
    }
    Serial.println("connected...yeey :)");
}
}
