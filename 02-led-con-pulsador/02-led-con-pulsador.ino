// define the constants
#define BUTTON 2
#define LED 3

void setup() {
  Serial.begin(9600);                   // initialize serial monitor
  
  pinMode(BUTTON, INPUT);               // declare pin as input
  pinMode(LED, OUTPUT);                 // declare pin as output

  Serial.println("Listo!!");            // indicate that everything was loaded successfully
}

void loop() {
  int state = digitalRead(BUTTON);      // read the state of the button

  // when the button was pressed we turn on the led for 1s, turn it off
  // for 1s, turn it on for 1s and turn it off until the button is
  // pressed again
  if (state == HIGH) {
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
  }
}