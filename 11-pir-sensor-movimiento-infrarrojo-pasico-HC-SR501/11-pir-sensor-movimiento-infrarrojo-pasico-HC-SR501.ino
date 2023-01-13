#define PIN_PIR 2
#define PIN_RELE 3

int estado = 0;

void setup() {
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_RELE, OUTPUT);

  // the sensor requires a few seconds to stabilize
  delay(2000);
}

void loop() {
  estado = digitalRead(PIN_PIR);
  if (estado == HIGH) {
    digitalWrite(PIN_RELE, LOW);
    
    // check that the relay does not change state in short periods of time to avoid problems with the relay
    delay(4000);
  } else {
    digitalWrite(PIN_RELE, HIGH);
  }
}
