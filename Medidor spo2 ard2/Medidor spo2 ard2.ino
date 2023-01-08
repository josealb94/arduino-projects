/* PROGRAMA PARA MEDIR LOS SPO2 Y MANDARLO AL ARDUINO 1 PARA QUE LO MUESTRE POR PANTALLA
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected
 
  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.
*/

#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

MAX30105 particleSensor;

#define MAX_BRIGHTNESS 255
#define senal      10             // Salida pwm para envíar al arduino 1
#define envio      11              //Envía un bit al arduino 1 para que lea los SPO2
#define rec        12             //Recepta el bit del arduino 1 para procesar los datos de spo2

int liminfe = 85;             //límite inferior para que se acepte la lectura
int limsup= 99;               //límite superior para que se acepte la lectura
int pl1=0;                    //es el indicativo de la primera lectura
int salidaanalogica=0;          //valor de la salida pwm correspondiente al valor de spo2 que va hacia el arduino1
int estadorec=0;              //estado de receptar el bit de solicitud para leer los spo2
int lecturaval=0;             //Estado de recibir una lectura válida de spo2.

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // de acuerdo al tipo de arduino y su controlador, se usará un dato de int 16 o 32
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif

int32_t bufferLength; // cantidad de muestreo
int32_t spo2; // variable para medir los spo2
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid


void setup()
{

  pinMode(envio,OUTPUT);              //declarar el pin de envío como salida
  pinMode(rec,INPUT);                 //declarar el pin de rec como entrada
  pinMode(senal,OUTPUT);              //declarar el pin de senal como salida
  digitalWrite(envio,LOW);            //se inicializa el pin envio en 0  
   
  Serial.begin(115200); // initialize serial communication at 115200 bits per second:

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

 
  byte ledBrightness = 60; //Options: 0=Off to 255=50mA intensidad de luz roja utilizada para la medición
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200 //frecuencia de muestreo
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
}

void loop()
{
  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps ; cantidad de muestreo para recopilar información
  
  int estado=digitalRead(rec);          //lee el bit enviado por el arduino 1, para comenzar a leer los spo2

if (estado==HIGH)
{

  //Ciclo de medición hasta obtener una lectura válida
  while (lecturaval==0){
    
    // verifica si es la primera lectura
    if (pl1==0){
    //lee las primeras 100 muestras
    for (byte i = 0 ; i < bufferLength ; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data; Para recopilar nueva información

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample; termina de recopilar información

      Serial.print(F("red="));
      Serial.print(redBuffer[i], DEC);
      Serial.print(F(", ir="));
      Serial.println(irBuffer[i], DEC);
    }
    pl1=1;                      //para que no haga la primera lectura
    }

    // con la información recopilada de ir e ir, calcula el spo2 y bmp
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    //condicion para enviar el spo2
    if (spo2<=limsup && spo2>=liminfe)
    {
        Serial.print(F(", SPO2="));
        Serial.print(spo2, DEC);
        Serial.print(F(", SPO2Valid="));
        Serial.println(validSPO2, DEC);
        salidaanalogica = map(spo2, 0, 99, 0, 254);     //reescalado de la lectura del spo2 para salida de pwm hacia arduino 1 
        analogWrite(senal, salidaanalogica);             //envío del dato spo2    
        lecturaval=1;                                   //condicion para salir del while de lectura
        digitalWrite(envio,HIGH);                       //Ha terminado de medir el spo2 y que enviará al arduino 1 para que se prepare para la lectura
        delay (50);                                     //retardo para mantener el bit de envío
        digitalWrite(envio,LOW);
    }
      //mueve los primeros 75 datos hacia al ultimo
      //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
      for (byte i = 25; i < 100; i++)
      {
        redBuffer[i - 25] = redBuffer[i];
        irBuffer[i - 25] = irBuffer[i];
      }

      //take 25 sets of samples before calculating the heart rate., mide nuevos 25 datos
      for (byte i = 75; i < 100; i++)
      {
        while (particleSensor.available() == false) //do we have new data?
          particleSensor.check(); //Check the sensor for new data; prepara al sensor para guardar mediciones

        redBuffer[i] = particleSensor.getRed();
        irBuffer[i] = particleSensor.getIR();
        particleSensor.nextSample(); //We're finished with this sample so move to next sample, termina de realizar mediciones
      }
  }
    lecturaval=0;                   //una vez obtenido una medición válida, se pone esta variable en 0 para una próximo ciclo
    pl1=0;                          //se inicializa otra vez pl1 para el muestreo de 100 para un próximo ciclo de medición por petición del usuario
}

  
}

