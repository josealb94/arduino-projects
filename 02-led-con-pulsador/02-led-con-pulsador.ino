int PIN_PULSADOR = 2;
int PIN_LED = 3;
int ESTADO_LED = LOW;

void setup() {
  pinMode(PIN_PULSADOR, INPUT); // Pin2 como entrada, lectura del pulsador
  pinMode(PIN_LED, OUTPUT); // Pin3 como salida para LED
}

void loop() {
  while(digitalRead(PIN_PULSADOR) == LOW) {
    // no hacer nada cuando no esta pulsado
  }
  
  ESTADO_LED = digitalRead(PIN_LED); // leer estado actual del LED
  digitalWrite(PIN_LED, !ESTADO_LED); // cambiar el estado del LED

  while(digitalRead(PIN_PULSADOR) == HIGH) {
    // rutina antirebote ya que el pulsador no cambia de estado inmediatamente
  }
}
