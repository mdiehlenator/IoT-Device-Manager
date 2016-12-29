#ifdef FEATURE_DHTXX

#include "DHT.h"
#include "globals.h"

DHT dht(2, DHTTYPE);

void  setup_dhtxx() {
  DEBUG("DHTxx initialized!\n",0,0,0,0);

  dht.begin();

}

void  update_dhtxx() {
  static int done = 0;

delay(2000);
  if (done == 1) { return; }

  done = 1;
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);



  sprintf(buffer1, "%s%s/manager/%s/status/temperatureF", prefix, suffix, MAC);
  sprintf(buffer2, "%i.%i", int(f),(f-int(f))*100);
  publish(buffer1, buffer2);
  DEBUG("(%s) - (%s)\n", buffer1, buffer2, 0, 0);
  
}

#endif
