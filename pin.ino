#ifdef FEATURE_PIN

static int pin_last_update = -1000;
static int pin_update_interval = 5;

int pin_mode[MAXPINS];
int pin_lastvalue[MAXPINS];
int pin_interval[MAXPINS];
int pin_lastpoll[MAXPINS];
int pin_mindelta[MAXPINS];
int pin_maxinterval[MAXPINS];

struct mode {
  char *name;
  int value;
  void (*poll)(int pin);
};

struct mode modes[] = {
  {"digitalinput", INPUT, poll_digitalinput},
  {"digitaloutput", OUTPUT, dummy_poll},
  {"analoginput", INPUT, poll_analoginput},
  {"analogoutput", OUTPUT, dummy_poll},
};

void setup_pin() {
  int i;

  for (i=0; i<MAXPINS; i++) {
    pin_mode[i] = -1;
    pin_lastvalue[i] = -1;
    pin_interval[i] = 5;
    pin_lastpoll[i] = -1;
    pin_mindelta[i] = 0;
    pin_maxinterval[i] = 30;
  }
 
}

void  update_pin() {
  int i;

  for (i=0; i<MAXPINS; i++) {
    if (pin_mode[i] == -1) { continue; }
      //DEBUG("Polling pin (%i)\n", i, "", "","");

      if ((pin_lastpoll[i]+pin_interval[i] < wallclock) || (pin_lastpoll[i]+pin_maxinterval[i] < wallclock)) {
        DEBUG("Running it from for (%i)\n", i, "", "","");
        (modes[pin_mode[i]].poll)(i);
        pin_lastpoll[i] = wallclock;
      } else {
        if (wallclock-pin_last_update < pin_update_interval) { return; }
        pin_last_update = wallclock;
        DEBUG("Skipping it from for (%i)\n", i, "", "","");
      }
  }
}

void pinmode(char *topic, char *message) {
  int pin, value, i;
  int found = 0;
  
  DEBUG("PINMODE: %s %s\n", topic, message,"","");

  pin = String(topic[8]).toInt()*10 + String(topic[9]).toInt();
  value = String(message).toInt();
  for (i=0; i<sizeof(modes); i++) {  
    DEBUG("Finding pinmode for  (%s)\n", message, "", "","");

    if (strncmp(modes[i].name, message, strlen(modes[i].name)) == 0) {
        found++;
        break;
    }
  }

  if (!found) { return; }
  
  pin_mode[pin] = i;

  DEBUG("SETTING PINMODE #%i = %i\n", pin,i,"","");
  pinMode(pin,modes[i].value);
 
  sprintf(buffer1, "%s%s/manager/%s/status/pinmode/%i", prefix, suffix, MAC, pin);
  sprintf(buffer2, "%i", pin);
  mqtt.publish(buffer1, modes[i].name);
      
  return;
}

void digitalwrite(char *topic, char *message) {
  int pin, value;
  DEBUG("digitalwrite: %s\n", message,0,0,0);

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

  pin = String(topic[13]).toInt()*10 + String(topic[14]).toInt();
  value = digitalRead(pin);

  pin_value(pin, value);
   
  DEBUG("GETTING PIN #%i = %i\n", pin,value, "", "");
  
  return;
}

void analogread(char *topic, char *message) {
  int pin, value;
  DEBUG("analogread: %s\n", message, "", "", "");

  pin = String(topic[10]).toInt()*10 + String(topic[11]).toInt();

  value = analogRead(A0);
  
  pin_value(pin, value);
   
  DEBUG("GETTING PIN #%i = %i\n", pin,value, "", "");
  
  return;
}

void analogwrite(char *topic, char *message) {
  int pin, value;
  DEBUG("analogwrite: %s\n", message, "", "", "");

  pin = String(topic[12]).toInt()*10 + String(topic[13]).toInt();

  value = String(message).toInt();
    
  DEBUG("SETTING PIN #%i = %i\n", pin,value, "", "");
  
  analogWrite(pin, value);
 
  pin_value(pin, value);
   
  return;
}

void  poll_digitalinput(int pin) {
  int value;
  
  DEBUG("poll_digitalinput from for (%i)\n", pin, "", "","");
  value = digitalRead(pin);

  if (value != pin_lastvalue[pin]) {
    pin_value(pin, value);
  }
}

void  poll_analoginput(int pin) {
  int value;
  
  DEBUG("poll_analoginput from for (%i)\n", pin, "", "","");
  value = analogRead(A0);

  if ((value != pin_lastvalue[pin]) || (pin_lastpoll[pin]+pin_maxinterval[pin] < wallclock)) {
    pin_value(pin, value);
  }
}

void  pin_value(int pin, int value) {
  pin_lastvalue[pin] = value;

  sprintf(buffer1, "%s%s/manager/%s/status/pinvalue/%i", prefix, suffix, MAC, pin);
  sprintf(buffer2, "%i", value);

  publish(buffer1, buffer2); 

  return;
}

void dummy_poll(int pin) {
  DEBUG("dummmy from for (%i)\n", pin, "", "","");
}


#endif
