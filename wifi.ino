#include "globals.h"

char IP[16] = "";
char MAC[18] = "";
uint8 MAC_NVRAM[7];

void setup_wifi() {
  int count = 1;
  int i = 0;

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.printf("Not connected to wifi (%i).\n",count++);
  }

  Serial.println("Wifi is connected"); 
  WiFi.macAddress(MAC_NVRAM);

  sprintf(IP, "%i.%i.%i.%i\0", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  sprintf(MAC, "%02x:%02x:%02x:%02x:%02x:%02x\0", MAC_NVRAM[0], MAC_NVRAM[1], MAC_NVRAM[2], MAC_NVRAM[3], MAC_NVRAM[4],MAC_NVRAM[5]);

  Serial.printf("I am: %s - %s\n", IP, MAC);
}
