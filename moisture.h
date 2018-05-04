#import <Arduino.h>

class Moisture {
	public:
		Moisture(int pinNum, int muxVal, int muxS0PinNum, int muxS1PinNum);
		int getMoisture();
    
	private:
		int pinNumber;
		int muxValue;
    int muxS0PinNumber;
    int muxS1PinNumber;
};
