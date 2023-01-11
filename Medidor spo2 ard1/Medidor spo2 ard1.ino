//Este programa contempla para mostrar en pantalla, medir temperatura, y tener el pulsador para toma de mediciones

/*
 * A4 SDA
 * A5 SLC
 * 
 */


#include <Wire.h>                 //librería para I2C pantalla OLED
#include <Adafruit_GFX.h>         //librería gráfica para la pantalla
#include <Adafruit_SSD1306.h>     //librería principal para la pantalla
#include <SoftwareSerial.h>       // librería para el módulo bluetooth

#define ANCHO_PANTALLA 128        // Ancho de la pantalla OLED
#define ALTO_PANTALLA 64          // Alto de la pantalla OLED

#define OLED_RESET     -1         // Pin reset incluido en algunos modelos de pantallas (-1 si no disponemos de pulsador). 
#define DIRECCION_PANTALLA 0x3C   //Dirección de comunicacion: 0x3D para 128x64, 0x3C para 128x32
#define logo1_alto  40
#define logo1_ancho 44
#define logo2_alto  18
#define logo2_ancho 44
#define intntc      A1             // medición de temperatura en la entrada A1
#define pinspo2     12            // Definifir entrada de lectura pwm para leer datos de spo2 entregado del arduino 2
#define bat         A2            // Entrada analógica para medición del voltaje de batería y para referencia para vm del divisor de voltaje del ntc
#define boton       7             // Entrada de pulsador para lectura de datos temperatura y SPO2
#define envio       8             //Envía un bit para al segundo arduino para que lea los SPO2
#define rec         9             //Recepta el bit del segundo arduino para leer el dato del  SPO2


int estadosalida=0, salidaled, estadoled, estadoenvio=0;
int contador;
int spo2;
String strspo2;                   //variable para pasar de spo2 int a string
String strtempt;                  //varaible para pasar de tempt float a string

Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, OLED_RESET); //inicialización de pantalla
SoftwareSerial miBT (10,11);     // Condiguración del módulo pin 10 RX , pin 11 TX

