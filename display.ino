#include "globals.h"
#include "SSD1306.h"
#include "SSD1306Brzo.h"

SSD1306  display(0x3c, 2, 0);
//Adafruit_SSD1306 display(2);


void update_display() {
  //display.clearDisplay();

  for (int i = 0; i < 5; i++) {
    //display.display();
    //display.println("Hello world.");
    //display.println("\n");
  }        
}


void  setup_display() {
  //display.begin(0x3C,2,0);
  //display.clearDisplay();
  //display.drawPixel(10, 10, WHITE);
  //display.drawPixel(20, 10, WHITE);

  Serial.println("Display is commented out.");
  //display.display();

}
