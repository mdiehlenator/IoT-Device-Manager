#ifdef FEATURE_PIN

#define MAXDIGITALPINS (24)
#define MAXANALOGPINS (3)

static int pin_last_update = -1000;
static int pin_update_interval = 5;

int digital_pin_mode[MAXDIGITALPINS];
int digital_pin_lastvalue[MAXDIGITALPINS];
int digital_pin_interval[MAXDIGITALPINS];
int digital_pin_lastpoll[MAXDIGITALPINS];
int digital_pin_maxinterval[MAXDIGITALPINS];

int analog_pin_mode[MAXANALOGPINS];
int analog_pin_lastvalue[MAXANALOGPINS];
int analog_pin_interval[MAXANALOGPINS];
int analog_pin_lastpoll[MAXANALOGPINS];
int analog_pin_mindelta[MAXANALOGPINS];
int analog_pin_maxinterval[MAXANALOGPINS];

struct mode {
  char *name;
  int value;
  int analog;
  void (*poll)(int pin);
};

struct mode modes[] = {
  {"digitalinput", INPUT, 0, poll_digitalinput},
  {"digitaloutput", OUTPUT, 0, dummy_poll},
  {"analoginput", INPUT, 1, poll_analoginput},
  {"analogoutput", OUTPUT, 0, dummy_poll},
};

void setup_pin() {
  int i;

  for (i=0; i<MAXDIGITALPINS; i++) {
    digital_pin_mode[i] = -1;
    digital_pin_lastvalue[i] = -1;
    digital_pin_interval[i] = 5;
    digital_pin_lastpoll[i] = -1;
    digital_pin_maxinterval[i] = 30;
  }

   for (i=0; i<MAXANALOGPINS; i++) {
    analog_pin_mode[i] = -1;
    analog_pin_lastvalue[i] = -1;
    analog_pin_interval[i] = 5;
    analog_pin_lastpoll[i] = -1;
    analog_pin_maxinterval[i] = 30;
    analog_pin_mindelta[i] = 5;
  }
  
}

void  update_pin() {
  int i;

  for (i=0; i<MAXDIGITALPINS; i++) {
    if (digital_pin_mode[i] == -1) { continue; }
    
    if ((digital_pin_lastpoll[i]+digital_pin_interval[i] < wallclock) || (digital_pin_lastpoll[i]+digital_pin_maxinterval[i] < wallclock)) {
      DEBUG("Running it for (%i)\n", i, "", "","");
      (modes[digital_pin_mode[i]].poll)(i);
      digital_pin_lastpoll[i] = wallclock;
    } else {
      if (wallclock-pin_last_update < pin_update_interval) { return; }
    
      pin_last_update = wallclock;
      DEBUG("Skipping it from for (%i)\n", i, "", "","");
    }
  }
  
  for (i=0; i<MAXANALOGPINS; i++) {
    if (analog_pin_mode[i] == -1) { continue; }
    
    if ((analog_pin_lastpoll[i]+analog_pin_interval[i] < wallclock) || (analog_pin_lastpoll[i]+analog_pin_maxinterval[i] < wallclock)) {
      DEBUG("Running it for (A%i)\n", i, "", "","");
      (modes[analog_pin_mode[i]].poll)(i);
      analog_pin_lastpoll[i] = wallclock;
    } else {
      if (wallclock-pin_last_update < pin_update_interval) { return; }
      pin_last_update = wallclock;
      DEBUG("Skipping it from for (A%i)\n", i, "", "","");
    }
  }
}

void pinmode(char *topic, char *message) {
  int pin, value, i;
  int found = 0;
  
  DEBUG("PINMODE: %s %s\n", topic, message,"","");

  value = String(message).toInt();
  for (i=0; i<sizeof(modes); i++) {  
    DEBUG("Finding pinmode for  (%s)\n", message, "", "","");

    if (strncmp(modes[i].name, message, strlen(modes[i].name)) == 0) {
        found++;
        break;
    }
  }

  if (!found) { return; }

  if (modes[i].analog == 0) {
    pin = String(topic[8]).toInt()*10 + String(topic[9]).toInt();
    digital_pin_mode[pin] = i;
    DEBUG("SETTING PINMODE #%i = %i\n", pin,i,"","");
    pinMode(pin,modes[i].value);
    sprintf(buffer1, "%s%s/manager/%s/status/pinmode/%i", prefix, suffix, MAC, pin);
  } else {
    pin = String(topic[9]).toInt();
    analog_pin_mode[pin] = i;
    DEBUG("SETTING PINMODE #A%i = %i\n", pin,i,"","");
    pinMode(A0,modes[i].value);  // We need to get rid of this constant.
    sprintf(buffer1, "%s%s/manager/%s/status/pinmode/A%i", prefix, suffix, MAC, pin);
  }
  
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

  pin_value(pin, value, 0);
   
  return;
}

void digitalread(char *topic, char *message) {
  int pin, value;
  DEBUG("digitalread: %s\n", message, "","","");

  pin = String(topic[13]).toInt()*10 + String(topic[14]).toInt();
  value = digitalRead(pin);

  pin_value(pin, value, 0);
   
  DEBUG("GETTING PIN #%i = %i\n", pin,value, "", "");
  
  return;
}

void analogread(char *topic, char *message) {
  int pin, value;
  DEBUG("analogread: %s\n", message, "", "", "");

  pin = String(topic[11]).toInt();

  value = analogRead(A0);
  
  pin_value(pin, value, 1);
   
  DEBUG("GETTING PIN #%i = %i\n", pin,value, "", "");
  
  return;
}

void analogwrite(char *topic, char *message) {
  int pin, value;
  DEBUG("analogwrite: %s\n", message, "", "", "");

  pin = String(topic[12]).toInt()*10 + String(topic[13]).toInt();

  value = String(message).toInt();
    
  DEBUG("SETTING PIN #%i = %i\n", pin,value, "", "");
  
  analogWrite(A0, value);
 
  pin_value(pin, value, 1);
   
  return;
}

void  poll_digitalinput(int pin) {
  int value;
  
  DEBUG("poll_digitalinput from for (%i)\n", pin, "", "","");
  value = digitalRead(pin);

  if (value != digital_pin_lastvalue[pin]) {
    pin_value(pin, value, 0);
  }
}

void  poll_analoginput(int pin) {
  int value;
  
  DEBUG("poll_analoginput for (%i)\n", pin, "", "","");
  value = analogRead(A0);

  if ((value != analog_pin_lastvalue[pin]) || (analog_pin_lastpoll[pin]+analog_pin_maxinterval[pin] < wallclock)) {
    pin_value(pin, value, 1);
  }
}

void  pin_value(int pin, int value, int analog) {

  DEBUG("X: (%i) = (%i) %i\n", pin,value,analog,0);

  if (analog == 0) {
    digital_pin_lastvalue[pin] = value;
    sprintf(buffer1, "%s%s/manager/%s/status/pinvalue/%i", prefix, suffix, MAC, pin);
  } else {
    analog_pin_lastvalue[pin] = value;
    sprintf(buffer1, "%s%s/manager/%s/status/pinvalue/A%i", prefix, suffix, MAC, pin);    
  }
  
  sprintf(buffer2, "%i", value);

  DEBUG("X: (%s) - (%s)\n", buffer1, buffer2, 0,0);
  
  publish(buffer1, buffer2); 

  return;
}

void dummy_poll(int pin) {
  DEBUG("dummmy from for (%i)\n", pin, "", "","");
}


#endif
