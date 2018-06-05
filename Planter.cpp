/*
 * File: Planter.cpp
 * Rev:  1.2.4
 * Date: 06/02/2018
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

  daysBetweenWatersCounter = 0;
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

  //StaticJsonBuffer<500> jsonBuffer;
  //DynamicJsonBuffer jsonBuffer;
  jsonBuffer.clear();
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& vacationModeArr = root.createNestedArray(F("vacationMode"));
  JsonArray& useFeritizerArr = root.createNestedArray(F("useFeritizer"));
  JsonArray& moistureLowerBoundArr = root.createNestedArray(F("moistureLowerBound"));
  JsonArray& numberWatersInTankArr = root.createNestedArray(F("numberWatersInTank"));
  JsonArray& numberFertilizersInTankArr = root.createNestedArray(F("numberFertilizersInTank"));
  JsonArray& currentWatersInTankArr = root.createNestedArray(F("currentWatersInTank"));
  JsonArray& currentFertilizersInTankArr = root.createNestedArray(F("currentFertilizersInTank"));
  JsonArray& daysBetweenWatersArr = root.createNestedArray(F("daysBetweenWaters"));
  JsonArray& numberFertilizersUsedArr = root.createNestedArray(F("numberFertilizersUsed"));
  JsonArray& vacationModeLengthArr = root.createNestedArray(F("vacationModeLength"));
  JsonArray& temperatureArr = root.createNestedArray(F("temperature"));
  JsonArray& lightArr = root.createNestedArray(F("light"));
  JsonArray& moistureArr = root.createNestedArray(F("moisture"));
  JsonArray& demoModeArr = root.createNestedArray(F("demoMode"));
  JsonArray& demoFrequencyeArr = root.createNestedArray(F("demoFrequency"));
  JsonArray& moistureErrorArr = root.createNestedArray(F("moistureError"));
  JsonArray& timeStamp = root.createNestedArray(F("timeStamp"));
  
  Serial.println(F("json data"));
  root[F("vacationMode")] = vacationMode;
  root[F("useFeritizer")] = useFeritizer;
  root[F("moistureLowerBound")] = moistureLowerBound;
  root[F("numberWatersInTank")] = numberWatersInTank;
  root[F("numberFertilizersInTank")] = numberFertilizersInTank;
  root[F("currentWatersInTank")] = currentWatersInTank;
  root[F("currentFertilizersInTank")] = currentFertilizersInTank;
  root[F("daysBetweenWaters")] = daysBetweenWaters;
  root[F("numberPumpRunsPerWater")] = numberPumpRunsPerWater;
  root[F("vacationModeLength")] = vacationModeLength;
  root[F("demoMode")] = demoMode;
  root[F("demoFrequency")] = demoFrequency;
  root[F("temperature")] = temperature;
  root[F("light")] = light;
  root[F("moisture")] = moisture;
  root[F("moistureError")] = moistureError;

  if(updateDaysBetweenWaters)
    prev_water = getTime();
  
  root["timeStamp"] = prev_water;
  Serial.println(F("Printing JSON Data"));
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

  Serial.println(F("******************************Water Start******************************"));
  /* Diagnostic */
  if(moisture == MoistureSensor.getMoisture() && MoistureSensor.getMoisture() == 0) {
    moistureError = true;
    light = LightSensor.getIntensity();
    Serial.println(F("ERROR: Moisture sensor"));
    Serial.println(F("******************************Water Exit******************************"));
    /* currentWatersInTank & currentFertilizersInTank Reset */
    // Reset waters in tank if water level was low and now it's high
    if(waterLvlLow && WaterLevelSensor.waterPresent()) {
      currentWatersInTank = WATER_TANK_CAP;
      Serial.println(F("INFO: currentWatersInTank reset"));
    } else if(!WaterLevelSensor.waterPresent()) {
      currentWatersInTank = 0;
      waterLvlLow = true;
    }
    
    if(fertilizerLvlLow && FertilizerLevelSensor.waterPresent()) {
      currentFertilizersInTank = FERTILIZER_TANK_CAP;
      Serial.println(F("INFO: currentFertilizersInTank reset"));
    } else if(!FertilizerLevelSensor.waterPresent()) {
      currentFertilizersInTank = 0;
      fertilizerLvlLow = true;
    }
    return -1;
  } else {
    moistureError = false;
  }

  /* currentWatersInTank & currentFertilizersInTank Reset */
  // Reset waters in tank if water level was low and now it's high
  if(waterLvlLow && WaterLevelSensor.waterPresent()) {
    currentWatersInTank = WATER_TANK_CAP;
    Serial.println(F("INFO: currentWatersInTank reset"));
  }
    
  if(fertilizerLvlLow && FertilizerLevelSensor.waterPresent()) {
    currentFertilizersInTank = FERTILIZER_TANK_CAP;
    Serial.println(F("INFO: currentFertilizersInTank reset"));
  }

  /* Adjust moisture lower bound */
  if(vacationMode) {
    Serial.println(F("INFO: Vacation mode"));
    if(daysBetweenWaters == -1 || daysBetweenWaters == 0) {// No data
      Serial.println(F("ERROR: daysBetweenWaters not valid."));
      localMoistureLowerBound = moistureLowerBound * 0.8;
    }
    else {
      Serial.println(F("INFO: Updating localMoistureLowerBound..."));
      Serial.print(F("INFO: Vaction Mode Length: "));
      Serial.print(vacationModeLength);
      Serial.print(F("; Days between waters: "));
      Serial.print(daysBetweenWaters);
      Serial.print(F("; Current waters in tank: "));
      Serial.println(currentWatersInTank);
      if(vacationModeLength*7/daysBetweenWaters > currentWatersInTank) { // Waters needed > Waters Remained
        Serial.println(F("INFO: Waters needed > Waters Remained"));
        // currentWatersInTank/vacationModeLength/7 - daysBetweenWaters = minDaysBetweenWaters - daysBetweenWaters
        localMoistureLowerBound = moistureLowerBound - moistureLowerBound/2 * (currentWatersInTank/vacationModeLength/7 - daysBetweenWaters);
        if(localMoistureLowerBound < moistureLowerBound/2)
          localMoistureLowerBound = moistureLowerBound/2;
        else if(localMoistureLowerBound > moistureLowerBound)
          localMoistureLowerBound = moistureLowerBound;
      } else {
        Serial.println(F("INFO: Waters needed < Waters Remained. No need to adjust bound"));
        localMoistureLowerBound = moistureLowerBound;
      }
    }
  } else {
    Serial.println(F("INFO: Regular mode"));
    localMoistureLowerBound = moistureLowerBound;
  }
  
  /* read sensors */
  light = LightSensor.getIntensity();
  moisture = MoistureSensor.getMoisture();
  // temperature
  Serial.print(F("INFO: Sensors - light: "));
  Serial.print(light);
  Serial.print(F(" moisture: "));
  Serial.print(moisture);
  Serial.println(F("%"));

  daysBetweenWatersCounter += 1;

  /* See if watering is needed */
  Serial.print(F("INFO: Moisture - Current: "));
  Serial.print(moisture);
  Serial.print(F(" Lower Bound: "));
  Serial.println(localMoistureLowerBound);
  
  if(moisture < localMoistureLowerBound) {
    Serial.println(F("INFO: Water required"));
    if(WaterLevelSensor.waterPresent()) {
      Serial.print(F("INFO: Water Lvl OK: watering... "));
      WaterPump.pumpWater();
      numberPumpRunsPerWater = 1;
      currentWatersInTank -= 1;
      ret = 1;
      Serial.println(F("Done"));

      // Only attempt to fertilize if there was enough water
      if(FertilizerLevelSensor.waterPresent()) {
        Serial.print(F("INFO: Fertilizer Lvl OK: fertilizing... "));
        Serial.print(fertilizersToUseArr[fertilizerPtr]);
        Serial.println(F(" fertilizers"));
        for(int i=0; i<fertilizersToUseArr[fertilizerPtr] && FertilizerLevelSensor.waterPresent(); ++i) {
          FertilizerPump.pumpWater();
          currentFertilizersInTank -= 1;
        }
        
        fertilizerPtr += 1;
        if(fertilizerPtr >= 6)
          fertilizerPtr = 0;

        Serial.print(F("INFO: Updated fertilizerPtr = "));
        Serial.println(fertilizerPtr);

        Serial.print(F("INFO: Next fertilizersToUse = "));
        Serial.println(fertilizersToUseArr[fertilizerPtr]);
          
        Serial.println(F("INFO: Fertilizing done"));
      }
      else {
        Serial.println(F("WARNING: Fertilizer Lvl Low"));
      }
    }
    else {
      Serial.println(F("WARNING: Water Lvl Low"));
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
    Serial.println(F("WARNING: Water Lvl Low (after watering, if applicable)"));
    sleepMemory.currentWatersInTank = 0;
    currentWatersInTank = 0;
    //WaterLvlLed.turnOn();
  }

  if(FertilizerLevelSensor.waterPresent());
    //FertilizerLvlLed.turnOff();
  else {
    Serial.println(F("WARNING: Fertilizer Lvl Low (after watering, if applicable)"));
    sleepMemory.currentFertilizersInTank = 0;
    currentFertilizersInTank = 0;
    //FertilizerLvlLed.turnOn();
  }
  
  Serial.println(F("******************************Water End******************************"));
  return ret;
}


