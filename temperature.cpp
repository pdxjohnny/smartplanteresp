/*
 * File: temperature.cpp
 * Rev:  1.0
 * Date: 05/09/2018
 * Description: 
 *  This is not currently in use due to lack of pins.
 *  It currently supports two mux select lines. If 
 *  only S0 is used, enter -1 for S1.
 * 
 * Portland State University ECE Capstone Project
 * IoT-Based Smart Planter
 * Sponsor: Mukund Pai
 * Advisor: Roy Kravitz
 * Team Members: Abdulqader Alqadri, John Anderson, Alvin Lin,
 *               Tsegaslase Mebrahtu, Andrew Vo, Qiuren Wang
 *               
 * Revision History
 *  Rev 1.0 05/09/2018: Initial release
 */

#include "temperature.h"

Temperature :: Temperature(int pinNum, int muxVal, int muxS0PinNum, int muxS1PinNum) {
	// initialize pins (both output from mux and the pin to control the mux)
  pinNumber = pinNum;
  muxValue = muxVal;
  muxS0PinNumber = muxS0PinNum;
  muxS1PinNumber = muxS1PinNum;

  pinMode(pinNumber, INPUT);
  pinMode(muxS0PinNumber, OUTPUT);

  if(muxS1PinNumber != -1)
    pinMode(muxS1PinNumber, OUTPUT);
	// return nothing
}

double Temperature :: farenheit() {
  // update select line
  int S1 = muxValue/2;
  int S0 = muxValue%2;
  
  if(muxS1PinNumber != -1)
    digitalWrite(muxS1PinNumber, S1);
    
  digitalWrite(muxS0PinNumber, S0);
  delay(100);
  
  // read from sensor
  int voltage = analogRead(A0);
  
  // return temperature in integer
  return voltage; // TODO: convert voltage into temperature in ferenheit
}
