/*
 * File: helperFunc.h
 * Rev:  1.2.1
 * Date: 05/27/2018
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
 *    See planterMain.ino
 */

#include "Planter.h"
#include <FS.h>
#include <WiFiClientSecure.h>

#define fingerprint "05 89 14 F6 C4 D3 4F F5 6B 03 3C 92 7C FD 08 A5 14 82 98 1D"
#define host "web.cecs.pdx.edu"
#define httpsPort 443
#define url "/~kuanju/smartplanter/api/sync/"

extern nvmData sleepMemory;
extern Planter Planter;
extern StaticJsonBuffer<1500> jsonBuffer;
//extern WiFiManager wifiManager;

void saveData();
int readData();
void apConnect(bool rst, int timeout);
void initialize();
void getConfiguration();
void memoryCorrupted();
void wakeup();
bool sendServerUpdatedJSON(bool updateDaysBetweenWaters);
String getTime();
void wificonnect();
