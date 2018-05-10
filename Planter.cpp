/*
 * File: Planter.cpp
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
 *    See planterMain.ino
 */

#include "Planter.h"

Planter::Planter() {
  vacationMode = false;
  useFeritizer = true;
  moistureLowerBound = DEFAULT_LOWER_BOUND;

  numberWatersInTank = WATER_TANK_CAP;
  numberFertilizersInTank = FERTILIZER_TANK_CAP;

  // TODO: When should this reset?
  currentWatersInTank = -1;
  currentFertilizersInTank = -1;

  daysBetweenWaters = -1;
  numberPumpRunsPerWater = -1;
  vacationModeLength = -1;

  temperature = -1;
  light = -1;
  moisture = -1;
}

int Planter::configure(bool vacationModeIn, bool useFeritizerIn, int moistureLowerBoundIn, int vacationModeLengthIn) {
  if(moistureLowerBoundIn < 0 || moistureLowerBoundIn > 100) 
    return -1;

  vacationMode = vacationModeIn;
  useFeritizer = useFeritizerIn;
  moistureLowerBound = moistureLowerBoundIn;
  vacationModeLength = vacationModeLengthIn;

  return 0;
}

String Planter::getJsonData() {
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

  Serial.println("json data");
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
  root["temperature"] = temperature;
  root["light"] = light;
  root["moisture"] = moisture;

  Serial.println("Printing JSON Data");
  root.prettyPrintTo(Serial);

  // Store Json data to a string
  String output;
  root.printTo(output);
  
  return output;
}

bool Planter::water() {
  bool ret = 0;
  int localMoistureLowerBound;
  bool waterLvl, fertilizerLvl;

  Serial.println("******************************Water Start******************************");
  if(vacationMode) {
    Serial.println("INFO: Vacation mode");
    localMoistureLowerBound = moistureLowerBound * 0.8;
  }
  else {
    Serial.println("INFO: Regular mode");
    localMoistureLowerBound = moistureLowerBound;
  }
  
  /* read sensors */
  light = LightSensor.getIntensity();
  moisture = MoistureSensor.getMoisture();
  // temperature
  Serial.print("INFO: Sensors - light: ");
  Serial.print(light);
  Serial.print(" moisture: ");
  Serial.print(moisture);
  Serial.println("%");

  daysBetweenWaters += 1; // TODO: currently implemented as 30 minutes not days

  /* See if watering is needed */
  Serial.print("INFO: Moisture - Current: ");
  Serial.print(moisture);
  Serial.print(" Lower Bound: ");
  Serial.println(localMoistureLowerBound);
  
  if(moisture < localMoistureLowerBound) {
    Serial.println("INFO: Water required");
    if(WaterLevelSensor.waterPresent()) {
      Serial.print("INFO: Water Lvl OK: watering... ");
      WaterPump.pumpWater();
      numberPumpRunsPerWater = 1;
      ret = 1;
      Serial.println("Done");

      // Only attempt to fertilize if there was enough water
      if(FertilizerLevelSensor.waterPresent()) {
        Serial.print("INFO: Fertilizer Lvl OK: fertilizing... ");
        Serial.print(fertilizersToUseArr[fertilizerPtr]);
        Serial.println(" fertilizers");
        for(int i=0; i<fertilizersToUseArr[fertilizerPtr]; ++i) {
          FertilizerPump.pumpWater();
        }
        
        fertilizerPtr += 1;
        if(fertilizerPtr >= 6)
          fertilizerPtr = 0;

        Serial.print("INFO: Updated fertilizerPtr = ");
        Serial.println(fertilizerPtr);

        Serial.print("INFO: Next fertilizersToUse = ");
        Serial.println(fertilizersToUseArr[fertilizerPtr]);
          
        Serial.println("INFO: Fertilizing done");
      }
      else {
        Serial.println("WARNING: Fertilizer Lvl Low");
      }
    }
    else {
      Serial.println("WARNING: Water Lvl Low");
      currentWatersInTank = 0;
    }
  }
  else {
    numberPumpRunsPerWater = 0;
  }

  /* Update LEDs & sleep memory */
  if(WaterLevelSensor.waterPresent())
    WaterLvlLed.turnOff();
  else {
    Serial.println("WARNING: Water Lvl Low (after watering, if applicable)");
    sleepMemory.currentWatersInTank = 0;
    WaterLvlLed.turnOn();
  }

  if(FertilizerLevelSensor.waterPresent())
    FertilizerLvlLed.turnOff();
  else {
    Serial.println("WARNING: Fertilizer Lvl Low (after watering, if applicable)");
    sleepMemory.currentFertilizersInTank = 0;
    FertilizerLvlLed.turnOn();
  }
  
  Serial.println("******************************Water End******************************");
  return ret;
}
