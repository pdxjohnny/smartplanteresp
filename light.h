/*
 * File: light.h
 * Rev:  0.9
 * Date: 05/09/2018
 * Description: 
 *  It currently supports two mux select lines. If 
 *  only S0 is used, enter -1 for S1. The output
 *  currently is ADC, needs to do more testing to
 *  convert it properly to intensity (lux).
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
