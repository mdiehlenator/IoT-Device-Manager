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

  if (done == 1) { return; }

  done = 1;
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

  sprintf(buffer1, "%s%s/manager/%s/status/temperatureF", prefix, suffix, MAC);
  sprintf(buffer2, "%i", int(f));
  publish(buffer1, buffer2);
  Serial.printf("(%s) - (%s)\n", buffer1, buffer2);
  
}

#endif
