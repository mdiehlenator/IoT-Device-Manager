#include "globals.h"

int pin_mode[MAXPINS];
int pin_lastvalue[MAXPINS];
int pin_interval[MAXPINS];
int pin_lastpoll[MAXPINS];
int pin_mindelta[MAXPINS];
int pin_maxinterval[MAXPINS];

int wallclock = 0;

void init_polling() {
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

void do_poll() {
  int i;

  for (i=0; i<MAXPINS; i++) {
    if (pin_mode[i] == -1) { continue; }

      if ((pin_lastpoll[i]+pin_interval[i] < wallclock) || (pin_lastpoll[i]+pin_maxinterval[i] < wallclock)) {
        DEBUG("Running it from for (%i)\n", i, "", "","");
        (modes[pin_mode[i]].poll)(i);
        pin_lastpoll[i] = wallclock;
      } else {
        DEBUG("Skipping it from for (%i)\n", i, "", "","");

      }
  }

  wallclock++;
  delay(1000);
}

void dummy_poll(int pin) {
  DEBUG("dummmy from for (%i)\n", pin, "", "","");
}


