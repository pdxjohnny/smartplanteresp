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
