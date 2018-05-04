#import <Arduino.h>

class Light {
	public:
		Light(int pinNum, int muxVal, int muxS0PinNum, int muxS1PinNum);
		int getIntensity();
    
	private:
		int pinNumber;
		int muxValue;
    int muxS0PinNumber;
    int muxS1PinNumber;
};
