
/*
  Example of reading values from a Bolsen Hall Effect water flow sensor
  and printing the values to a Grove-LCD RGB Backlight V4.0
  using an Arduino Uno Wifi.
  
  This example code is in the public domain.
*/

#include <Wire.h>
#include "rgb_lcd.h"

volatile int flow_frequency; // Measures flow sensor pulses
// Calculated litres/hour
float vol = 0.0,l_minute;
unsigned char flowsensor = 2; // Sensor Input Pin
unsigned long currentTime;
unsigned long cloopTime;
rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH); 
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setRGB(colorR, colorG, colorB);
  attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Water Flow Meter");
  currentTime = millis();
  cloopTime = currentTime;
}

// the loop function runs over and over again forever
void loop() {
  currentTime = millis();
  // Every second, calculate and print litres/hour
  if(currentTime >= (cloopTime + 1000)) {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print(l_minute);
      lcd.print(" L/M");
      l_minute = l_minute/60;
      lcd.setCursor(0,1);
      vol = vol +l_minute;
      lcd.print("Vol:");
      lcd.print(vol);
      lcd.print(" L");
      flow_frequency = 0; // Reset Counter
      Serial.print(l_minute, DEC); // Print litres/hour
      Serial.println(" L/Sec");
   } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print( flow_frequency );
      lcd.print(" L/M");
      lcd.setCursor(0,1);
      lcd.print("Vol:");
      lcd.print(vol);
      lcd.print(" L");
    }
  }
}

// Interrupt function
void flow () {
 flow_frequency++;
}
