#ifdef FEATURE_DTHXX

#include "DHT.h"
#include "globals.h"
#include "config.h"

DHT dht(2, DHTTYPE);

int done = 0;

void  setup_dthxx() {
  DEBUG("DHTxx test!",0,0,0,0);

  dht.begin();

}

void  update_dthxx() {

  if (done == 1) { return; }

  done = 1;
  
    float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
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
