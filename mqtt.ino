#include <PubSubClient.h>

const char* mqtt_server = MQTT_SERVER;
int mqtt_port = MQTT_PORT;
const char* mqtt_user = MQTT_USER;
const char* mqtt_passwd = MQTT_PASSWORD;
const char prefix[16] = MQTT_PREFIX;
const char suffix[16] = MQTT_SUFFIX;

PubSubClient mqtt(espclient);

void reconnect() {

  if (WiFi.status() != WL_CONNECTED) { return; }
  
  Serial.printf("Connection status: %i\n", mqtt.state());

  while (!mqtt.connected()) {
   Serial.println("Not connected to mqtt\n");
   
   if (mqtt.connect("ESP8266mqtt",mqtt_user, mqtt_passwd)) {
         Serial.println("Connected to mqtt\n");
   } else {
          Serial.printf("Connection failed: %i\n", mqtt.state());
          delay(1000);
   }
  }  

  sprintf(buffer1,"%s%s/%s/#", prefix,suffix, MAC);
  Serial.printf("Subscribing to %s.\n", buffer1, suffix);
  mqtt.subscribe(buffer1);
  
  sprintf(buffer1, "%s%s/manager/boot", prefix, suffix);
  mqtt.publish(buffer1, MAC);
  Serial.printf("Publishing: %s %s\n", buffer1, MAC);
}

void publish(char *t, char *m) {
    mqtt.publish(t, m); 
}
void update_mqtt() {
  
  mqtt.loop(); 
  
  if (!mqtt.connected()) {
    reconnect();
  }
}

void setup_mqtt() {
  if (WiFi.status() != WL_CONNECTED) { return; }

  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(callback);
}

