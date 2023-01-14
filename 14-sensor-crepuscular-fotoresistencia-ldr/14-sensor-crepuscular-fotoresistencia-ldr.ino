#define PIN_LED 10
#define PIN_LDR A0

void turnOnOrOff() {
  int valor = analogRead(PIN_LDR);

  if (valor < 200) {
    digitalWrite(PIN_LED, HIGH);
    delay(1000);
  }
  digitalWrite(PIN_LED, LOW);
}

void ledPWM() {
  // it is necessary that the led is connected to a pwm pin
  int valor = analogRead(PIN_LDR);

  // if we have low level in the LDR we must map to a high
  // value for the led
  int pwm = map(valor, 0, 1023, 255, 0);

  analogWrite(PIN_LED, pwm);
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  ledPWM();
}
