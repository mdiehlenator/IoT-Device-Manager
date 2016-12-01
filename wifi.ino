#include "globals.h"

#include <WiFiClient.h>
WiFiClient espclient;

const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;

char IP[16] = "";
char MAC[18] = "";
uint8 MAC_NVRAM[7];

void setup_wifi() {
  int count = 1;
  int i = 0;

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DEBUG("Not connected to wifi (%i) (%i).\n",WiFi.status(), count++,0,0);

    if (count > 20) {
      DEBUG("Wifi connection has failed.\n", 0,0,0,0);
      return;
    }
  }
  
  Serial.println("Wifi is connected"); 
  WiFi.macAddress(MAC_NVRAM);

  sprintf(IP, "%i.%i.%i.%i\0", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  sprintf(MAC, "%02x:%02x:%02x:%02x:%02x:%02x\0", MAC_NVRAM[0], MAC_NVRAM[1], MAC_NVRAM[2], MAC_NVRAM[3], MAC_NVRAM[4],MAC_NVRAM[5]);

  DEBUG("I am: %s - %s\n", IP, MAC,0,0);
}
