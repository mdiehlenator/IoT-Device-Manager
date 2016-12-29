#ifdef FEATURE_DHTXX

#include "DHT.h"
#include "globals.h"

DHT dht(2, DHTTYPE);

int last_update = 0;
int update_interval = 30;

void  setup_dhtxx() {
  DEBUG("DHTxx initialized!\n",0,0,0,0);

  dht.begin();

}

void  update_dhtxx() {

  if (wallclock-last_update < update_interval) { return; }

  last_update = wallclock;
  
humidity("a","b");
tempc("a","b");
tempf("a","b");
  
//  float hif = dht.computeHeatIndex(f, h);
//  float hic = dht.computeHeatIndex(t, h, false);
 
}

void tempc(char *topic, char *message) {
  float t = dht.readTemperature();
  sprintf(buffer1, "%s%s/manager/%s/status/tempc", prefix, suffix, MAC);
  sprintf(buffer2, "%i.%i", int(t),(t-int(t))*100);
  publish(buffer1, buffer2);
  DEBUG("(%s) - (%s)\n", buffer1, buffer2, 0, 0);
}

void tempf(char *topic, char *message) {
  float t = dht.readTemperature(true);
  sprintf(buffer1, "%s%s/manager/%s/status/tempf", prefix, suffix, MAC);
  sprintf(buffer2, "%i.%i", int(t),(t-int(t))*100);
  publish(buffer1, buffer2);
  DEBUG("(%s) - (%s)\n", buffer1, buffer2, 0, 0);
}


void humidity(char *topic, char *message) {
  float h = dht.readHumidity();
  sprintf(buffer1, "%s%s/manager/%s/status/humidity", prefix, suffix, MAC);
  sprintf(buffer2, "%i.%i", int(h),(h-int(h))*100);
  publish(buffer1, buffer2);
  DEBUG("(%s) - (%s)\n", buffer1, buffer2, 0, 0);
}

#endif
