/*
 * File: helperFunc.h
 * Rev:  0.2
 * Date: 04/21/2018
 * Description: 
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

#include <FS.h>

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

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
} nvmData;

extern nvmData sleepMemory;
extern Planter Planter;
extern WiFiManager wifiManager;

extern bool bDoneSleeping;
extern int timer;
extern int timerHalfHour;
extern int timerMinute;

extern int currentHour;
extern int currentMinute;

void saveData();
int readData();
void apConnect(bool rst);
void initialize();
void goToSleep();
void getConfiguration();
void loadConfiguration();
void setTimer();
void memoryCorrupted();
void wakeup();
int getTime();
int calcTime();
void water();
bool sendServerUpdatedJSON();
