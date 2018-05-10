/*
 * File: led.cpp
 * Rev:  1.0
 * Date: 05/09/2018
 * Description: 
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
 
#include "led.h"

Led::Led(int pinNum) {
  // set pin number
  pinNumber = pinNum;
  // initialize pin mode
  pinMode(pinNumber, OUTPUT);
}

void Led::turnOn() {
	// turn on led
  digitalWrite(pinNumber, HIGH);
	// return nothing
}

void Led::turnOff() {
	// turn off led
  digitalWrite(pinNumber, LOW);
	// return nothing
}
