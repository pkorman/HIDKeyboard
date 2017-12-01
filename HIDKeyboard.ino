
/*
  IN PROGRESS

  inspiration:
  https://www.sparkfun.com/news/2412
  https://github.com/awende/Cherry_MX_Keyboard

  other keyboard inspiration:
  https://iamzxlee.wordpress.com/2013/07/24/4x4-matrix-keypad/
  https://arduino.stackexchange.com/questions/36775/analog-pins-for-4x4-matrix-keypad
  https://playground.arduino.cc/Code/OneWireKeyPad
  https://en.wikipedia.org/wiki/Resistor_ladder#R-2R_resistor_ladder_network
  http://forum.arduino.cc/index.php?topic=8558.0
  http://www.instructables.com/id/Connecting-a-4-x-4-Membrane-Keypad-to-an-Arduino/

*/

#include <Keypad.h>
#include <Encoder.h>
//Bounce2 https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

#define HEARTBEAT_PIN 13

#define ENCODER_L0  6
#define ENCODER_L1  7

#define ENCODER_R0  2
#define ENCODER_R1  3

#define BUTTON_ENCODER_L_SW  8
#define BUTTON_ENCODER_R_SW  4

Bounce encoderRightSW = Bounce();
Bounce encoderLeftSW = Bounce();

Encoder knobLeft(ENCODER_L0, ENCODER_L1);
Encoder knobRight(ENCODER_R1, ENCODER_R0);

byte rowPin[4] = {9, 10, 16, 14}; //banggood Arduino mini Leonardo
byte colPin[4] = {A0, A1, A2, A3};

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPin, colPin, 4, 4);

long heartbeatTime;

void setup()
{
  Serial.begin(9600);

  pinMode(BUTTON_ENCODER_R_SW, INPUT_PULLUP);
  encoderRightSW.attach(BUTTON_ENCODER_R_SW);
  encoderRightSW.interval(5); // interval in ms

  pinMode(BUTTON_ENCODER_L_SW, INPUT_PULLUP);
  encoderLeftSW.attach(BUTTON_ENCODER_L_SW);
  encoderLeftSW.interval(5); // interval in ms

  // initialize digital pin 13 as an output. heartbeat HEARTBEAT_PIN
  pinMode(HEARTBEAT_PIN, OUTPUT);
}

void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.println(key);
  }

  // Read Encoder Switches using Bounce2 lib
  if (encoderRightSW.read() == LOW) {
    Serial.println("encoderRightSW");
  }
  if (encoderLeftSW.read() == LOW) {
    Serial.println("encoderLeftSW");
  }

  // Read Encoders
  char leftValue = 0, rightValue = 0;
  leftValue = knobLeft.read();
  rightValue = knobRight.read();

  // Left Encoder
  if (leftValue > 3)
  {
    Serial.println("Left Encoder Up");
    knobLeft.write(0);
  }
  else if (leftValue < -3)
  {
    Serial.println("Left Encoder Down");
    knobLeft.write(0);
  }

  // Right Encoder
  if (rightValue > 3)
  {
    Serial.println("Right Encoder Up");
    knobRight.write(0);
  }
  else if (rightValue < -3)
  {
    Serial.println("Right Encoder Down");
    knobRight.write(0);
  }

  if (millis() > heartbeatTime + 125) {
    heartbeatTime = millis();
    digitalWrite(HEARTBEAT_PIN, !digitalRead(HEARTBEAT_PIN));
  }

}
