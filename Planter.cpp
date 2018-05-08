/*
 * File: Planter.cpp
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

#include "Planter.h"

Planter::Planter() {
  /*
  waterStartHour = 0;
  waterFrequency = 8;
  useMiracleGro = true;
  moistureLowerBound = 30;
  */

  Serial.println("Planter initializing vars");
  /*
  moistureBefore = 0;
  moistureAfter = 0;
  temperature = -40;
  light = -1;
  waterLevel = 1;
  miracleGroLevel = 1;
  watered = 0;
  fertilized = 0;
  */
}


int Planter::configure(bool vacationModeIn, bool useFeritizerIn, int moistureLowerBoundIn, int vacationModeLengthIn, bool demoModeIn, int demoFrequencyIn) {
  /*
  // Test if inputs are valid
  if(waterStartHourIn < 0 || waterStartHourIn > 23) 
    return -1;
  if(waterFrequencyIn != 1 && waterFrequencyIn != 2 && waterFrequencyIn != 3 && waterFrequencyIn != 4 && waterFrequencyIn != 6 && waterFrequencyIn != 8 && waterFrequencyIn != 12) 
    return -2;
  */
  if(moistureLowerBoundIn < 0 || moistureLowerBoundIn > 100) 
    return -1;

  /*
  while(waterStartHourIn - waterFrequencyIn > 0)
    waterStartHourIn -= waterFrequencyIn; // Make startHour to be the hour of the very first watering schedule of the day
  */
  
  // If inputs are valid, store them into internal variables
  /*
  waterStartHour = waterStartHourIn;
  waterFrequency = waterFrequencyIn;
  useMiracleGro = useMiracleGroIn;
  moistureLowerBound = moistureLowerBoundIn;
  */
  vacationMode = vacationModeIn;
  useFeritizer = useFeritizerIn;
  moistureLowerBound = moistureLowerBoundIn;
  vacationModeLength = vacationModeLengthIn;
  demoMode = demoModeIn;
  demoFrequency = demoFrequencyIn;

  
  return 0;
}




// startHour is expected to be the hour of the very first watering schedule of the time
/*
int Planter::calcTime() {
  //getTime();
  while(currentHour >= waterStartHour) 
    waterStartHour += waterFrequency; // Make startHour to be the hour of the next watering schedule of the day

  return waterStartHour * 60 - (currentHour * 60 + currentMinute); // return watering timer (in minutes)
}
*/


int Planter::sleep() {
  ESP.deepSleep(1*1000000);
  return 0;
}

// TODO currently is seconds for debugging purposes
void Planter::sleep(int minutes) {
  ESP.deepSleep(minutes * 60 * 1000000);
}

void Planter::displayInternalVars() {
  /*
  Serial.print("currentHour: ");
  Serial.println(currentHour);

  Serial.print("currentMinute: ");
  Serial.println(currentMinute);

  Serial.print("waterStartHour: ");
  Serial.println(waterStartHour);

  Serial.print("waterFrequency: ");
  Serial.println(waterFrequency);

  Serial.print("useMiracleGro: ");
  Serial.println(useMiracleGro);

  Serial.print("moistureLowerBound: ");
  Serial.println(moistureLowerBound);
  */
}

String Planter::getDataJson() {

    char json[] = "{\"vacationMode\":\"1\", "
                "\"useFeritizer\":\"1\", "
                "\"moistureLowerBound\":\"40\", "
                "\"numberWatersInTank\":\"15\", "
                "\"numberFertilizersInTank\":\"8\", "
                "\"currentWatersInTank\":\"20\", "
                "\"currentFertilizersInTank\":\"10\", "
                "\"daysBetweenWaters\":\"1\", "
                "\"numberPumpRunsPerWater\": \"1\", "
                "\"vacationModeLength\": \"0\", " 
                "\"temperature\":\"100\", "
                "\"light\":\"110\", "
                "\"moisture\":\"38\"}";

           
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& vacationModeArr = root.createNestedArray("vacationMode");
  JsonArray& useFeritizerArr = root.createNestedArray("useFeritizer");
  JsonArray& moistureLowerBoundArr = root.createNestedArray("moistureLowerBound");
  JsonArray& numberWatersInTankArr = root.createNestedArray("numberWatersInTank");
  JsonArray& numberFertilizersInTankArr = root.createNestedArray("numberFertilizersInTank");
  JsonArray& currentWatersInTankArr = root.createNestedArray("currentWatersInTank");
  JsonArray& currentFertilizersInTankArr = root.createNestedArray("currentFertilizersInTank");
  JsonArray& daysBetweenWatersArr = root.createNestedArray("daysBetweenWaters");
  JsonArray& numberFertilizersUsedArr = root.createNestedArray("numberFertilizersUsed");
  JsonArray& vacationModeLengthArr = root.createNestedArray("vacationModeLength");
  JsonArray& temperatureArr = root.createNestedArray("temperature");
  JsonArray& lightArr = root.createNestedArray("light");
  JsonArray& moistureArr = root.createNestedArray("moisture");
  JsonArray& demoModeArr = root.createNestedArray("demoMode");
  JsonArray& demoFrequencyeArr = root.createNestedArray("demoFrequency");

  Serial.println("json data");
  //Serial.println(moistureBefore);
  root["vacationMode"] = vacationMode;
  root["useFeritizer"] = useFeritizer;
  root["moistureLowerBound"] = moistureLowerBound;
  root["numberWatersInTank"] = numberWatersInTank;
  root["numberFertilizersInTank"] = numberFertilizersInTank;
  root["currentWatersInTank"] = currentWatersInTank;
  root["currentFertilizersInTank"] = currentFertilizersInTank;
  root["daysBetweenWaters"] = daysBetweenWaters;
  root["numberPumpRunsPerWater"] = numberPumpRunsPerWater;
  root["vacationModeLength"] = vacationModeLength;
  root["demoMode"] = demoMode;
  root["demoFrequency"] = demoFrequency;
  root["temperature"] = temperature;
  root["light"] = light;
  root["moisture"] = moisture;

  Serial.println("Printing JSON Data");
  root.prettyPrintTo(Serial);


  Serial.println("Saving data to string");
  String output;
  root.printTo(output);
  
  return output;
}

