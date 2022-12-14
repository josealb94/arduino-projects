#include <SoftwareSerial.h>

// AndroidApk: Bluetooth Serial Controller => El app permite
// enviar datos desde el telefono para probar la conexion BT

// Habilitar pines para comunicacion con modulo BT
// RX Pin10
// TX Pin11
SoftwareSerial miBT(10,11);

String lecturaStr;
int LED_ROJO = 2;
int LED_VERDE = 3; // Pin PWM (Regulable la salida de voltaje)
int BRILLO = 0; // 0-255

void setup() {
  miBT.begin(38400); // velocidad por defecto del modulo de BT
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
}

void loop() {
  if (miBT.available()) {
    // cuando hay informacion disponible desde el modulo BT [lee BT y envia a Arduino]
    lecturaStr = miBT.readString();
    if (lecturaStr == "1")
      digitalWrite(LED_ROJO, !digitalRead(LED_ROJO));
    
    if (lecturaStr == "2") {
      BRILLO += 10;
      if (BRILLO > 255)
        BRILLO = 255;
      analogWrite(LED_VERDE, BRILLO);
    }

    if (lecturaStr == "3") {
      BRILLO -= 10;
      if (BRILLO < 0)
        BRILLO = 0;
      analogWrite(LED_VERDE, BRILLO);
    }

    if (lecturaStr == "Hola") {
      digitalWrite(LED_ROJO, HIGH);
      digitalWrite(LED_VERDE, HIGH);
    }

    if (lecturaStr == "Adios") {
      digitalWrite(LED_ROJO, LOW);
      digitalWrite(LED_VERDE, LOW);
    }
  }
}
