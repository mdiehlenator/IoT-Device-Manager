
void reconnect() {
  Serial.printf("Connection status: %i\n", mqtt.state());

  while (!mqtt.connected()) {
   Serial.println("Not connected to mqtt\n");
   
   if (mqtt.connect("ESP8266mqtt")) {
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


void update_mqtt() {
  
  mqtt.loop(); 
  
  if (!mqtt.connected()) {
    reconnect();
  }
}

void setup_mqtt() {
  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(callback);
}

