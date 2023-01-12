#define PIN_RELE 10

void setup() {
  pinMode(PIN_RELE, OUTPUT);
}

void loop() {
  digitalWrite(PIN_RELE, LOW); // turn on relé
  delay(5000);
  digitalWrite(PIN_RELE, HIGH); // turn on relé
  delay(5000);
}
