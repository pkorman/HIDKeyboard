
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

  Resources:
  HID-Project.h (https://github.com/NicoHood/HID)
  Encoder.h (https://www.pjrc.com/teensy/td_libs_Encoder.html)
*/

#include <Keypad.h> //
#include <Encoder.h> //https://github.com/PaulStoffregen/Encoder
#include <Bounce2.h> //Bounce2 https://github.com/thomasfredericks/Bounce2
#include <HID-Project.h>
#include <HID-Settings.h>

#define ENCODER_L0  7
#define ENCODER_L1  6

#define ENCODER_R0  3
#define ENCODER_R1  2

#define BUTTON_ENCODER_L_SW  5
#define BUTTON_ENCODER_R_SW  4

Bounce encoderRightSW = Bounce();
Bounce encoderLeftSW = Bounce();

Encoder knobLeft(ENCODER_L0, ENCODER_L1);
Encoder knobRight(ENCODER_R1, ENCODER_R0);

byte rowPin[4] = {8, 14, 16, 10}; //banggood Arduino mini Leonardo
byte colPin[4] = {A2, A1, A0, 15};

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPin, colPin, 4, 4);

long play_pause_time;
long marker_time;

boolean rewind_active = false;
boolean fast_forward_active = false;

void setup()
{
  Serial.begin(9600);

  pinMode(BUTTON_ENCODER_R_SW, INPUT_PULLUP);
  encoderRightSW.attach(BUTTON_ENCODER_R_SW);
  encoderRightSW.interval(10); // interval in ms

  pinMode(BUTTON_ENCODER_L_SW, INPUT_PULLUP);
  encoderLeftSW.attach(BUTTON_ENCODER_L_SW);
  encoderLeftSW.interval(10); // interval in ms

  Consumer.begin();
  Keyboard.begin();
}

void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.println(key);

    //keyboard row1
    if (key == '1') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.write(KEY_LEFT_ARROW);
      delay(100);
      Keyboard.releaseAll();
    }
    if (key == '2') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.write(KEY_RIGHT_ARROW);
      delay(100);
      Keyboard.releaseAll();
    }
    if (key == '3') {
      Keyboard.print("-");
    }
    if (key == 'A') {
      Keyboard.print("=");
    }

    //keyboard row2
    if (key == '4') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.print("j");
      delay(100);
      Keyboard.releaseAll();
    }
    if (key == '5') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.print("k");
      delay(100);
      Keyboard.releaseAll();
    }
    if (key == '6') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.print("l");
      delay(100);
      Keyboard.releaseAll();
    }
    if (key == 'B') {
      Keyboard.write(KEY_DELETE);
    }

    //keyboard row3
    if (key == '7') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.print("g");
      delay(100);
      Keyboard.releaseAll();
    }
    if (key == '8') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.print("i");
      delay(100);
      Keyboard.releaseAll();
    }
    if (key == '9') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.print("t");
      delay(100);
      Keyboard.releaseAll();
    }
    if (key == 'C') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.print("y");
      delay(100);
      Keyboard.releaseAll();
    }

    //keyboard row4
    if (key == '*') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.print("s");
      delay(100);
      Keyboard.releaseAll();
    }
    if (key == '0') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.print("c");
      delay(100);
      Keyboard.releaseAll();
    }
    if (key == '#') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.print("v");
      delay(100);
      Keyboard.releaseAll();
    }
    if (key == 'D') {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.print("z");
      delay(100);
      Keyboard.releaseAll();
    }

  }

  // Read Encoder Switches using Bounce2 lib
  encoderLeftSW.update();
  if ((encoderLeftSW.read() == LOW) && (millis() > marker_time + 333)) {
    Serial.println("encoderLeftSW");

    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.print("m");
    delay(100);
    Keyboard.releaseAll();
    marker_time = millis();
  }
  
  encoderRightSW.update();
  if ((encoderRightSW.read() == LOW) && (millis() > play_pause_time + 333)) {
    Serial.println("encoderRightSW");
    Keyboard.write(KEY_UP_ARROW);
    play_pause_time = millis();    
  }


  // Read Encoders
  char leftValue = 0, rightValue = 0;
  leftValue = knobLeft.read();
  rightValue = knobRight.read();

  // Left Encoder
  if (leftValue > 1)
  {
    Serial.println("Left Encoder Up");
    knobLeft.write(0);
    Keyboard.write(KEY_RIGHT_ARROW);
  }
  else if (leftValue < -1)
  {
    Serial.println("Left Encoder Down");
    knobLeft.write(0);
    Keyboard.write(KEY_LEFT_ARROW);
  }

  // Right Encoder
  if (rightValue > 1)
  {
    Serial.println("Right Encoder Up");
    knobRight.write(0);
    if (rewind_active) {
      Keyboard.write(KEY_RIGHT_ARROW);
      fast_forward_active = false;
      rewind_active = false;
    } else {
      Keyboard.print("l");
      fast_forward_active = true;      
    }
  }
  else if (rightValue < -1)
  {
    Serial.println("Right Encoder Down");
    knobRight.write(0);
    if (fast_forward_active) {
      Keyboard.write(KEY_LEFT_ARROW);
      fast_forward_active = false;
      rewind_active = false;
    } else {
      Keyboard.print("j");
      rewind_active = true;      
    }
  }

}
