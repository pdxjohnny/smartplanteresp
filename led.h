#import <Arduino.h>

class Led {
	public:
		Led(int pinNum);
		void turnOn();
		void turnOff();
   
	private:
		int pinNumber;
};
