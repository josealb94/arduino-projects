#include <DHT.h>
#include <DHT_U.h>

#define PIN_SENSOR 10

int temperature;
int humidity;

DHT dht (PIN_SENSOR, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("Listo!!!");
}

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" | Humedad: ");
  Serial.println(humidity);

  delay(500);
}
