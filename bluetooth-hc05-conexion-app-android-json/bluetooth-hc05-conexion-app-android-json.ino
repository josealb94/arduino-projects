#include <SoftwareSerial.h>

// AndroidApk: Bluetooth Serial Controller => El app permite
// enviar datos desde el telefono para probar la conexion BT

// Docs
// [Concat float to string] (https://forum.arduino.cc/t/concatenate-float-w-a-string/143164/5)

const int LOWER_SPO2_MEASUREMENT = 9500;
const int HIGHER_SPO2_MEASUREMENT = 10000;
const int LOWER_TEMP_MEASUREMENT = 3550;
const int HIGHER_TEMP_MEASUREMENT = 3750;

// Habilitar pines para comunicacion con modulo BT
// RX Pin10
// TX Pin11
SoftwareSerial miBT(10,11);

// full message example
// {"spo2":"%s","temp":"%s"}
char* msgBase = "{\"spo2\":\"%s\",\"temp\":\"%s\"}";

float vSpO2;
float vTemp;
//float vSpO2 = 1000.98;
//float vTemp = 1234.12;

// with length 8 allows values like: 9876.98
// Note: 4 integers and 2 decimals
char msg[254], valueStr[8], sSpO2[9], sTemp[9];


void setup() {
  Serial.begin(9600);
  Serial.println("Listo");
  miBT.begin(38400); // bluetooth module default speed
}

void loop() {
  vSpO2 = generateRandomValues(LOWER_SPO2_MEASUREMENT, HIGHER_SPO2_MEASUREMENT);
  vTemp = generateRandomValues(LOWER_TEMP_MEASUREMENT, HIGHER_TEMP_MEASUREMENT);
  
  // parse values
  FtoChar2(vSpO2, sSpO2);
  FtoChar2(vTemp, sTemp);

  // build final message
  sprintf(msg, msgBase, sSpO2, sTemp);

  // send message to serial monitor
  Serial.println(msg);
  
  // send message to app
  miBT.write(msg, strlen(msg)); 

  // changing value
  vSpO2 = vSpO2 + 1;

  // delay 1 second
  delay(1000); // delay 1s
}

float generateRandomValues(float lowerMeasurenment, float higherMeasurenment) {
  long randNumber = random(lowerMeasurenment,higherMeasurenment);
  return (float) randNumber / 100.00;
  
}

void funciona1() {
  // parse values
  FtoChar2(vSpO2, sSpO2);

  // build final message
  sprintf(msg, "Hola amigos como estan: %s", sSpO2);
  
  // send message to serial monitor
  Serial.println(msg);
  
  // send message to app
  miBT.write(msg, strlen(msg)); 

  // delay 1 second
  delay(1000); // delay 1s
}

char* FtoChar(float fVal, int iDigs, char* cF) {
  long dSep = pow(10, iDigs);

  signed long slVal = fVal * dSep;
  sprintf(cF, "%d.%d", int(slVal / dSep), int(slVal % int(dSep)));
}

char* FtoChar2(float fVal, char* cF) {
  int iDigs = 2;
  long dSep = pow(10, iDigs);

  signed long slVal = fVal * dSep;
  sprintf(cF, "%d.%d", int(slVal / dSep), int(slVal % int(dSep)));
}
