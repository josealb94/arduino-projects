#define PIN_RGB_RED 10
#define PIN_RGB_GREEN 11
#define PIN_RGB_BLUE 9

void writeRGB(int red, int green, int blue, int timeToDelay = 2000) {
  analogWrite(PIN_RGB_RED, red);
  analogWrite(PIN_RGB_GREEN, green);
  analogWrite(PIN_RGB_BLUE, blue);
  delay(timeToDelay);
}

void setup() {
  pinMode(PIN_RGB_RED, OUTPUT);
  pinMode(PIN_RGB_GREEN, OUTPUT);
  pinMode(PIN_RGB_BLUE, OUTPUT);
}

void loop() {
  // red
  writeRGB(255, 0, 0);

  // green
  writeRGB(0, 255, 0);

  // blue
  writeRGB(0, 0, 255);

  // yellow
  writeRGB(255, 255, 0);

  // skyblue
  writeRGB(0, 255, 255);

  // pink
  writeRGB(255, 0, 255);

  // light pink
  writeRGB(255, 118, 192);
}
