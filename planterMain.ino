/*
 * File: planterMain.ino
 * Rev:  0.4
 * Date: 05/16/2018
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
 *  Rev 0.2 04/21/2018
 *  Rev 0.3 05/10/2018
 *    Sketch uses 429469 bytes (41%) of program storage space. Maximum is 1044464 bytes.
 *    Global variables use 41612 bytes (50%) of dynamic memory, leaving 40308 bytes for local variables. Maximum is 81920 bytes.
 *    Summary:
 *      1. Added sensors
 *      2. Added water function
 *      3. Removed unused functions
 *      4. Uses delay instead of sleep
 *      5. Waters every 30 minutes if not moist
 *    
 *    Todo:
 *      1. Add demo mode
 *      2. Update configure function
 *      3. Add diagnostic
 *      4. Other TODOs commented in the code
 *  
 *  Rev 0.4 05/16/2018
 *    Sketch uses 432201 bytes (41%) of program storage space. Maximum is 1044464 bytes.
 *    Global variables use 42664 bytes (52%) of dynamic memory, leaving 39256 bytes for local variables. Maximum is 81920 bytes.
 *    Summary:
 *      1. Completed daysBetweenWaters
 *      2. Improved vacation mode by using previous data (days between waters) and current water level
 *      3. Planter sends data to server every time it waters
 *      4. Planter asks for configuration every 4 hours
 *      5. Added demo mode
 *      6. Added numbers of waters/fertilizers tracking
 *      7. Updated configure function
 *    Todo:
 *      1. Add diagnostic for moisture and light sensors
 *      2. Use Network LED to indicate and error where planter is not able to connect to Internet
 *      3. Reset numbers of waters/fertilizers when user refills the tanks
 *        - Assume user only refills water after he receives water level low alert
 */
#include "helperFunc.h"
nvmData sleepMemory;
class Planter Planter;
WiFiManager wifiManager;
int configSinceLastUpdate;

void setup() {
  Serial.begin(9600);
  Serial.println("settig up...");
  
  initialize();

  Serial.println("~GET CONFIG START~");
  if(sleepMemory.magicNumber != MAGIC_NUMBER) { // Data read from memory is not valid
    memoryCorrupted();
  }
  else {
    wakeup();
  }
  configSinceLastUpdate = 0;
  Serial.println("~GET CONFIG END~");
}

void loop() 
{
  
  Serial.println("loop");

  /* Water */
  if(Planter.water()) {   
    if(!sendServerUpdatedJSON())
      Serial.println("send not sucess");
    else {
      ; // Do nothing here
    }
  }
  
  /* Server Communication*/
  if(configSinceLastUpdate == 8) {
    //sendServerUpdatedJSON();
    getConfiguration();
    saveData();
    configSinceLastUpdate = 0;
  }
  configSinceLastUpdate += 1;

  /* Delay */
  if (sleepMemory.demoMode) {
    delay(sleepMemory.demoFrequency*1e3);
  } else {
    delay(2*1e3); // TODO
  }
}
