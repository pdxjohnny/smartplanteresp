/*
 * File: helperFunc.cpp
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

#include "helperFunc.h"

void printSleepMemory() {
  Serial.println("========= BEGIN sleepMemory =========");
  Serial.print("sleepMemory.wakeCount: ");
  Serial.println(sleepMemory.wakeCount);
  Serial.print("sleepMemory.bFirstTime: ");
  Serial.println(sleepMemory.bFirstTime);
  Serial.print("sleepMemory.magicNumber: ");
  Serial.println(sleepMemory.magicNumber);
  Serial.print("sleepMemory.vacationMode: ");
  Serial.println(sleepMemory.vacationMode);
  Serial.print("sleepMemory.useFeritizer: ");
  Serial.println(sleepMemory.useFeritizer);
  Serial.print("sleepMemory.moistureLowerBound: ");
  Serial.println(sleepMemory.moistureLowerBound);
  Serial.print("sleepMemory.currentWatersInTank: ");
  Serial.println(sleepMemory.currentWatersInTank);
  Serial.print("sleepMemory.currentFertilizersInTank: ");
  Serial.println(sleepMemory.currentFertilizersInTank);
  Serial.print("sleepMemory.daysBetweenWaters: ");
  Serial.println(sleepMemory.daysBetweenWaters);
  Serial.print("sleepMemory.numberPumpRunsPerWater: ");
  Serial.println(sleepMemory.numberPumpRunsPerWater);
  Serial.print("sleepMemory.vacationModeLength: ");
  Serial.println(sleepMemory.vacationModeLength);
  Serial.print("sleepMemory.token: ");
  Serial.println(sleepMemory.token);
  Serial.println("=========  END sleepMemory  =========");
}

void saveData() {
  Serial.println("saveData()");
  if (!SPIFFS.begin()) {
    Serial.println("failed to mount file system");
    return;
  }

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("error opening config file");
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["wakeCount"] = sleepMemory.wakeCount;
  json["bFirstTime"] = sleepMemory.bFirstTime;
  json["magicNumber"] = sleepMemory.magicNumber;
  json["vacationMode"] = sleepMemory.vacationMode;
  json["useFeritizer"] = sleepMemory.useFeritizer;
  json["moistureLowerBound"] = sleepMemory.moistureLowerBound;
  json["currentWatersInTank"] = sleepMemory.currentWatersInTank;
  json["currentFertilizersInTank"] = sleepMemory.currentFertilizersInTank;
  json["daysBetweenWaters"] = sleepMemory.daysBetweenWaters;
  json["numberPumpRunsPerWater"] = sleepMemory.numberPumpRunsPerWater;
  json["vacationModeLength"] = sleepMemory.vacationModeLength;
  json["token"] = sleepMemory.token;

  Serial.println("Saved sleepMemory to /config.json");
  json.prettyPrintTo(Serial);
  json.printTo(configFile);
  configFile.close();
}

int readData() {
  Serial.println("readData()");
  if (!SPIFFS.begin()) {
    Serial.println("failed to mount file system");
    return 0;
  }

  if (!SPIFFS.exists("/config.json")) {
    Serial.println("/config.json does not exist");
    return 0;
  }

  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("error opening config file");
    return 0;
  }

  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  configFile.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  json.prettyPrintTo(Serial);
  if (!json.success()) {
    Serial.println("Failed to parse JSON");
    return 0;
  }

  sleepMemory.wakeCount = json["wakeCount"];
  sleepMemory.bFirstTime = json["bFirstTime"];
  sleepMemory.magicNumber = json["magicNumber"];
  sleepMemory.vacationMode = json["vacationMode"];
  sleepMemory.useFeritizer = json["useFeritizer"];
  sleepMemory.moistureLowerBound = json["moistureLowerBound"];
  sleepMemory.currentWatersInTank = json["currentWatersInTank"];
  sleepMemory.currentFertilizersInTank = json["currentFertilizersInTank"];
  sleepMemory.daysBetweenWaters = json["daysBetweenWaters"];
  sleepMemory.numberPumpRunsPerWater = json["numberPumpRunsPerWater"];
  sleepMemory.vacationModeLength = json["vacationModeLength"];
  memset(sleepMemory.token, '\0', 1024);
  strncpy(sleepMemory.token, json["token"], 1023);

  Serial.println("Loaded sleepMemory from /config.json");
  printSleepMemory();
  return 1;
}

// Connect to AP
void apConnect(bool rst) {
  if(rst)
    wifiManager.resetSettings();

  char blank_token[1024];
  blank_token[0] = '\0';
  WiFiManagerParameter token_param("t", "token", blank_token, 1024);
  wifiManager.addParameter(&token_param);
  wifiManager.setAPStaticIPConfig(IPAddress(10, 0, 1, 1),
      IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));
  // wifiManager.setConfigPortalTimeout(300);
  wifiManager.autoConnect("Smart Planter");
  memset(sleepMemory.token, '\0', 1024);
  strncpy(sleepMemory.token, token_param.getValue(), 1023);
  sleepMemory.token[1023] = '\0';
  Serial.print("token: ");
  Serial.println(sleepMemory.token);
  Serial.println("WiFi Connected");
}

void initialize() {
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("======================================================================================");

  // TODO configure planter with saved settings
  readData();
}

void getConfiguration() {
  const size_t bufferSize = JSON_OBJECT_SIZE(10) + 250;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  const char* host = "web.cecs.pdx.edu";
  const int httpsPort = 443;

  // Use web browser to view and copy
  // SHA1 fingerprint of the certificate
  // TODO Expires On Tuesday, July 24, 2018 at 4:10:28 PM
  const char* fingerprint = "05 89 14 F6 C4 D3 4F F5 6B 03 3C 92 7C FD 08 A5 14 82 98 1D";

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
    return;
  }

  String url = "/~jsa3/smartplanter/api/sync/";

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + sleepMemory.token + "\r\n" +
               "Connection: close\r\n\r\n");

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println("could not find end of headers");
    return;
  }

  // Root of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  JsonObject& root = jsonBuffer.parseObject(client);
  // Check if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  bool vacationMode = root["vacationMode"]; // true
  int vacationModeLength = root["vacationModeLength"]; // 2
  bool useFeritizer = root["useFeritizer"]; // true
  int moistureLowerBound = root["moistureLowerBound"]; // 20
  int daysBetweenWaters = root["daysBetweenWaters"]; // 7
  int numberWatersInTank = root["numberWatersInTank"]; // 16
  int currentWatersInTank = root["currentWatersInTank"]; // 16
  int numberPumpRunsPerWater = root["numberPumpRunsPerWater"]; // 1
  int numberFertilizersInTank = root["numberFertilizersInTank"]; // 8
  int currentFertilizersInTank = root["currentFertilizersInTank"]; // 8

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();

  Serial.println("Parsing JSON configuration");
  Serial.println(vacationMode);
  Serial.println(vacationModeLength);
  Serial.println(useFeritizer);
  Serial.println(moistureLowerBound);
  Serial.println(daysBetweenWaters);
  Serial.println(numberWatersInTank);
  Serial.println(currentWatersInTank);
  Serial.println(numberPumpRunsPerWater);
  Serial.println(numberFertilizersInTank);
  Serial.println(currentFertilizersInTank);

  client.stop();

  Planter.configure(vacationMode, useFeritizer, moistureLowerBound, vacationModeLength);

  // save data into sleep memory
  sleepMemory.vacationMode = vacationMode;
  sleepMemory.useFeritizer = useFeritizer;
  sleepMemory.moistureLowerBound = moistureLowerBound;
  sleepMemory.vacationModeLength = vacationModeLength;
}

void memoryCorrupted() {
  sleepMemory.bFirstTime = true;
  sleepMemory.magicNumber = MAGIC_NUMBER;

  Serial.println("Memory corrupted");
  apConnect(true);
  getConfiguration();
}

// Send JSON data function
// This function gets the JSON string String by calling "Planter.getDataJson();"
bool sendServerUpdatedJSON() {
  const char* host = "web.cecs.pdx.edu";
  const int httpsPort = 443;

  // Use web browser to view and copy
  // SHA1 fingerprint of the certificate
  // TODO Expires On Tuesday, July 24, 2018 at 4:10:28 PM
  const char* fingerprint = "05 89 14 F6 C4 D3 4F F5 6B 03 3C 92 7C FD 08 A5 14 82 98 1D";

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  if (!client.connect(host, httpsPort)) {
    Serial.println("sendServerUpdatedJSON: connection failed");
    return false;
  }
  Serial.println("sendServerUpdatedJSON: Connected to server");

  if (!client.verify(fingerprint, host)) {
    Serial.println("sendServerUpdatedJSON: certificate doesn't match");
    return false;
  }
  Serial.println("sendServerUpdatedJSON: Verification OK");

  String url = "/~jsa3/smartplanter/api/sync/";

  Serial.println("sendServerUpdatedJSON: Planter.getDataJson");
  String data = Planter.getJsonData();
  Serial.println("sendServerUpdatedJSON: Planter.getDataJson successfull");
  client.print(String("PUT ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " + data.length() + "\r\n" +
               "Authorization: Bearer " + sleepMemory.token + "\r\n" +
               "Connection: close\r\n\r\n" + data);

  Serial.println("Sent request");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"udpated\":true}")) {
    client.stop();
    return true;
  }
  client.stop();

  return false;
}
