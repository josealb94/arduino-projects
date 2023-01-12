#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6 ,7); // rs, e, d3, d2, d1, d0 | rs, e, d4, d5, d6, d7

void setup() {
  lcd.begin(16, 2); // cols, rows
}

void programElapsedTime() {
  lcd.setCursor(0, 0); // col, row, line 1
  lcd.print("Hola, han pasado");
  lcd.setCursor(0, 1); // line 2
  // millis => time elapsed since the start of the program in milliseconds
  lcd.print(millis() / 1000); // time in seconds
  lcd.print(" seg.");
}

void flashingMessage() {
  lcd.setCursor(0, 0);
  lcd.print("Alarma activa !");
  lcd.display();
  delay(500);
  lcd.noDisplay();
  delay(500);
}

void showCursor() {
  lcd.setCursor(0, 0);
  lcd.print("Ingrese clave:");
  lcd.setCursor(0, 1);
  lcd.cursor();
  delay(500);
  lcd.noCursor();
  delay(500);
}

void moreCharactersThanColumns() {
  // 40 character limit to display on a line
  lcd.setCursor(2, 0);
  lcd.print("Prueba scroll");
  lcd.scrollDisplayLeft();
  delay(1000);
}

void loop() {
  moreCharactersThanColumns();
}
