// define the constants
#define POTENTIOMETER A0
#define LED 11

// intensity can take a value from 0 to 255, which is the range of values of a PWM output.
int intensity = 0;

void setup() {
  Serial.begin(9600);                   // initialize serial monitor
  
  // no need to declare pin (POTENTIOMETER) as input since Analog In is used
  pinMode(LED, OUTPUT);                 // declare pin as output

  Serial.println("Listo!!");            // indicate that everything was loaded successfully
}

void loop() {
  int intensity = analogRead(POTENTIOMETER) / 4; // 1023 / 4 = 255,75 => 255
  analogWrite(LED, intensity);
}
