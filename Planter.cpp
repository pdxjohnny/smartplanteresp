/*
 * File: Planter.cpp
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
 *    See planterMain.ino
 */

#include "Planter.h"

Planter::Planter() {
  vacationMode = false;
  useFeritizer = true;
  moistureLowerBound = DEFAULT_LOWER_BOUND;

  numberWatersInTank = WATER_TANK_CAP;
  numberFertilizersInTank = FERTILIZER_TANK_CAP;

  currentWatersInTank = WATER_TANK_CAP;
  currentFertilizersInTank = FERTILIZER_TANK_CAP;

  daysBetweenWaters = -1;
  numberPumpRunsPerWater = -1;
  vacationModeLength = -1;

  temperature = -1;
  light = -1;
  moisture = -1;

  demoMode = false;
  demoFrequency = 30;

  moistureError = false;

  daysBetweenWatersCounter = -1;
  waterLvlLow = false;
  fertilizerLvlLow = false;
  prev_water = "NODATA";
}

int Planter::configure(bool vacationModeIn, bool useFeritizerIn, int moistureLowerBoundIn, int vacationModeLengthIn, bool demoModeIn, int demoFrequencyIn) {
  if(moistureLowerBoundIn < 0 || moistureLowerBoundIn > 100) 
    return -1;

  vacationMode = vacationModeIn;
  useFeritizer = useFeritizerIn;
  moistureLowerBound = moistureLowerBoundIn;
  vacationModeLength = vacationModeLengthIn;
  demoMode = demoModeIn;
  demoFrequency = demoFrequencyIn;

  return 0;
}

String Planter::getJsonData(bool updateDaysBetweenWaters) {
  if(updateDaysBetweenWaters)
    daysBetweenWaters = daysBetweenWatersCounter/48.0;

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
  JsonArray& moistureErrorArr = root.createNestedArray("moistureError");
  JsonArray& timeStamp = root.createNestedArray("timeStamp");
  
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
  root["demoMode"] = demoMode;
  root["demoFrequency"] = demoFrequency;
  root["temperature"] = temperature;
  root["light"] = light;
  root["moisture"] = moisture;
  root["moistureError"] = moistureError;

  if(updateDaysBetweenWaters)
    prev_water = getTime();
  
  root["timeStamp"] = prev_water;
  Serial.println("Printing JSON Data");
  root.prettyPrintTo(Serial);

  // Store Json data to a string
  String output;
  root.printTo(output);

  if(updateDaysBetweenWaters)
    daysBetweenWatersCounter = 0;
  
  return output;
}

int Planter::water() {
  int ret = 0;
  int localMoistureLowerBound;
  bool waterLvl, fertilizerLvl;

  Serial.println("******************************Water Start******************************");
  /* Diagnostic */
  if(moisture == MoistureSensor.getMoisture() && MoistureSensor.getMoisture() == 0) {
    moistureError = true;
    Serial.println("ERROR: Moisture sensor");
    Serial.println("******************************Water Exit******************************");
    /* currentWatersInTank & currentFertilizersInTank Reset */
    // Reset waters in tank if water level was low and now it's high
    if(waterLvlLow && WaterLevelSensor.waterPresent()) {
      currentWatersInTank = WATER_TANK_CAP;
      Serial.println("INFO: currentWatersInTank reset");
    }
    
    if(fertilizerLvlLow && FertilizerLevelSensor.waterPresent()) {
      currentFertilizersInTank = FERTILIZER_TANK_CAP;
      Serial.println("INFO: currentFertilizersInTank reset");
    }
    return -1;
  } else {
    moistureError = false;
  }

  /* currentWatersInTank & currentFertilizersInTank Reset */
  // Reset waters in tank if water level was low and now it's high
  if(waterLvlLow && WaterLevelSensor.waterPresent()) {
    currentWatersInTank = WATER_TANK_CAP;
    Serial.println("INFO: currentWatersInTank reset");
  }
    
  if(fertilizerLvlLow && FertilizerLevelSensor.waterPresent()) {
    currentFertilizersInTank = FERTILIZER_TANK_CAP;
    Serial.println("INFO: currentFertilizersInTank reset");
  }

  /* Adjust moisture lower bound */
  if(vacationMode) {
    Serial.println("INFO: Vacation mode");
    if(daysBetweenWaters == -1 || daysBetweenWaters == 0) {// No data
      Serial.println("ERROR: daysBetweenWaters not valid.");
      localMoistureLowerBound = moistureLowerBound * 0.8;
    }
    else {
      Serial.println("INFO: Updating localMoistureLowerBound...");
      Serial.print("INFO: Vaction Mode Length: ");
      Serial.print(vacationModeLength);
      Serial.print("; Days between waters: ");
      Serial.print(daysBetweenWaters);
      Serial.print("; Current waters in tank: ");
      Serial.println(currentWatersInTank);
      if(vacationModeLength*7/daysBetweenWaters > currentWatersInTank) { // Waters needed > Waters Remained
        Serial.println("INFO: Waters needed > Waters Remained");
        // currentWatersInTank/vacationModeLength/7 - daysBetweenWaters = minDaysBetweenWaters - daysBetweenWaters
        localMoistureLowerBound = moistureLowerBound - moistureLowerBound/2 * (currentWatersInTank/vacationModeLength/7 - daysBetweenWaters);
      } else {
        Serial.println("INFO: Waters needed < Waters Remained. No need to adjust bound");
        localMoistureLowerBound = moistureLowerBound;
      }
    }
  } else {
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

  daysBetweenWatersCounter += 1;

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
      currentWatersInTank -= 1;
      ret = 1;
      Serial.println("Done");

      // Only attempt to fertilize if there was enough water
      if(FertilizerLevelSensor.waterPresent()) {
        Serial.print("INFO: Fertilizer Lvl OK: fertilizing... ");
        Serial.print(fertilizersToUseArr[fertilizerPtr]);
        Serial.println(" fertilizers");
        for(int i=0; i<fertilizersToUseArr[fertilizerPtr] && FertilizerLevelSensor.waterPresent(); ++i) {
          FertilizerPump.pumpWater();
          currentFertilizersInTank -= 1;
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

  /* Update sleep memory and water levels */
  waterLvlLow = !WaterLevelSensor.waterPresent();
  fertilizerLvlLow = !FertilizerLevelSensor.waterPresent();
  
  if(WaterLevelSensor.waterPresent());
    //WaterLvlLed.turnOff();
  else {
    Serial.println("WARNING: Water Lvl Low (after watering, if applicable)");
    sleepMemory.currentWatersInTank = 0;
    currentWatersInTank = 0;
    //WaterLvlLed.turnOn();
  }

  if(FertilizerLevelSensor.waterPresent());
    //FertilizerLvlLed.turnOff();
  else {
    Serial.println("WARNING: Fertilizer Lvl Low (after watering, if applicable)");
    sleepMemory.currentFertilizersInTank = 0;
    currentFertilizersInTank = 0;
    //FertilizerLvlLed.turnOn();
  }
  
  Serial.println("******************************Water End******************************");
  return ret;
}


