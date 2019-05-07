/*
 * Art 150: Final Project
 * Author: Ronny Recinos
 * Project: Not An Actual Bomb
 * 
 * Desc: This code is uploaded to the Arduino that is connected to the LN298N H-Bridge Motor Module
 *       and recieves/sends input from the PS4 controller to the LN298N Module or the Speakers Arduino
 * 
 */

// Bluetooth Settings
#include <PS4BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <L298N.h>

/*
 * IN1 = WHITE WIRE
 * IN2 = YELLOW WIRE
 * 
 * IN3 = BLUE WIRE
 * IN4 = PURPLE WIRE
 * 
 * ENA = GRAY WIRE
 * ENB = GREEN WIRE
 */

// Controls left side of the car wheels
#define ENA 9
#define IN1 2
#define IN2 8

// Controls right side of the car wheels
#define IN3 4
#define IN4 7
#define ENB 3

USB Usb;
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
PS4BT PS4(&Btd, PAIR);

L298N motorR(ENA, IN1, IN2);
L298N motorL(ENB, IN3, IN4);


uint8_t carTrigger = 99;

int setSpeed = 255;
  
void setup() { 
  Serial.begin(9600);
  #if !defined(__MIPSEL__)
  while (!Serial); 
  #endif
  if (Usb.Init() == -1) {
    while (1); 
    }
    
  motorR.setSpeed(setSpeed); // an integer between 0 and 255
  motorL.setSpeed(setSpeed); // an integer between 0 and 255
}

void loop() {
  Usb.Task();
  if (PS4.connected()) {
    if (PS4.getButtonClick(PS)) { 
         PS4.disconnect(); 
    }
    else {
       // Move Car Backwards
       if(carTrigger == 0 && carTrigger != 1){
           motorR.setSpeed(setSpeed); // an integer between 0 and 255
           motorL.setSpeed(setSpeed); // an integer between 0 and 255
           motorR.forward();
           motorL.forward();
       }  
       // Move Car Forwards
       else if(carTrigger == 1 && carTrigger != 0){
           motorR.setSpeed(setSpeed); // an integer between 0 and 255
           motorL.setSpeed(setSpeed); // an integer between 0 and 255
           motorR.backward();
           motorL.backward();

       }
       // Stop Car
       else if(carTrigger == 2){
        
           motorR.stop();
           motorL.stop();
       }
       // Move Car Forward Left
       else if(carTrigger == 4 && carTrigger != 0){
          motorL.setSpeed(setSpeed);
          motorL.backward();

          motorR.setSpeed(setSpeed * .2);
          motorR.backward();
       }
       //Move Car Forward Right
       else if(carTrigger == 3 && carTrigger != 0){
          motorL.setSpeed(setSpeed * .2);
          motorL.backward();

          motorR.setSpeed(setSpeed);
          motorR.backward();
       }
       // Move Car Back Left
       else if(carTrigger == 6 && carTrigger != 0){
          motorL.setSpeed(setSpeed);
          motorL.forward();

          motorR.setSpeed(setSpeed * .2);
          motorR.forward();
       }
       //Move Car Back Right
       else if(carTrigger == 5 && carTrigger != 0){
          motorL.setSpeed(setSpeed * .2);
          motorL.forward();

          motorR.setSpeed(setSpeed);
          motorR.forward();
       }

/*
 * CAR MOVEMENT BUTTONS
 */
      // Stops Car
      if (PS4.getButtonPress(L2)) {     
          carTrigger = 2;
      }
      else
      // Moves the car forward
      if (PS4.getButtonPress(R2)) {
          carTrigger = 1;

          if (PS4.getButtonPress(LEFT)){
             carTrigger = 3;
          }
          else
          // Turns the car Forward Right
          if (PS4.getButtonPress(RIGHT)){
             carTrigger = 4;
          }     
      }
      else
      // Moves the car backwards
      if (PS4.getButtonPress(R1)) {
         carTrigger = 0;

          if (PS4.getButtonPress(LEFT)){
             carTrigger = 5;
          }
          else
          // Turns the car Forward Right
          if (PS4.getButtonPress(RIGHT)){
             carTrigger = 6;
          }    
         
      }
      else
      // Turns the car Forward Left
      if (PS4.getButtonPress(LEFT)){
         carTrigger = 3;
      }
      else
      // Turns the car Forward Right
      if (PS4.getButtonPress(RIGHT)){
         carTrigger = 4;
      }

/*
 * END OF CAR MOVEMENT BUTTONS
 */


/*
 * SIGNALS TO OTHER ARDUINO BUTTONS
 */

    
      // Send Signal to turn ON the Speakers
      if (PS4.getButtonClick(SQUARE)){
          PS4.setRumbleOn(RumbleHigh);
          PS4.setLed(Red);
          PS4.setLedFlash(10, 10);
          Serial.write("1\n");
      }
    
    
      // Send Signal to turn OFF the Speakers
      if (PS4.getButtonClick(CIRCLE)){
          PS4.setRumbleOff();
          PS4.setLed(Green);
          PS4.setLedFlash(0, 0);
          Serial.write("0\n");     
      }
      /*
       * END OF SIGNALS TO OTHER ARDUINO BUTTONS
       */
  
    } 
  } 
}
