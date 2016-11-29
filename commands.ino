#include "globals.h"

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




void  pin_value(int pin, int value) {
  pin_lastvalue[pin] = value;

  sprintf(buffer1, "%s%s/manager/%s/status/pinvalue/%i", prefix, suffix, MAC, pin);
  sprintf(buffer2, "%i", value);

  mqtt.publish(buffer1, buffer2); 

  return;
}

