#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include "Adafruit_LEDBackpack.h"
#include "pitches.h"

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))
#define DOTTED(X) (X * 1.5f)

#define TRIG_PIN    11
#define ECHO_PIN    10
#define BUZZER_PIN  9

#define TEMPO 1250

#define DISPLAY_ADDRESS   0x70

Adafruit_7segment clockDisplay = Adafruit_7segment();

unsigned long timerMillis = 0;

unsigned long clockTime;

boolean start;

const int melody[][2] = {
  {NOTE_G6, DOTTED(NOTE_EIGHTH)}, {NOTE_G6, NOTE_SIXTEENTH}, {NOTE_A6, NOTE_QUARTER}, {NOTE_G6, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, {NOTE_B6, NOTE_HALF}, 
  {NOTE_G6, DOTTED(NOTE_EIGHTH)}, {NOTE_G6, NOTE_SIXTEENTH}, {NOTE_A6, NOTE_QUARTER}, {NOTE_G6, NOTE_QUARTER}, {NOTE_D7, NOTE_QUARTER}, {NOTE_C7, NOTE_HALF},
  {NOTE_G6, DOTTED(NOTE_EIGHTH)}, {NOTE_G6, NOTE_SIXTEENTH}, {NOTE_E7, NOTE_QUARTER}, {NOTE_D7, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, {NOTE_B6, NOTE_QUARTER}, {NOTE_A6, NOTE_HALF}, 
  {NOTE_F7, DOTTED(NOTE_EIGHTH)}, {NOTE_F7, NOTE_SIXTEENTH}, {NOTE_E7, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, {NOTE_D7, NOTE_QUARTER}, {NOTE_C7, NOTE_HALF},      

};

void setup() {
  // Set pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // set 7-segment display
  clockDisplay.begin(DISPLAY_ADDRESS);
  Serial.begin(9600);

  start = false;
}

void loop() {
  long pulseDuration, distance;

  triggerUltraSoundPulse();
  pulseDuration = pulseIn(ECHO_PIN, HIGH);

  distance = pulseDuration * 0.034 / 2;
  
  if (distance > 0 && distance < 5 && !start) {
    start = true;
    clockTime = millis();
  }

  if (start) {
    startTimer();
  } else {
    turnClockDisplayOff();
    noTone(BUZZER_PIN); 
  }
}

void startTimer() {
    unsigned long timeInS;
   
    for (int note = 0; note < LEN(melody) && start; note++) {
      int noteDuration = TEMPO / melody[note][1];
      int pauseBetweenNotes = noteDuration * 1.30;
  
      unsigned long musicTime = millis();
      tone(BUZZER_PIN, melody[note][0], noteDuration);
      
      while (millis() - musicTime < pauseBetweenNotes) {
         timeInS = 20 - (millis() - clockTime) / 1000;
         printTime(timeInS);
      }
      noTone(BUZZER_PIN);
      if (timeInS <= 0) {
        start = false;
      }
    }



}
    
void printTime(long time_counter) {
    clockDisplay.writeDigitNum(0, (time_counter / 600) % 10, false);
    clockDisplay.writeDigitNum(1, (time_counter / 60) % 10, false);
    clockDisplay.drawColon(true);
    clockDisplay.writeDigitNum(3, (time_counter / 10) % 6, false);
    clockDisplay.writeDigitNum(4, time_counter % 10, false);
    clockDisplay.writeDisplay();
}

void turnClockDisplayOff(){
    clockDisplay.writeDigitRaw(0, 0);
    clockDisplay.writeDigitRaw(1, 0);
    clockDisplay.writeDigitRaw(3, 0);
    clockDisplay.writeDigitRaw(4, 0);
    clockDisplay.writeDisplay();
}

/**
 * enable the Ultrasound sensor to send a pulse
 */
void triggerUltraSoundPulse() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
}
