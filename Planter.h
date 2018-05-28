/*
 * File: Planter.h
 * Rev:  1.2
 * Date: 05/26/2018
 * 
 * Portland State University ECE Capstone Project
 * IoT-Based Smart Planter
 * Sponsor: Mukund Pai
 * Advisor: Roy Kravitz
 * Team Members: Abdulqader Alqadri, John Anderson, Alvin Lin,
 *               Tsegaslase Mebrahtu, Andrew Vo, Qiuren Wang
 *               
 * Revision History
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
#define MAGIC_NUMBER 24 // used to know if the memory is good and been written to

// define pins here
// 9 digital pins used, 1 analog used
#define MUXS0_PIN           D1
#define MUXS1_PIN           -1 // not used
#define NETWORKLED_PIN      D8  
#define WATERLED_PIN        D5
#define FERTILIZERLED_PIN   D4
#define PHOTORESISTOR_PIN   A0
#define MOISTURE_PIN        A0
#define WATERPUMP_PIN       D7
#define FERTILIZERPUMP_PIN  D2
#define TEMPERATURE_PIN     A0
#define WATERLVL_PIN        D6
#define FERTILIZERLVL_PIN   D3

// mux select lines
#define MOISTURE_SEL        1
#define TEMPERATURE_SEL     2
#define PHOTORESISTOR_SEL   0 // not used

#define DEFAULT_LOWER_BOUND 40
#define WATER_TANK_CAP      22 // TODO
#define FERTILIZER_TANK_CAP 12 // TODO

// nvmData is used to check the validity of memory
typedef struct {
  int wakeCount;
  bool bFirstTime;
  int magicNumber;
  
  // Configuration
  bool vacationMode;
  bool useFeritizer;
  int moistureLowerBound;

  // Tank status
  int currentWatersInTank;
  int currentFertilizersInTank;

  int daysBetweenWaters;
  int numberPumpRunsPerWater; 
  int vacationModeLength;

  char token[1024];

  bool demoMode;
  int demoFrequency;

  char mySSID[20];
  char myPASS[20];
} nvmData;

extern nvmData sleepMemory;
extern String getTime();
extern StaticJsonBuffer<1500> jsonBuffer;

class Planter {
  public:
    Planter();
    int configure(bool vacationModeIn, bool useFeritizerIn, int moistureLowerBoundInm, int vacationModeLength, bool demoMode, int demoFrequency);
    int water();
    String getJsonData(bool updateDaysBetweenWaters);

    Led NetworkErrLed = Led(NETWORKLED_PIN);

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
    //Led NetworkErrLed = Led(NETWORKLED_PIN); // moved to public
    //Led WaterLvlLed = Led(WATERLED_PIN); // hw controlled
    //Led FertilizerLvlLed = Led(FERTILIZERLED_PIN); // hw controlled

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
    int temperature;
    int light;
    int moisture;

    // fertilizer pointer
    const int fertilizersToUseArr[6] = {1, 1, 2, 1, 1, 0};
    int fertilizerPtr = 0;

    // demo mode
    bool demoMode;
    int demoFrequency;

    // error
    bool moistureError;
    
    // Temp vars
    int daysBetweenWatersCounter;
    bool waterLvlLow;
    bool fertilizerLvlLow;
    String prev_water;
};

