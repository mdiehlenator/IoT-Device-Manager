#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Stream.h>

#include "globals.h"
#include "config.h"

char buffer1[50];
char buffer2[50];

void setup() {

#ifdef DEBUGGING
  setup_serial();
#endif

  init_polling();

  setup_wifi();
  setup_mqtt();
  
#ifdef FEATURE_SSD1306
  setup_display();
#endif

#ifdef FEATURE_DTHXX
  setup_dthxx();
#endif
}

void loop() {
  update_mqtt();
  
#ifdef FEATURE_SSD1306
  update_display();
#endif

#ifdef FEATURE_DTHXX
  update_dthxx();
#endif

  do_poll();
  yield();
}

#ifdef DEBUGGING
void setup_serial() {
  Serial.begin(115200);

  while (!Serial);
  
  DEBUG("\n\n\nSerial is initialized","","","","");
  delay(500);
}
#endif

