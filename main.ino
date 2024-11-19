//***********************************************************************************************
//  This example shows how to use the basic methods of the FluxGarage Robo Eyes library. 
//
//  Hardware: You'll need a breadboard, an arduino nano r3, an I2C oled display with 1306   
//  or 1309 chip and some jumper wires.
//  
//  Published in September 2024 by Dennis Hoelscher, FluxGarage
//  www.youtube.com/@FluxGarage
//  www.fluxgarage.com
//
//***********************************************************************************************


#include <Adafruit_SSD1306.h>
#include <Wire.h>

// #include <SparkFun_TB6612.h>

// #define AIN1 D6
// #define AIN2 D1
// #define PWMA D8
// #define BIN1 D3
// #define BIN2 D4
// #define PWMB A0
// #define STBY 9

// const int offsetA = 1;
// const int offsetB = 1;

// Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);

// Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <FluxGarage_RoboEyes.h>
roboEyes roboEyes; // create RoboEyes instance

#define touchPin D7
#define angryPin D0

#define sda D2
#define scl D5

#define motor1GND D3
#define motor1VCC D1

unsigned int beenConfused = 0;
unsigned int readyToBeHappy = 0;

unsigned long timeStamp = 0;

unsigned int isAngry = 0;
unsigned int angered = 0;

void setup() {

  Wire.pins(sda, scl);
  Wire.begin();

  pinMode(touchPin, INPUT);
  pinMode(angryPin, INPUT);

  pinMode(motor1GND, OUTPUT);
  pinMode(motor1VCC, OUTPUT);

  Serial.begin(9600);

  //motor1.drive(255, 2000);

  // Startup OLED Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C or 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Startup robo eyes
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100); // screen-width, screen-height, max framerate

  // Define some automated eyes behaviour
  roboEyes.setAutoblinker(ON, 3, 2); // Start auto blinker animation cycle -> bool active, int interval, int variation -> turn on/off, set interval between each blink in full seconds, set range for random interval variation in full seconds
  //roboEyes.setIdleMode(ON, 2, 2); // Start idle animation cycle (eyes looking in random directions) -> turn on/off, set interval between each eye repositioning in full seconds, set range for random time interval variation in full seconds
  
  // Define eye shapes, all values in pixels
  //roboEyes.setWidth(36, 36); // byte leftEye, byte rightEye
  //roboEyes.setHeight(36, 36); // byte leftEye, byte rightEye
  //roboEyes.setBorderradius(8, 8); // byte leftEye, byte rightEye
  //roboEyes.setSpacebetween(10); // int space -> can also be negative

  // Cyclops mode
  //roboEyes.setCyclops(ON); // bool on/off -> if turned on, robot has only on eye

  // Define mood, curiosity and position
  //roboEyes.setMood(DEFAULT); // mood expressions, can be TIRED, ANGRY, HAPPY, DEFAULT
  //roboEyes.setPosition(DEFAULT); // cardinal directions, can be N, NE, E, SE, S, SW, W, NW, DEFAULT (default = horizontally and vertically centered)
  //roboEyes.setCuriosity(ON); // bool on/off -> when turned on, height of the outer eyes increases when moving to the very left or very right

  // Set horizontal or vertical flickering
  //roboEyes.setHFlicker(ON, 2); // bool on/off, byte amplitude -> horizontal flicker: alternately displacing the eyes in the defined amplitude in pixels
  //roboEyes.setVFlicker(ON, 2); // bool on/off, byte amplitude -> vertical flicker: alternately displacing the eyes in the defined amplitude in pixels

  // Play prebuilt oneshot animations
  //roboEyes.anim_confused(); // confused - eyes shaking left and right
  //roboEyes.anim_laugh(); // laughing - eyes shaking up and down
  
} // end of setup


void loop() {
 roboEyes.update(); // update eyes drawings

  

  digitalWrite(motor1GND, LOW);
  digitalWrite(motor1VCC, HIGH);
  // delay(100);
  // digitalWrite(motor1GND, HIGH);
  // digitalWrite(motor1VCC, LOW);
  // delay(100);
  // pinMode(motor1GND, INPUT);
  // pinMode(motor1VCC, INPUT);

  int isTouched = digitalRead(touchPin);
  int isAngryPressed = digitalRead(angryPin);

  if (isTouched == HIGH) {

      roboEyes.setMood(HAPPY);
      roboEyes.setHFlicker(ON, 2);
      roboEyes.setIdleMode(OFF, 2, 2);
      readyToBeHappy = 1;
      angered = 0;
      timeStamp = millis();
  }

  else {

      if (millis() >= (timeStamp + 350)) {
          roboEyes.setHFlicker(OFF, 2);
          roboEyes.setIdleMode(ON, 2, 2);
          if (readyToBeHappy == 1) {
              
              roboEyes.setMood(DEFAULT);

          }
      }

      

  }

  if (isAngryPressed == HIGH && isAngry == 0) {

      isAngry = 1;
      angered = angered + 1;
      Serial.println(angered);

  }

  else if (isAngryPressed == LOW) {

      isAngry = 0;

  }

  if (angered == 1 && beenConfused == 0) {

      roboEyes.anim_confused();
      beenConfused = 1;
      roboEyes.setIdleMode(OFF, 2, 2);
      timeStamp = millis();
  }

  else if (angered == 3 && beenConfused == 0) {

      roboEyes.anim_laugh();
      roboEyes.setCuriosity(ON);
      beenConfused = 1;
      roboEyes.setIdleMode(ON, 1, 0);
      timeStamp = millis();
  }

  else if (angered == 5 && beenConfused == 0) {

      roboEyes.setPosition(S);
      roboEyes.setCuriosity(OFF);
      roboEyes.setMood(ANGRY);
      roboEyes.anim_confused();
      beenConfused = 1;
      roboEyes.setIdleMode(OFF, 1, 0);
      readyToBeHappy = 0;
      timeStamp = millis();
  }

  if (millis() >= (timeStamp + 2000) && beenConfused == 1) {

      angered = angered + 1;
      beenConfused = 0;
      roboEyes.setIdleMode(ON, 2, 2);

  }

  
}
 // Dont' use delay() here in order to ensure fluid eyes animations.
 // Check the AnimationSequences example for common practices.
