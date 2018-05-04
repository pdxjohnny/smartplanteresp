#import <Arduino.h>

class WaterLevel {
	public:
		WaterLevel(int pinNum);
		bool waterPresent();
    
	private:
		int pinNumber;
};
