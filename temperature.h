#import <Arduino.h>

class Temperature {
	public:
		Temperature(int pinNum, int muxVal, int muxS0PinNum, int muxS1PinNum);
		double farenheit();
   
	private:
		int pinNumber;
		int muxValue;
		int muxS0PinNumber;
    int muxS1PinNumber;
};
