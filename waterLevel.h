/*
 * File: waterLevel.h
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

#include <Arduino.h>

class WaterLevel {
	public:
		WaterLevel(int pinNum);
		bool waterPresent();
    
	private:
		int pinNumber;
};
