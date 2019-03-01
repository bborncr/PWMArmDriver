// Install the Adafruit_PWMServoDriver from the Arduino IDE Library Manager

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <avr/pgmspace.h>

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

String inputString = "";
String servoString = "";
String commandString = "";
boolean stringComplete = false;
boolean inputState = false;

long previousMillis;

void servoWrite(int servo, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servo, 0, pulse);
}

void setup() {
  Serial.begin(38400);

  pinMode(INPUT_PIN, INPUT_PULLUP);

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  inputString.reserve(10);
  servoString.reserve(10);
  commandString.reserve(10);

  for (int i = 0; i < 4; i++) {
    servoWrite(i, 90);
  }
  delay(100);

}

void loop() {
  parseCommand();
}

void finish() {
  while (true) {
    ;
    ;
  }
}
/*
  SerialEvent occurs whenever a new data comes in the
  hardware serial RX.  This routine is run between each
  time loop() runs, so using delay inside loop can delay
  response.  Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void parseCommand() {
  // print the string when a newline arrives:
  if (stringComplete) {
    inputString.trim();
    //Serial.println(inputString);
    int separatorIndex = inputString.indexOf(',');
    servoString = inputString.substring(0, 1);
    commandString = inputString.substring(separatorIndex + 1);
    int servoNum = servoString.toInt();
    int servoPos = commandString.toInt();

    servoWrite(servoNum, servoPos);
    // clear the string:
    inputString = "";
    servoString = "";
    commandString = "";
    stringComplete = false;
  }
}
