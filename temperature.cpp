#include "temperature.h"

Temperature :: Temperature(int pinNum, int muxVal, int muxS0PinNum, int muxS1PinNum) {
	// initialize pins (both output from mux and the pin to control the mux)
  pinNumber = pinNum;
  muxValue = muxVal;
  muxS0PinNumber = muxS0PinNum;
  muxS1PinNumber = muxS1PinNum;

  pinMode(pinNumber, INPUT);
  pinMode(muxS0PinNumber, OUTPUT);
  pinMode(muxS1PinNumber, OUTPUT);
	// return nothing
}

double Temperature :: farenheit() {
  // update select line
  int S1 = muxValue/2;
  int S0 = muxValue%2;
  digitalWrite(muxS1PinNumber, S1);
  digitalWrite(muxS0PinNumber, S0);
  delay(100);
  
  // read from sensor
  int voltage = analogRead(A0);
  
  // return temperature in integer
  return voltage; // TODO: convert voltage into temperature in ferenheit
}
