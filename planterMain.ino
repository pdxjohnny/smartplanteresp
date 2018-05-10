/*
 * File: planterMain.ino
 * Rev:  0.3
 * Date: 05/09/2018
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
 */
#include "helperFunc.h"
nvmData sleepMemory;
class Planter Planter;
WiFiManager wifiManager;

void setup() {
  Serial.begin(9600);
  Serial.println("settig up...");
  
  initialize();
  if(sleepMemory.magicNumber != MAGIC_NUMBER) { // Data read from memory is not valid
    memoryCorrupted();
  }
  else {
    //wakeup();
  }  

  // TODO: remove this line of code. It is for testing only
  Planter.configure(1, 1, 40, 0, false, 30);
  saveData();
}

void loop() 
{
  Serial.println("loop");
  if(Planter.water())
    if(!sendServerUpdatedJSON())
      Serial.println("send not sucess");
    else {
      ;// TODO: need to reset daysBetweenWatering
      saveData();
    }

  if (sleepMemory.demoMode) {
    delay(sleepMemory.demoFrequency * 1e3);
    Planter.water();
    getConfiguration();
  } else {
    delay(10*1e3); // TODO: set this to 30 minutes, or, in demo mode, set this to the frequency defined in json
  }
}
