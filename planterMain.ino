/*
 * File: planterMain.ino
 * Rev:  1.0
 * Date: 05/19/2018
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
 *  
 *  Rev 0.5 05/17/2018
 *    Sketch uses 432793 bytes (41%) of program storage space. Maximum is 1044464 bytes.
 *    Global variables use 42888 bytes (52%) of dynamic memory, leaving 39032 bytes for local variables. Maximum is 81920 bytes.
 *    Summary:
 *      1. Added diagnostic for moisture sensor (not going to implement this for light sensor, not a moving part, unlikely to fail)
 *      2. Numbers of waters/fertilizers are reset when user refills the tanks
 *        - Assume user only refills water after he receives water level low alert
 *      3. Implemented WiFi error LED. Added timeout back to apConnect(). Added timeout argument to apConnect().
 *        Issue:
 *          a. It does not work when user tries to update the wifi settings after connection is lost (stack trace)
 *          --> Currently, it only tries to reconnect to the same AP. It's only able to receive WiFi settings at the beginning        
 *    Todo:
 *      1. planterMain.ino: Line 143: Update delay to 30 minutes
 *      2. Planter.h: Lines 57 & 58: Update number of waters/fertilizers
 *      
 *  Rev 1.0 05/19/2018
 *  Sketch uses 433549 bytes (41%) of program storage space. Maximum is 1044464 bytes.
 *  Global variables use 43148 bytes (52%) of dynamic memory, leaving 38772 bytes for local variables. Maximum is 81920 bytes.
 *    Summary:
 *      1. Added timestamp (last time it water in UTC) to json data
 *      2. Commented out water level LEDs out as they are now hardware controlled
 *      3. planterMain.ino: Line 143: Update delay to 30 minutes
 *      4. Added updateDaysBetweenWaters as an argument in getJsonData()& sendServerUpdatedJSON() to correctly update daysBetweenWaters
 *      5. Changed water() return type to int
 *    Todo:
 *      1. Planter.h: Lines 57 & 58: Update number of waters/fertilizers
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
  /* Network Error */
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi is not connected. Attempting to connect");
    Planter.NetworkErrLed.turnOn();
    apConnect(false, 1); // attempt to connect to ap w/o erasing SSID and PW
    
    if(WiFi.status() != WL_CONNECTED) { // try this again
      Serial.println("WiFi is still not connected.");
    } else {
      Planter.NetworkErrLed.turnOff(); 
    }
  } else {
    Planter.NetworkErrLed.turnOff();
    Serial.println("WiFi connection OK");
  }
  Serial.println("WiFi connection test done");

  /* Water */
  int waterStatus = Planter.water();
  if(waterStatus == 1) {
    getConfiguration();
    saveData();   
    if(!sendServerUpdatedJSON(true))
      Serial.println("send not sucess");
  } else if (waterStatus == -1) {
    getConfiguration();
    saveData();
    Serial.println("moisture error. notify server");
    if(!sendServerUpdatedJSON(false))
      Serial.println("send not sucess");
  }
  
  /* Server Communication*/
  if(configSinceLastUpdate >= 8) {
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
    delay(1*1e3);
  }
}
