/*
 * File: light.cpp
 * Rev:  0.9
 * Date: 05/09/2018
 * Description: 
 *  It currently supports two mux select lines. If 
 *  only S0 is used, enter -1 for S1. The output
 *  currently is ADC, needs to do more testing to
 *  convert it properly to intensity (lux).
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

#include "light.h" 

Light::Light(int pinNum, int muxVal, int muxS0PinNum, int muxS1PinNum) {
	// initialize pins
  pinNumber = pinNum;
  muxValue = muxVal;
  muxS0PinNumber = muxS0PinNum;
  muxS1PinNumber = muxS1PinNum;

  pinMode(pinNumber, INPUT);
  pinMode(muxS0PinNumber, OUTPUT);
  
  if(muxS1PinNumber != -1)
    pinMode(muxS1PinNumber, OUTPUT);
}

int Light::getIntensity() {
  // update select line
  int S1 = muxValue/2;
  int S0 = muxValue%2;

  if(muxS1PinNumber != -1)
    digitalWrite(muxS1PinNumber, S1);
    
  digitalWrite(muxS0PinNumber, S0);
  delay(100);

  
	// read from sensor
  int voltage = analogRead(A0);
  
	// return light intensity in integer
  return voltage; // TODO: convert voltage into light intensity (lux)
}
