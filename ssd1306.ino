#ifdef FEATURE_SSD1306

#include "globals.h"
#include "SSD1306.h"
//#include "SSD1306Brzo.h"

SSD1306  display(0x3c, 2, 0);

void update_display() {
  //display.clear();

  display.display();  
}

void  setup_display() {
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  DEBUG("Display is connected",0,0,0,0);
  display.clear();
  display.display();
}

void clearscreen(char *topic, char *message) {
  display.clear();  display.display();

}

void text(char *topic, char *message) {
  int x,y;

  DEBUG("Display text: (%s) (%s)\n", topic, message,0,0);
  
  x = String(topic[5]).toInt()*100 + String(topic[6]).toInt()*10 + String(topic[7]).toInt();
  y = String(topic[9]).toInt()*100 + String(topic[10]).toInt()*10 + String(topic[11]).toInt();

  DEBUG("Display text: (%s) (%i,%i)\n", message,x,y,0);

  display.drawStringMaxWidth(x,y,64-x,message);
  display.display();
}

void setfont(char *topic, char *message) {
  int f;

  DEBUG("Setfont: (%s) (%s)\n", topic, message,0,0);
  
  f = String(message).toInt();
  
  display.setFont("ArialMT_Plain_10");
  display.display();
}

#endif
