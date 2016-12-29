#ifdef FEATURE_DHTXX

#include "DHT.h"
#include "globals.h"

DHT dht(4, DHTTYPE);

static int dhtxx_last_update = -1000;
static int dhtxx_update_interval = 30;

void  setup_dhtxx() {
  DEBUG("DHTxx initialized!\n",0,0,0,0);

  dht.begin();

}

void  update_dhtxx() {

  if (wallclock-dhtxx_last_update < dhtxx_update_interval) { return; }
  dhtxx_last_update = wallclock;
  
  humidity("","");
  tempc("","");
  tempf("","");
  
//  float hif = dht.computeHeatIndex(f, h);
//  float hic = dht.computeHeatIndex(t, h, false);
 
}

void tempc(char *topic, char *message) {
  float t = dht.readTemperature();

DEBUG("XXX: %i\n", int(t*100), 0,0,0);
  
  sprintf(buffer1, "%s%s/manager/%s/status/tempc", prefix, suffix, MAC);
  sprintf(buffer2, "%i.%i", int(t), 0); //,(t-int(t))*100);
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
