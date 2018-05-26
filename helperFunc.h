/*
 * File: helperFunc.h
 * Rev:  1.1
 * Date: 05/26/2018
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

extern nvmData sleepMemory;
extern Planter Planter;
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
