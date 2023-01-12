#include <Keypad.h>

#define ROWS 4
#define COLUMNS 4
char keys[ROWS][COLUMNS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte columnPins[COLUMNS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, ROWS, COLUMNS);

char key;
char password;
char code[7];
char master_code[7] = "123456";
byte index = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  key = keypad.getKey();
  if (key) {
    code[index++] = key;
    Serial.print(key);
  }
  if (index == 6) {
    if (!strcmp(code, master_code)) Serial.println(" Correcta");
    else Serial.println(" Incorrecta");
    index = 0;
  }
}