//logo corazon
const unsigned char PROGMEM logo1 [] = {
  0x00, 0x3e, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0xff, 0xc0, 0x3f, 0xf0, 0x00, 0x03, 0xff, 0xf0, 0xff, 
  0xfc, 0x00, 0x07, 0xff, 0xf9, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x0f, 0xff, 
  0xff, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x80, 
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xfc, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xfc, 0xff, 
  0xff, 0xc0, 0x3f, 0xff, 0xfc, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xf8, 0x7f, 0xff, 0xc0, 0x3f, 0xff, 
  0x78, 0x7f, 0xff, 0xc0, 0x1f, 0xfe, 0x78, 0x7f, 0xff, 0x80, 0x1f, 0xfe, 0x70, 0x7f, 0xff, 0x80, 
  0x1f, 0xfc, 0x30, 0x7f, 0xff, 0x80, 0x0f, 0xfc, 0x32, 0x3f, 0xff, 0x00, 0x0f, 0xf8, 0x22, 0x3f, 
  0xff, 0x00, 0xff, 0xf9, 0x83, 0x3f, 0xfe, 0x00, 0x00, 0x01, 0x87, 0x38, 0x00, 0x00, 0x00, 0x03, 
  0x87, 0x30, 0x00, 0x00, 0x00, 0xff, 0xcf, 0x13, 0xff, 0xf0, 0x00, 0xff, 0xcf, 0x87, 0xff, 0xf0, 
  0x00, 0x7f, 0xff, 0x87, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0x8f, 0xc0, 0x00, 0x00, 0x1f, 0xff, 0x9f, 
  0x80, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 
  0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
//logo termometro
const unsigned char PROGMEM logo2 [] = {
  0x07, 0xf0, 0x00, 0x0c, 0x18, 0x00, 0x18, 0x08, 0x00, 0x1b, 0xe8, 0x00, 0x13, 0xe8, 0x00, 0x10, 
  0xe8, 0x00, 0x13, 0xe8, 0x00, 0x13, 0xe8, 0x00, 0x10, 0xe8, 0x00, 0x13, 0xe8, 0x00, 0x11, 0xe8, 
  0x00, 0x10, 0xe8, 0x00, 0x13, 0x08, 0x00, 0x17, 0xe8, 0x00, 0x17, 0xe8, 0x00, 0x17, 0xe8, 0x00, 
  0x17, 0xe8, 0x00, 0x17, 0xe8, 0x00, 0x17, 0xe8, 0x00, 0x17, 0xe8, 0x00, 0x17, 0xe8, 0x00, 0x17, 
  0xe8, 0x00, 0x17, 0xe8, 0x00, 0x17, 0xe8, 0x00, 0x17, 0xe8, 0x00, 0x17, 0xe8, 0x00, 0x17, 0xe8, 
  0x00, 0x17, 0xe8, 0x00, 0x17, 0xe8, 0x00, 0x17, 0xe8, 0x00, 0x37, 0xec, 0x00, 0x67, 0xf6, 0x00, 
  0x4f, 0xfb, 0x00, 0xdf, 0xfd, 0x00, 0xdf, 0xdd, 0x00, 0xbf, 0x8d, 0x80, 0xbf, 0xfd, 0x80, 0xbf, 
  0xfd, 0x80, 0xdf, 0xfd, 0x00, 0x5f, 0xfb, 0x00, 0x6f, 0xf2, 0x00, 0x33, 0xe4, 0x00, 0x1c, 0x18, 
  0x00, 0x07, 0xf0, 0x00
};

// variables para medición de temperatura
float rAux = 9830.0;                    // resistencia para el divisor
float vcc = 0.0;                        // Voltaje de alimentación del divisor de tensión posteriormente calculada
float beta = 3380;                      // Constante del NTC
float temp0 = 298.15;                   // temperatura en Kelvin de 10k
float r0 = 10000.0;                     //Resistencia del NTC a 25 °C
//Variables usadas en el cálculo
float vm = 0.0;                         // Voltaje en el NTC
float rntc = 0.0;                       //Resistencia del NTC tiempo real cambio de temperatura
float temperaturaK = 0.0;               //Temperatura calculada en Kelvin                      
float tempt = 0.0;                      //Temperatura promedio de 15 mediciones

// inicio de configuración
void setup(){ 

  Wire.begin();                   // Inicializar comunicación I2C
  //inicialización de la comunicación de la pantalla
  if(!display.begin(SSD1306_SWITCHCAPVCC, DIRECCION_PANTALLA)) {      
    Serial.println(F("Fallo en la asignacion de SSD1306"));
  }
  display.clearDisplay();             //limpia pantalla
  miBT.begin (38400);                // Configuración de la velocidad de Bluetooth                           
  Serial.begin(9600);                // Puerto serial configurado a 9600 Baudios
  pinMode(boton,INPUT);               //declarar el pin de boton como entrada
  pinMode(rec,INPUT);                 //declarar el pin de rec como entrada
  pinMode(pinspo2,INPUT);             //declarar el pin de pinspo2 como entrada
  pinMode(envio,OUTPUT);              //declarar el pin de envio como salida
  
  digitalWrite(envio,LOW);            //se inicializa el pin envio en 0

    //inicio de pantalla para impresión
  display.clearDisplay();         //limpia la pantalla
  // dibujar logo corazon y temperatura
  display.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
  display.drawBitmap(0, 18, logo1,logo1_ancho, logo1_alto, WHITE);
  display.setCursor (46, 32);    // ubica cursor en coordenas 46,32
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print("0");           // escribe valor de los latidos
  display.setCursor (45, 45);    // ubica cursor en coordenas 45,45
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print("%");      // escribe texto
  
  // Dibujar logo termómetro y temperatura
  display.drawBitmap(75, 13, logo2,logo2_alto, logo2_ancho, WHITE);
  display.setCursor (92, 32);    // ubica cursor en coordenas 92,32
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print("0.0");    // escribe el valor de la medición de temperatura
  display.setCursor (100, 45);    // ubica cursor en coordenas 10,30
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print(" C");      // escribe texto
  display.display();     // muestra en pantalla todo lo establecido anteriormente


}

void loop(){
  int porc= 0;                           // porcentaje de batería
  int il=0;                              // porcentaje del ícono de la batería
 //int pulso = analogRead(pulsePin);      // Lee el valor del pulsometro conectado al puerto Analogo A0
  int medbat= analogRead(bat);           // lee el valor digital de la batería
  int spo2=0;
  int estado=digitalRead(boton);

// condicion para antirebotes del pulsador y leer el botón de lectura
if (estado==1)
  {estadosalida=1;}
if (estadosalida==1 && estado==LOW)               //condición de enviar dato cuando se suelta el botón
  {
  //salidaled=!salidaled;
  //digitalWrite(led,salidaled);
  estadosalida=0;                                 //pone el estadosalida en 0
  estadoenvio=1;                                  //se confirma que el botón se ha presionado y se ha soltado
  }  
delay (50);


//una vez pulsado el botón, envía señal al arduino 2 para la lectura y envío de la información
if (estadoenvio ==1)
{
  digitalWrite(envio,HIGH);     //envía un bit al arduino 2 para que lea los spo2
  delay (50);
  digitalWrite(envio,LOW);      //se pone en estado 
  estadoenvio=0;                //se reinicia el estado de envío para una próximo proceso, si fuera necesario por el usuario

  //pantalla que indica que está midiendo 
  display.clearDisplay();
  display.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
  display.setCursor (1, 1);    // ubica cursor en coordenas 46,32
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print("Midiendo...");      // escribe texto
  display.drawBitmap(0, 18, logo1,logo1_ancho, logo1_alto, WHITE);
  display.setCursor (46, 32);    // ubica cursor en coordenas 46,32
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print(spo2);           // escribe valor de los latidos
  display.setCursor (45, 45);    // ubica cursor en coordenas 45,45
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print("%");      // escribe texto
  
  // Dibujar logo termómetro y temperatura
  display.drawBitmap(75, 13, logo2,logo2_alto, logo2_ancho, WHITE);
  display.setCursor (92, 32);    // ubica cursor en coordenas 92,32
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print(tempt);    // escribe el valor de la medición de temperatura
  display.setCursor (100, 45);    // ubica cursor en coordenas 10,30
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print(" C");      // escribe texto
  display.display();     // muestra en pantalla todo lo establecido anteriormente
}

//lee el bit del arduino 2 que ya realizó la lectura y envío el bit mencionado y el dato spo2
if (digitalRead(rec)==HIGH)
{
  // "map" es para realizar una escala de valores leidos a 0-99, pulseIN es una función para leer (en milisegundos) el flanco positivo del PWM
  spo2=map(pulseIn(pinspo2, HIGH),0,2020,0,99);         // lee el dato de spo2 en pwm del arduino 2 ,y  hace la converción del rango de 0 a 99 spo2
  tempt = 0;
  for (int i = 0;i < 15; i++){

  vcc=(5.0 / 1024)*( analogRead(bat));              // Calcular el voltaje de la batería.
  vm=(5.0 / 1024)*( analogRead(intntc) );           //Calcular tensión en la entrada del divisor de voltaje del ntc
  rntc = rAux / ((vcc/vm)-1);                       //Calcular la resistencia de la NTC
  temperaturaK = beta/(log(rntc/r0)+(beta/temp0));  //Calcular la temperatura en Kelvin , ecuación logaritmica de un NTC
  tempt = tempt + temperaturaK -273.15 +7;          //Calcular la temperatura en °C, considerar agregar un valor para calibrar debido a la tolerancia de las resistencias y exactitud del ntc, en este caso se sumó 7
  }
  tempt = tempt/15;
  porc = (100*(medbat-737))/157;          // Calula el porcentaje de la batería, valor máximo es 894 (carga completa batería) y 737 para el valor 3.6 Voltios; 1024 es el valor de 5 voltios
  il = (porc * 8)/100;                               // cálculo de animación ícono batería
  if (il>8){
    il=8;
  }
  
  //mostrar datos en pantalla
  display.clearDisplay();

  // dibujar logo corazon y BPM
  display.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
  display.setCursor (1, 1);    // ubica cursor en coordenas 46,32
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print("RESULTADOS!!!");      // escribe texto
  display.drawBitmap(0, 18, logo1,logo1_ancho, logo1_alto, WHITE);
  display.setCursor (46, 32);    // ubica cursor en coordenas 46,32
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print(spo2);           // escribe valor de los latidos
  display.setCursor (45, 45);    // ubica cursor en coordenas 45,45
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print("%");      // escribe texto
  
  // Dibujar logo termómetro y temperatura
  display.drawBitmap(75, 13, logo2,logo2_alto, logo2_ancho, WHITE);
  display.setCursor (92, 32);    // ubica cursor en coordenas 92,32
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print(tempt);    // escribe el valor de la medición de temperatura
  display.setCursor (100, 45);    // ubica cursor en coordenas 10,30
  display.setTextSize(1);      // establece tamano de texto en 1
  display.print(" C");      // escribe texto
  display.display();     // muestra en pantalla todo lo establecido anteriormente
  /*
  
   Serial.print("analo :");
  Serial.println(analogRead(intntc));
  Serial.print("temperatura :");
  Serial.println(tempt);
  */

    strspo2+=spo2;      //conversión de integer a string
    strtempt+=tempt;    //conversión de float a string

  //Envío de datos por vía bluetooth
  //miBT.print("dato de oximetro "+String(spo2));      //envío de spo2 y temperatura (variable de latidos) al módulo bluetooth

  miBT.print("spo2:"+String(spo2)+",temp:"+String(tempt)); //envío de spo2 y temperatura en un solo envío en tipo de dato string

  //"spo2\":\"95\",\"temp\":\"36.5\"}"
  delay(100);           // tiempo de espera 500 ms
  //miBT.write(tempt);    //otra forma de enviar datos pero no funciona con string

    
}

}



  


  