/*
 * File: Planter.h
 * Rev:  0.2
 * Date: 04/21/2018
 * 
 * Portland State University ECE Capstone Project
 * IoT-Based Smart Planter
 * Sponsor: Mukund Pai
 * Advisor: Roy Kravitz
 * Team Members: Abdulqader Alqadri, John Anderson, Alvin Lin,
 *               Tsegaslase Mebrahtu, Andrew Vo, Qiuren Wang
 *               
 * Revision History
 *  Rev 0.1: 04/11/2018
 *    See planterMain.ino
 */

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <string>
#include "user_interface.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include "ArduinoJson.h"

// NVM Data
#define RTCMemOffset 12 // arbitrary location
#define MAGIC_NUMBER 25 // used to know if the memory is good and been written to

class Planter {
  public:
    Planter();
    int configure(bool vacationModeIn, bool useFeritizerIn, int moistureLowerBoundIn);
    int sleep();
    void sleep(int minutes);

    void displayInternalVars(); // debug
    
    // function to read all sensors 

    // function to read water levels (used after watering)

    String getDataJson();
    
  private:
    // moisture sensor

    // temperature sensor

    // light sensor

    // water level sensor 1: water 1

    // water level sensor 2: water 2

    // water level sensor 3: miracleGro 1

    // water level sensor 4: miracleGro 2

    // water pump 1: water

    // water pump 2: miracleGro

    // RGB LED

    // Internal Variables    
    //int currentHour;
    //int currentMinute;

    /*
    int waterStartHour;
    int waterFrequency;
    bool useMiracleGro;
    int moistureLowerBound;
    */
    
    // Configuration
    bool vacationMode;
    bool useFeritizer;
    int moistureLowerBound;

    // Tank size information 
    int numberWatersInTank;
    int numberFertilizersInTank;

    // Tank status
    int currentWatersInTank;
    int currentFertilizersInTank;

    // Water schedule
    double daysBetweenWaters;
    int numberPumpRunsPerWater;
    int vacationModeLength;

    // Sensor reading
    //int numberWatersUsed;
    //int numberFertilizersUsed; 
    int temperature;
    int light;
    int moisture;
};

