#include "globals.h"

int wallclock = 0;

void do_poll() {
  int i;
  
#ifdef FEATURE_SSD1306
  update_display();
#endif

#ifdef FEATURE_DHTXX
  update_dhtxx();
#endif

#ifdef FEATURE_PIN
  update_pin();
#endif

  wallclock++;
}

void dummy_poll(int pin) {
  DEBUG("dummmy from for (%i)\n", pin, "", "","");
}


