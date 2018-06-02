/*
 * File: pump.h
 * Rev:  1.0
 * Date: 05/09/2018
 * Description: 
 *  This is not currently in use due to lack of pins.
 *  It currently supports two mux select lines. If 
 *  only S0 is used, enter -1 for S1.
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

#include <Arduino.h>

#define WATER_TYPE      1
#define FERTILIZER_TYPE 2

#define DISPENCE_ONE_WATER_TIME       5000
#define DISPENCE_ONE_FERTILIZER_TIME  500

class Pump {
	public:
		Pump(int pinNum, int type);
		void pumpWater();
   
	private:
		int pinNumber;
    int type;
};
