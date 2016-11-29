#include "globals.h"
#include "SSD1306.h"
//#include "SSD1306Brzo.h"

SSD1306  display(0x3c, 2, 0);
const char* test[] = {
  "----",
  "Your husband",
  "is too",
  "awesome",
  "for words.",
  "-----",
  ""
};

void update_display() {
  display.clear();


  display.setLogBuffer(5, 30);

  for (uint8_t i = 0; i < 7; i++) {
    display.clear();
    delay(350);
    display.println(test[i]);

    //display.drawLine(0,0,32,32);
    
    display.drawLogBuffer(0, 0);
    display.drawString(50,90,"Hello");
    display.display();
}
     
  display.display();
   
}


void  setup_display() {
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  Serial.println("Display is connected");
  display.clear();
  display.display();
}
