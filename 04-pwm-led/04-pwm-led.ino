// define the constants
#define POTENTIOMETER 10
#define LED 11

// intensity can take a value from 0 to 255, which is the range of values of a PWM output.
int intensity = 0;

void setup() {
  Serial.begin(9600);                   // initialize serial monitor
  
  // pinMode(BUTTON, INPUT);               // declare pin as input
  pinMode(LED, OUTPUT);                 // declare pin as output

  Serial.println("Listo!!");            // indicate that everything was loaded successfully
}

void loop() {
  for (intensity = 0; intensity < 256; intensity++) {
    analogWrite(LED, intensity);
    delay(15);
  }
  for (intensity = 255; intensity > 0; intensity--) {
    analogWrite(LED, intensity);
    delay(15);
  }
}