#include "globals.h"

struct mode {
  char *name;
  int value;
  void (*poll)(int pin);
};

struct mode modes[] = {
  {"digitalinput", INPUT, dummy_poll},
  {"digitaloutput", OUTPUT, dummy_poll},
  {"analoginput", INPUT, dummy_poll},
  {"analogoutput", OUTPUT, dummy_poll},
  
};

void ping(char *topic, char *message) {   
  DEBUG("PING message: %s\n", message, "","","");
  sprintf(buffer1, "%s%s/manager/%s/pong", prefix,suffix,MAC,IP);
  DEBUG("PING publish: (%s)\n", buffer1, "","","");
  mqtt.publish(buffer1, IP);
  return;
}

void pinmode(char *topic, char *message) {
  int pin, value, i;
  
  DEBUG("PINMODE: %s %s\n", topic, message,"","");

  pin = String(topic[8]).toInt();
  value = String(message).toInt();
  for (i=0; i<sizeof(modes); i++) {  
    DEBUG("Finding pinmode for  (%s)\n", message, "", "","");

    if (strncmp(modes[i].name, message, strlen(modes[i].name)) == 0) {
        break;
    }
  }
  
  pin_mode[pin] = i;

  DEBUG("SETTING PINMODE #%i = %i\n", pin,i,"","");
  pinMode(pin,modes[i].value);
 
  sprintf(buffer1, "%s%s/manager/%s/status/pinmode/%i", prefix, suffix, MAC, pin);

  mqtt.publish(buffer1, message);
      
  return;
}

void digitalwrite(char *topic, char *message) {
  int pin, value;
  Serial.printf("digitalwrite: %s\n", message);

  pin = String(topic[13]).toInt()*10 + String(topic[14]).toInt();
  value = String(message).toInt();
  
  DEBUG("SETTING PIN #%i = %i\n", pin,value, "", "");

  digitalWrite(pin, value);

  pin_value(pin, value);
   
  return;
}

void digitalread(char *topic, char *message) {
  int pin, value;
  DEBUG("digitalread: %s\n", message, "","","");

  pin = String(topic[13]).toInt();
  value = digitalRead(pin);

  pin_value(pin, value);
   
  DEBUG("GETTING PIN #%i = %i\n", pin,value, "", "");
  
  return;
}

void analogread(char *topic, char *message) {
  int pin, value;
  DEBUG("analogread: %s\n", message, "", "", "");

  pin = (topic[11]-48)*10 + topic[12]-48;
  value = analogRead(A0);
  
  pin_value(pin, value);
   
  DEBUG("GETTING PIN #%i = %i\n", pin,value, "", "");
  
  return;
}

void analogwrite(char *topic, char *message) {
  int pin, value;
  DEBUG("analogwrite: %s\n", message, "", "", "");

  pin = (topic[12]-48)*10 + topic[13]-48;
  value = String(message).toInt();
    
  DEBUG("SETTING PIN #%i = %i\n", pin,value, "", "");
  
  analogWrite(pin, value);
 
  pin_value(pin, value);
   
  return;
}

void reboot(char *topic, char *message) {


}

void version(char *topic, char *message) {
  sprintf(buffer1, "%s/raw/%s/version", prefix,MAC);
  sprintf(buffer2, "%i.%i", VERSIONMAJOR, VERSIONMINOR);

  Serial.printf("VERSION: %s => %s", buffer1, buffer2);

  mqtt.publish(buffer1, buffer2);

}

void uptime(char *topic, char *message) {
  sprintf(buffer1, "%s/raw/%s/uptime", prefix,MAC);
  sprintf(buffer2, "%i", millis()/1000);

  DEBUG("UPTIME: %i", millis()/1000, "", "", "");

  mqtt.publish(buffer1, buffer2);
}

void  pin_value(int pin, int value) {
  pin_values[pin] = value;

  sprintf(buffer1, "%s%s/manager/%s/status/pinvalue/%i", prefix, suffix, MAC, pin);
  sprintf(buffer2, "%i", value);

  mqtt.publish(buffer1, buffer1); 

  return;
}

