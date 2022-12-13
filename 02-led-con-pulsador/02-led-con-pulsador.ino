#include <SoftwareSerial.h>

// Habilitar pines para comunicacion con modulo BT
// RX Pin10
// TX Pin11
SoftwareSerial miBT(10,11);

void setup() {
  Serial.begin(9600);
  Serial.println("Listo");
  miBT.begin(38400); // velocidad por defecto del modulo de BT
}

void loop() {
  if (miBT.available()) // cuando hay informacion disponible desde el modulo BT [lee BT y envia a Arduino]
    Serial.write(miBT.read());

  if (Serial.available()) // cuando hay informacion disponible en el monitor serial [lee Arduino y envia a BT]
    miBT.write(Serial.read());
}
