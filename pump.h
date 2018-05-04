#import <Arduino.h>

#define WATER_TYPE      1
#define FERTILIZER_TYPE 2

#define DISPENCE_ONE_WATER_TIME       1000
#define DISPENCE_ONE_FERTILIZER_TIME  250

class Pump {
	public:
		Pump(int pinNum, int type);
		void pumpWater();
   
	private:
		int pinNumber;
    int type;
};
