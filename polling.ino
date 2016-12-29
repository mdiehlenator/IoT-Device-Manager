#include "globals.h"

int wallclock = 0;
int last_milli = 0;

void  setup_polling() {
  last_milli = millis();
}

void do_poll() {
  int i;

#ifdef FEATURE_CORE
  update_core();
#endif

#ifdef FEATURE_SSD1306
  update_display();
#endif

#ifdef FEATURE_DHTXX
  update_dhtxx();
#endif

#ifdef FEATURE_PIN
  update_pin();
#endif

  if ((millis() - last_milli) > 1000) {
    last_milli = millis();
    wallclock++;
  }

}

