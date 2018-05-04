/*
 * File: Planter.h
 * Rev:  0.2
 * Date: 05/01/2018
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
#include "temperature.h"
#include "led.h"
#include "light.h"
#include "moisture.h"
#include "waterLevel.h"
#include "pump.h"

// NVM Data
#define RTCMemOffset 11 // arbitrary location
#define MAGIC_NUMBER 25 // used to know if the memory is good and been written to

// define pins here
// 9 digital pins used, 1 analog used
// TODO: update actual pin numbers
#define MUXS0_PIN           1
#define MUXS1_PIN           2
#define NETWORKLED_PIN      3  
#define WATERLED_PIN        4
#define FERTILIZERLED_PIN   5
#define PHOTORESISTOR_PIN   A0
#define MOISTURE_PIN        A0
#define WATERPUMP_PIN       6
#define FERTILIZERPUMP_PIN  7
#define TEMPERATURE_PIN     A0
#define WATERLVL_PIN        8
#define FERTILIZERLVL_PIN   9

// mux select lines
#define MOISTURE_SEL        1
#define TEMPERATURE_SEL     2
#define PHOTORESISTOR_SEL   0

class Planter {
  public:
    Planter();
    int configure(bool vacationModeIn, bool useFeritizerIn, int moistureLowerBoundInm, int vacationModeLength);
    int sleep();
    void sleep(int minutes);
    void displayInternalVars(); // debug

    int water(); // called everytime 30 mintues have elapsed

    String getDataJson();

    // The following functions are used to test hardware connections
    void inputsTest();
    void outputsTest();

  private:
    /* inputs */
    // Analog sensors
    Light LightSensor = Light(PHOTORESISTOR_PIN, PHOTORESISTOR_SEL, MUXS0_PIN, MUXS1_PIN);
    Moisture MoistureSensor = Moisture(MOISTURE_PIN, MOISTURE_SEL, MUXS0_PIN, MUXS1_PIN);
    // temperature

    // Water Lvl sensors
    WaterLevel WaterLevelSensor = WaterLevel(WATERLVL_PIN);
    WaterLevel FertilizerLevelSensor = WaterLevel(FERTILIZERLVL_PIN);
  
    /* outputs */
    // LEDs
    Led NetworkErrLed = Led(NETWORKLED_PIN);
    Led WaterLvlLed = Led(WATERLED_PIN);
    Led FertilizerLvlLed = Led(FERTILIZERLED_PIN);

    // Pumps
    Pump WaterPump = Pump(WATERPUMP_PIN, WATER_TYPE);
    Pump FertilizerPump = Pump(FERTILIZERPUMP_PIN, FERTILIZER_TYPE);

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

