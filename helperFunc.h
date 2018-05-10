/*
 * File: helperFunc.h
 * Rev:  0.3
 * Date: 05/09/2018
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
#include <WiFiClientSecure.h>

extern nvmData sleepMemory;
extern Planter Planter;
extern WiFiManager wifiManager;

void saveData();
int readData();
void apConnect(bool rst);
void initialize();
void getConfiguration();
void memoryCorrupted();
bool sendServerUpdatedJSON();
