#define PIN_LED 10
#define PIN_SOUND_SENSOR_DIGITAL 11 // MIC
#define PIN_SOUND_SENSOR_ANALOG 11 // MIC

int PIN_MIC = 11;
// int valor;
int estado;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_MIC, INPUT);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  int valor = digitalRead(PIN_MIC);
  Serial.print("valor: ");
  Serial.println(valor);
  if (valor == HIGH) {
    Serial.println("entro");
    int estado = digitalRead(PIN_LED);
    digitalWrite(PIN_LED, !estado);
    delay(500);
  }
}
