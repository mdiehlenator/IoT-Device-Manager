#include "globals.h"

int pin_mode[MAXPINS];
int pin_values[MAXPINS];
int pin_intervals[MAXPINS];
int pin_lastpoll[MAXPINS];

void init_polling() {
  int i;

  for (i=0; i<MAXPINS; i++) {
    pin_mode[i] = -1;
    pin_values[i] = -1;
    pin_intervals[i] = 30;
    pin_lastpoll[i] = -1;
  }
  
}

void do_poll() {
  
}

void dummy_poll(int pin) {

  
}

