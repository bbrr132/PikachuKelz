#include "AdafruitIO_WiFi.h"
#include "secrets.h"
#include "JigglyPuff.h"

#if defined(USE_AIRLIFT) || defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE) ||         \
    defined(ADAFRUIT_PYPORTAL)
// Configure the pins used for the ESP32 connection
#if !defined(SPIWIFI_SS) // if the wifi definition isnt in the board variant
// Don't change the names of these #define's! they match the variant ones
#define SPIWIFI SPI
#define SPIWIFI_SS 10 // Chip select pin
#define NINA_ACK 9    // a.k.a BUSY or READY pin
#define NINA_RESETN 6 // Reset pin
#define NINA_GPIO0 -1 // Not connected
#endif
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, ssid, password, SPIWIFI_SS,
                   NINA_ACK, NINA_RESETN, NINA_GPIO0, &SPIWIFI);
#else
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY);
#endif

#define IO_LOOP_DELAY 500 
unsigned long lastUpdate;
int flag=0;
unsigned long lastSent = 0;
int prevMode=0;

AdafruitIO_Group *group = io.group(FEED_GROUP_NAME);

void handleSecondsPerMode(AdafruitIO_Data *data) {
  SECONDS_PER_PALETTE=data->toInt();
  if (SECONDS_PER_PALETTE<1)
  {
    SECONDS_PER_PALETTE = 1;
  }
  writeFile(LittleFS, "/secondspermode.txt",String(SECONDS_PER_PALETTE).c_str());
}
void handleTwinkleDensity(AdafruitIO_Data *data) {
  TWINKLE_DENSITY=data->toInt();
  writeFile(LittleFS, "/twinkledensity.txt",String(TWINKLE_DENSITY).c_str());
}
void handleTwinkleSpeed(AdafruitIO_Data *data) {
  TWINKLE_SPEED=data->toInt();
  writeFile(LittleFS, "/twinklespeed.txt",String(TWINKLE_SPEED).c_str());
}
void handleBrightness(AdafruitIO_Data *data) {
  BRIGHTNESS=data->toInt();
  writeFile(LittleFS, "/brightness.txt",String(BRIGHTNESS).c_str());
}
void handlePalette(AdafruitIO_Data *data) {
  MODE=data->toInt();
  writeFile(LittleFS, "/mode.txt",String(MODE).c_str());
  flag=1;
}
void handleMode(AdafruitIO_Data *data) {
}

void UpdateDataIO(){
  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    io.run();
    lastUpdate = millis();
  }
}

void updateData(){
  if(flag==1&&lastSent+4000<millis()&&MODE!=prevMode)
  {
    prevMode=MODE;
    lastSent=millis();
    group->set("mode", modes[MODE]);
    group->save();
    flag=0;
  }
}
void AdafruitIOStart(){
  io.connect();
  group->onMessage("secondspermode", handleSecondsPerMode);
  group->onMessage("twinkledensity", handleTwinkleDensity);
  group->onMessage("twinklespeed", handleTwinkleSpeed);  
  group->onMessage("brightness", handleBrightness); 
  group->onMessage("palette", handlePalette); 
  group->onMessage("mode", handleMode); 
}
