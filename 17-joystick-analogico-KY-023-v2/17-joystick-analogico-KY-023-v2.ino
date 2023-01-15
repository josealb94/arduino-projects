#define PIN_LED_LEFT 3
#define PIN_LED_UP 5
#define PIN_LED_RIGHT 6
#define PIN_LED_DOWN 9
#define PIN_LED_BUTTON 11
#define PIN_JOYSTICK_VRX A1
#define PIN_JOYSTICK_VRY A0
#define PIN_JOYSTICK_SW 10
#define CENTER_POINT 500
#define SHOULD_LOG true
// int restAreaXMin = CENTER_POINT - 20;
// int restAreaXMax = CENTER_POINT + 20;
// int restAreaYMin = CENTER_POINT - 20;
// int restAreaYMax = CENTER_POINT + 20;

// void isRestArea(x, y) {
//   return x <= restAreaXMax &&
//           x >= restAreaXMin &&
//           y <= restAreaYMax &&
//           y >= restAreaYMin
// }

void showStatus(int x, int y, int sw) {
  Serial.print("[");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print("] (");
  Serial.print(sw);
  Serial.println(")");
}

enum AxisEnum { XPositive, XNegative, YPositive, YNegative };

class AxisRange {
private:
    int min;
    int max;
 
public:
    // Constructor
    AxisRange(int min, int max)
    {
        // this pointer
        this->min = min;
        this->max = max;
    }

    bool getMin() { return this->min; }
    bool getMax() { return this->max; }
};

class JoystickData {
private:
    int x_positive;
    int x_negative;
    int y_positive;
    int y_negative;
    int sw;
 
public:
    // Constructor
    JoystickData(int x_positive, int x_negative, int y_positive, int y_negative, int sw)
    {
        this->x_positive = x_positive;
        this->x_negative = x_negative;
        this->y_positive = y_positive;
        this->y_negative = y_negative;
        this->sw = sw;
    }

    int getXPositive() { return this->x_positive; }
    int getXNegative() { return this->x_negative; }
    int getYPositive() { return this->y_positive; }
    int getYNegative() { return this->y_negative; }
    int getSW() { return this->sw; }
};

class JoystickRestArea {
private:
    int x_min;
    int x_max;
    int y_min;
    int y_max;
 
public:
    // Constructor
    JoystickRestArea(int x_min, int x_max, int y_min, int y_max)
    {
        // this pointer
        this->x_min = x_min;
        this->x_max = x_max;
        this->y_min = y_min;
        this->y_max = y_max;
    }

    bool isRestArea(int x, int y) {
      return x <= this->x_max &&
              x >= this->x_min &&
              y <= this->y_max &&
              y >= this->y_min;
    }

    bool validateAxisMovement(int value, AxisEnum axis, AxisRange axisRange) {
      // return (
      //     (axis == XNegative || axis == YNegative) &&
      //     value >= axisRange.getMin() &&
      //     value < axisRange.getMax()
      //   ) ||
      //   (
      //     (axis == XPositive || axis == YPositive) &&
      //     value > axisRange.getMin() &&
      //     value <= axisRange.getMax()
      //   );
      if ((axis == XNegative || axis == YNegative) && value >= axisRange.getMin() && value < axisRange.getMax()) {
        return true;
      }
      if ((axis == XPositive || axis == YPositive) && value > axisRange.getMin() && value <= axisRange.getMax()) {
        return true;
      }
      return false;
    }

    AxisRange getAxisRange(AxisEnum axis) {
      // 0 - 480 | 520 - 1023
      switch (axis)
      {
      case XPositive: case YPositive:
          return AxisRange(this->x_max, 1023);
      case XNegative: case YNegative:
          return AxisRange(0, this->x_min);
      default:
          return AxisRange(this->x_max, 1023);
      }      
    }
};

class Joystick {
private:
    int pin_vrx;
    int pin_vry;
    int pin_sw;
    JoystickRestArea *joystickRestArea;

    int mapValuesToPwmOutput(int value, AxisEnum axis) {
      AxisRange axisRange = joystickRestArea->getAxisRange(axis);
      // if (SHOULD_LOG) {
      //   Serial.println("***** RANGOS *****");
      //   Serial.print("axis: ");
      //   Serial.println(axis);
      //   Serial.print("axisRange: ");
      //   Serial.print("* axisRange min: ");
      //   Serial.println(axisRange.getMin());
      //   Serial.print("* axisRange max: ");
      //   Serial.println(axisRange.getMax());
      //   Serial.print("value: ");
      //   Serial.println(value);
      //   Serial.println("******************");
      // }
      if (!joystickRestArea->validateAxisMovement(value, axis, axisRange)) return 0;
      int minimumForMapper = 0;
      int maxForMapper = 255;
      if (axis == XNegative || axis == YNegative) {
        minimumForMapper = 255;
        maxForMapper = 0;
      }
      int mapedValue = map(value, axisRange.getMin(), axisRange.getMax(), minimumForMapper, maxForMapper);
      // if (SHOULD_LOG) {
      //   Serial.print("Maped Value: ");
      //   Serial.println(mapedValue);
      // }
      return mapedValue;
    }
 
public:
    // Constructor
    Joystick(JoystickRestArea *joystickRestArea, int pin_vrx, int pin_vry, int pin_sw)
    {
        this->joystickRestArea = joystickRestArea;
        this->pin_vrx = pin_vrx;
        this->pin_vry = pin_vry;
        this->pin_sw = pin_sw;
    }

