#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Function initialization
void writeInLcd(int col, int row, String message);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Starting....");
  delay(1000);
  lcd.clear();
}

void loop() {

}

// Functions
void writeInLcd(int col, int row, String message) {
  /*
    If you want to print "Hello World", you write as follows:
    writeInLcd(0, 0, "Hello World");
  */
  lcd.setCursor(col, row);
  lcd.print(message);

  delay(1000); // In order to see the words displayed.
}