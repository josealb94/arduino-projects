void setup() {
  // le indicamos si el pin es de entrada o salida
  pinMode(13, OUTPUT); // pin13 como salida
}

void loop() {
  // encendemos LED
  digitalWrite(13, HIGH); // enviamos 5V al pin13
  
  // agregamos un retraso
  delay(1000); // pausa el programa 1s

  // apagamos LED
  digitalWrite(13, LOW); // enviamos 0V al pin13

  // agregamos un retraso
  delay(1000); // pausa el programa 1s
}
