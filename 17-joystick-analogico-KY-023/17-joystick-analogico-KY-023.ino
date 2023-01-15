#define PIN_LED_LEFT 3
#define PIN_LED_UP 5
#define PIN_LED_RIGHT 6
#define PIN_LED_DOWN 9
#define PIN_LED_BUTTON 11
#define PIN_JOYSTICK_VRX A1
#define PIN_JOYSTICK_VRY A0
#define PIN_JOYSTICK_SW 10

void directionLedManagement(int value, int min, int max, int pinLed, bool isInvertedValue = false) {
  int minimumForMapper = 0;
  int maxForMapper = 255;
  if (isInvertedValue) {
    minimumForMapper = 255;
    maxForMapper = 0;
    min = min - 1;
  } else {
    max = max + 1;
  }
  if (value > min && value < max) {
    analogWrite(pinLed, map(value, min, max, minimumForMapper, maxForMapper));
  } else {
    analogWrite(pinLed, 0);
  }
}

void setup() {
  Serial.begin(9600);

  // config output pins
  pinMode(PIN_LED_LEFT, OUTPUT);
  pinMode(PIN_LED_UP, OUTPUT);
  pinMode(PIN_LED_RIGHT, OUTPUT);
  pinMode(PIN_LED_DOWN, OUTPUT);
  pinMode(PIN_LED_BUTTON, OUTPUT);
  
  // config input pins
  // inputs of type 'AnalogIn' do not need to be declared
  pinMode(PIN_JOYSTICK_SW, INPUT);

}

void loop() {
  // joystick reading
  int x = analogRead(PIN_JOYSTICK_VRX);
  int y = analogRead(PIN_JOYSTICK_VRY);
  int sw = digitalRead(PIN_JOYSTICK_SW);

  directionLedManagement(x, 0, 480, PIN_LED_LEFT, true);
  directionLedManagement(x, 520, 1023, PIN_LED_RIGHT);
  directionLedManagement(y, 0, 480, PIN_LED_UP, true);
  directionLedManagement(y, 520, 1023, PIN_LED_DOWN);
  
  // led with pull up resistor (always positive until the button is pressed)
  digitalWrite(PIN_LED_BUTTON, !sw);
}
