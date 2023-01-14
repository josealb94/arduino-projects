#define PIN_LED 9
#define PIN_TRIG 11
#define PIN_ECHO 10
#define DISTANCE_CM 58.2 // defined by sensor manufacturer

int duracion;
int distancia;

void triggerSensor() {
  digitalWrite(PIN_TRIG, HIGH);
  delay(1); // defined by sensor manufacturer
  digitalWrite(PIN_TRIG, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  Serial.println("listo!!");
}

void loop() {
  triggerSensor();

  // pulseIn => returns a time value in microseconds
  int duration = pulseIn(PIN_ECHO, HIGH);
  int distance = duration / DISTANCE_CM;

  Serial.print("distancia: ");
  Serial.println(distance);

  delay(200);

  if (distance <= 20 && distance >= 0) {
    digitalWrite(PIN_LED, HIGH);
    delay(distance  * 10);
  }
  digitalWrite(PIN_LED, LOW);
}
