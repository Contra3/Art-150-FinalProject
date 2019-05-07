/*
 * Art 150: Final Project
 * Author: Ronny Recinos
 * Project: Not An Actual Bomb
 * 
 * Desc: This code is uploaded to the Arduino that is connected to the speakers and recieves only recieves information from the
 *       CarArduino
 *       
 * 
 */


#include "pitches.h"

int melody[] = {NOTE_C5, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, 0, NOTE_B4, NOTE_C5};
int noteDurations[] = {8, 12, 12, 8, 8, 4, 8, 8};

int speakerOut = 6;
int bombTimer = 0;
int readSerial = 0;
bool bombFlag = false;
char serialStorage[15]; // Buffer used for the readBytes function

// Does some filler noise for the explode function
void noise(int freq, int duration)  {
  int low = freq - 300;
  int high = freq + 300;

  unsigned long time = millis();
  while (millis() - time <= duration)  {
    int rand = random(low, high) - duration / 100;
    tone(speakerOut, rand);
  }
}

// This will play an explosion sound
void explode() {
  tone(speakerOut, NOTE_A5, 500);
  delay(500);

  unsigned long time = millis();
  while (millis() - time <= 300)  {
    tone(speakerOut, (millis() - time) / 2);
  }

  noise(100, 100);

  time = millis();
  while (millis() - time <= 300)  {
    tone(speakerOut, 300 - (millis() - time) / 2);
  }

  noise(100, 1500);

  noTone(speakerOut);

}

void bombSound() {

  if (bombFlag == true) {
    if (bombTimer < 5) {
      tone(speakerOut, 2100, 500);
      bombTimer++;
      delay(900);
    }
     else if (bombTimer >= 5 && bombTimer < 10) {
      tone(speakerOut, 2100, 200);
      delay(500);
      bombTimer++;
    }
    else if (bombTimer >= 10 && bombTimer < 15) {
      tone(speakerOut, 2100, 190);
      delay(400);
      bombTimer++;
    }
    else if (bombTimer >= 15 && bombTimer < 25) {
      tone(speakerOut, 2100, 180);
      delay(300);
      bombTimer++;
    }
    else if (bombTimer >= 25 && bombTimer < 30) {
      tone(speakerOut, 2100, 150);
      delay(200);
      bombTimer++;
    }
    else if (bombTimer >= 30 && bombTimer < 50) {
      tone(speakerOut, 2100, 50);
      delay(100);
      bombTimer++;
    }
    else if (bombTimer >= 50 && bombTimer < 60) {
      tone(speakerOut, 1900);
      delay(75);
      bombTimer++;
    }
    else if (bombTimer >= 60) {
      explode();
      bombFlag = false;
      bombTimer = 0;
    }
  }
}

void setup() {
  pinMode(speakerOut, OUTPUT);
  Serial.begin(9600); // Set serial out if we want debugging
}

void loop() {

  readSerial = Serial.read();
  Serial.println(readSerial);
  
  if (readSerial == 49) {
    bombFlag = true;
  }
  else if (readSerial == 48) {
    bombFlag = false;
  }


  bombSound();
 


}
