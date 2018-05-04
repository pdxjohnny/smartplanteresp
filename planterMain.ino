/*
 * File: planterMain.ino
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
 *  Rev 0.1 04/11/2018
 *    Sketch uses 332225 bytes (31%) of program storage space. Maximum is 1044464 bytes.
 *    Global variables use 36688 bytes (44%) of dynamic memory, leaving 45232 bytes for local variables. Maximum is 81920 bytes.
 *    Features:
 *      1. deep sleep / wake up for power saving 
 *      2. wifi configure with smart phone or any internet compatible device
 *      3. hard-coded configuration
 *      4. json parsing for configuration
 *      5. json creating for data output
 *    Todo:
 *      1. add sensors into planter class
 *      2. read data from sensors
 *      3. water / fertilize the plant
 *      4. read configuration in json from server
 *      5. send data in json to server
 *      
 *  Rev 0.2 4/21/2018
 *  
 *    New features:
 *    
 *    Features removed:
 *    
 *    Todo:
 */
 
#include "helperfunc.h"
nvmData sleepMemory;
class Planter Planter;
WiFiManager wifiManager;

bool bDoneSleeping;
int timer;
int timerHalfHour;
int timerMinute;

int currentHour;
int currentMinute;

/*
Led WLed(rLedPin);
Led GLed(gLedPin);
Led BLed(bLedPin);
Light LightSensor(lightPin, lightSel, muxS0Pin, muxS1Pin);
Moisture MoistureSensor(moisturePin, moistureSel, muxS0Pin, muxS1Pin);
Pump WaterPump(waterPumpPin, 1);
Pump FertilizerPump(FertilizerPumpPin, 2);
Temperature TemperatureSensor(temperaturePin, temperatureSel, muxS0Pin, muxS1Pin);
WaterLevel WaterLevelSnesor(waterLvlPin);
WaterLevel FertilizerLevelSnesor(FertilizerLvlPin);
*/


// fertilizer pointer
int fertilizersToUseArr[6] = {1, 1, 2, 1, 1, 0};
int fertilizerPtr = 0;

int moistureLowerBound = 650;

/*
void water() {
  bool okayToWater;
  bool okayToFertilize;

  if(!WaterLevelSnesor.waterPresent()) {
    WLed.turnOn();
    okayToWater = false;
  }
  else {
    WLed.turnOff();
    okayToWater = true;
  }

  if(!FertilizerLevelSnesor.waterPresent()) {
    GLed.turnOn();
    okayToFertilize = true;
  }
  else {
    GLed.turnOff();
    okayToFertilize = false;
  }
  
  if(MoistureSensor.getMoisture() < moistureLowerBound) {
    // Water the plant
    WaterPump.pumpWater();

    for(int i=0; i<fertilizersToUseArr[fertilizerPtr]; ++i) {
      FertilizerPump.pumpWater();
    }

    // update water info

    // update fertilizer pointer
    fertilizerPtr += 1;
    if(fertilizerPtr >= 6)
      fertilizerPtr = 0; 
  }
}
*/
void setup() {
  initialize();
  if(sleepMemory.magicNumber != MAGIC_NUMBER) { // Data read from memory is not valid
    memoryCorrupted();
    // TODO: Read sensors
    // TODO: Water the plant
    // TODO: Read water level sensors
    // TODO: Send data to server
  }
  else {
    wakeup();
    // TODO: Read sensors
    // TODO: Water the plant
    // TODO: Read water level sensors
    // TODO: Send data to server
  }
  String output = Planter.getDataJson();
  Serial.print("output: ");
  Serial.println(output);
  goToSleep();
}

void loop() 
{
  // Nothing in here
}
