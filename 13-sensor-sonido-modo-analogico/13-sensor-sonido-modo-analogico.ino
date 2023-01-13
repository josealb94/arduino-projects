#define PIN_LED 10
#define PIN_SOUND_SENSOR_DIGITAL 11 // MIC
#define PIN_SOUND_SENSOR_ANALOG A0 // MIC

bool IS_DIGITAL = false; // true => digital | false => analog
int PIN_MIC;
int estado;

void digitalMode() {
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

void analogMode() {
  int valor = analogRead(PIN_MIC);
  Serial.print("valor: ");
  Serial.println(valor);
  delay(500);
}

void setup() {
  Serial.begin(9600);
  if (IS_DIGITAL) {
    PIN_MIC = PIN_SOUND_SENSOR_DIGITAL;
    pinMode(PIN_MIC, INPUT);
    pinMode(PIN_LED, OUTPUT);
  } else {
    PIN_MIC = PIN_SOUND_SENSOR_ANALOG;
  }
}

void loop() {
  if (IS_DIGITAL) {
    digitalMode();
  } else {
    analogMode();
  }
}
