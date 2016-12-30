#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Stream.h>

#include "globals.h"
#include "config.h"

char buffer1[100];
char buffer2[100];

void setup() {

#ifdef DEBUGGING
  setup_serial();
#endif

#ifdef FEATURE_PIN
  setup_pin();
#endif

  setup_wifi();
  setup_mqtt();
  
#ifdef FEATURE_SSD1306
  setup_display();
#endif

#ifdef FEATURE_DHTXX
  setup_dhtxx();
#endif

  setup_polling();
  
}

void loop() {
  update_mqtt();

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