    JoystickData readJoystick() {
      int x = analogRead(this->pin_vrx);
      int y = analogRead(this->pin_vry);
      int sw = !digitalRead(this->pin_sw);

      // if (SHOULD_LOG) showStatus(x, y, sw);
      // if (this->joystickRestArea->isRestArea(x, y))
      //   return JoystickData(0, 0, 0, 0, sw); // rest area values
  
      // returns the value of the joystick for all axes   
      // int x_positive, int x_negative, int y_positive, int y_negative, int sw   
      return JoystickData(
        mapValuesToPwmOutput(x, XPositive),
        mapValuesToPwmOutput(x, XNegative),
        mapValuesToPwmOutput(y, YPositive),
        mapValuesToPwmOutput(y, YNegative),
        sw
      );
    }
};

JoystickRestArea joystickRestArea(480, 520, 480, 520);
Joystick joystick(&joystickRestArea, PIN_JOYSTICK_VRX, PIN_JOYSTICK_VRY, PIN_JOYSTICK_SW);

void directionLedManagement(int value, int min, int max, int pinLed, bool isInvertedValue = false) {
  int minimumForMapper = 0;
  int maxForMapper = 255;
  if (isInvertedValue) {
    minimumForMapper = 255;
    maxForMapper = 0;
    min = min - 1;
  } else {
    max = max + 1;
  }
  if (value > min && value < max) {
    analogWrite(pinLed, map(value, min, max, minimumForMapper, maxForMapper));
  } else {
    analogWrite(pinLed, 0);
  }
}

void setup() {
  Serial.begin(9600);

  // config output pins
  pinMode(PIN_LED_LEFT, OUTPUT);
  pinMode(PIN_LED_UP, OUTPUT);
  pinMode(PIN_LED_RIGHT, OUTPUT);
  pinMode(PIN_LED_DOWN, OUTPUT);
  pinMode(PIN_LED_BUTTON, OUTPUT);

  // config input pins
  // pinMode(PIN_JOYSTICK_VRX, INPUT); // inputs of type 'AnalogIn' do not need to be declared
  // pinMode(PIN_JOYSTICK_VRY, INPUT); // inputs of type 'AnalogIn' do not need to be declared
  pinMode(PIN_JOYSTICK_SW, INPUT);

  // Serial.println("listo!!");
}


void manualProcess() {
  // joystick reading
  int x = analogRead(PIN_JOYSTICK_VRX);
  int y = analogRead(PIN_JOYSTICK_VRY);
  int sw = digitalRead(PIN_JOYSTICK_SW);

  // if (SHOULD_LOG) {
  //   Serial.print("[");
  //   Serial.print(x);
  //   Serial.print(", ");
  //   Serial.print(y);
  //   Serial.println("]");
  // }

  directionLedManagement(x, 0, 480, PIN_LED_LEFT, true);
  directionLedManagement(x, 520, 1023, PIN_LED_RIGHT);
  directionLedManagement(y, 0, 480, PIN_LED_UP, true);
  directionLedManagement(y, 520, 1023, PIN_LED_DOWN);
  
  // led with pull up resistor (always positive until the button is pressed)
  digitalWrite(PIN_LED_BUTTON, !sw);
}

void processWithObjects() {
  JoystickData joystickData = joystick.readJoystick();

  // if (SHOULD_LOG) {
  //   Serial.println("*****"); // TODO delete
  //   Serial.print("x negative: "); // TODO delete
  //   Serial.println(joystickData.getXNegative()); // TODO delete
  //   Serial.print("x positive: ");
  //   Serial.println(joystickData.getXPositive());
  //   Serial.print("y negative: ");
  //   Serial.println(joystickData.getYNegative());
  //   Serial.print("y positive: ");
  //   Serial.println(joystickData.getYPositive());
  //   Serial.print("sw: ");
  //   Serial.println(joystickData.getSW());
  //   Serial.println("*****");
  // }
  
  analogWrite(PIN_LED_LEFT, joystickData.getXNegative());
  analogWrite(PIN_LED_RIGHT, joystickData.getXPositive());
  analogWrite(PIN_LED_DOWN, joystickData.getYPositive());
  analogWrite(PIN_LED_UP, joystickData.getYNegative());
  digitalWrite(PIN_LED_BUTTON, joystickData.getSW());

  Serial.println("#################################################################");
}

void loop() {
  // manualProcess();
  processWithObjects();
}
