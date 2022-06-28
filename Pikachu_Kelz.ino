#include "adafruitjp.h"
#include "wifi_manager_8266.h"
void setup() {
  startWM();
  startMemory();
  AdafruitIOStart();
  startLED();
}

void loop()
{
  checkWM();
  UpdateDataIO();
  drawLED();
  updateData();
}
