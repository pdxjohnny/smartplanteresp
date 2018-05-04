#include "light.h" 

Light::Light(int pinNum, int muxVal, int muxS0PinNum, int muxS1PinNum) {
	// initialize pins
  pinNumber = pinNum;
  muxValue = muxVal;
  muxS0PinNumber = muxS0PinNum;
  muxS1PinNumber = muxS1PinNum;

  pinMode(pinNumber, INPUT);
  pinMode(muxS0PinNumber, OUTPUT);
  pinMode(muxS1PinNumber, OUTPUT);
}

int Light::getIntensity() {
  // update select line
  int S1 = muxValue/2;
  int S0 = muxValue%2;
  digitalWrite(muxS1PinNumber, S1);
  digitalWrite(muxS0PinNumber, S0);
  delay(100);

  
	// read from sensor
  int voltage = analogRead(A0);
  
	// return light intensity in integer
  return voltage; // TODO: convert voltage into light intensity (lux)
}
