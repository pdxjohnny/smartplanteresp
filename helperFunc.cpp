/*
 * File: helperFunc.cpp
 * Rev:  1.2.1
 * Date: 05/27/2018
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
  Serial.println(F("========= BEGIN sleepMemory ========="));
  Serial.print(F("sleepMemory.wakeCount: "));
  Serial.println(sleepMemory.wakeCount);
  Serial.print(F("sleepMemory.bFirstTime: "));
  Serial.println(sleepMemory.bFirstTime);
  Serial.print(F("sleepMemory.magicNumber: "));
  Serial.println(sleepMemory.magicNumber);
  Serial.print(F("sleepMemory.vacationMode: "));
  Serial.println(sleepMemory.vacationMode);
  Serial.print(F("sleepMemory.useFeritizer: "));
  Serial.println(sleepMemory.useFeritizer);
  Serial.print(F("sleepMemory.moistureLowerBound: "));
  Serial.println(sleepMemory.moistureLowerBound);
  Serial.print(F("sleepMemory.currentWatersInTank: "));
  Serial.println(sleepMemory.currentWatersInTank);
  Serial.print(F("sleepMemory.currentFertilizersInTank: "));
  Serial.println(sleepMemory.currentFertilizersInTank);
  Serial.print(F("sleepMemory.daysBetweenWaters: "));
  Serial.println(sleepMemory.daysBetweenWaters);
  Serial.print(F("sleepMemory.numberPumpRunsPerWater: "));
  Serial.println(sleepMemory.numberPumpRunsPerWater);
  Serial.print(F("sleepMemory.vacationModeLength: "));
  Serial.println(sleepMemory.vacationModeLength);
  Serial.print(F("sleepMemory.demoMode: "));
  Serial.println(sleepMemory.demoMode);
  Serial.print(F("sleepMemory.demoFrequency: "));
  Serial.println(sleepMemory.demoFrequency);
  Serial.print(F("sleepMemory.token: "));
  Serial.println(sleepMemory.token);
  Serial.println(F("=========  END sleepMemory  ========="));
}

void saveData() {
  Serial.println(F("saveData()"));
  if (!SPIFFS.begin()) {
    Serial.println(F("failed to mount file system"));
    return;
  }

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println(F("error opening config file"));
    return;
  }

  //DynamicJsonBuffer jsonBuffer;
  jsonBuffer.clear();
  JsonObject& json = jsonBuffer.createObject();
  json[F("wakeCount")] = sleepMemory.wakeCount;
  json[F("bFirstTime")] = sleepMemory.bFirstTime;
  json[F("magicNumber")] = sleepMemory.magicNumber;
  json[F("vacationMode")] = sleepMemory.vacationMode;
  json[F("useFeritizer")] = sleepMemory.useFeritizer;
  json[F("moistureLowerBound")] = sleepMemory.moistureLowerBound;
  json[F("currentWatersInTank")] = sleepMemory.currentWatersInTank;
  json[F("currentFertilizersInTank")] = sleepMemory.currentFertilizersInTank;
  json[F("daysBetweenWaters")] = sleepMemory.daysBetweenWaters;
  json[F("numberPumpRunsPerWater")] = sleepMemory.numberPumpRunsPerWater;
  json[F("vacationModeLength")] = sleepMemory.vacationModeLength;
  json[F("token")] = sleepMemory.token;
  json[F("demoMode")] = sleepMemory.demoMode;
  json[F("demoFrequency")] = sleepMemory.demoFrequency;
  json[F("SSID")] = sleepMemory.mySSID;
  json[F("PASS")] = sleepMemory.myPASS;

  Serial.println(F("Saved sleepMemory to /config.json"));
  json.prettyPrintTo(Serial);
  json.printTo(configFile);
  configFile.close();
}

int readData() {
  Serial.println(F("##############################Read Data START##############################"));
  if (!SPIFFS.begin()) {
    Serial.println(F("failed to mount file system"));
    return 0;
  }

  if (!SPIFFS.exists("/config.json")) {
    Serial.println(F("/config.json does not exist"));
    return 0;
  }

  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println(F("error opening config file"));
    return 0;
  }

  size_t size = configFile.size();
  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  configFile.readBytes(buf.get(), size);
  //DynamicJsonBuffer jsonBuffer;
  jsonBuffer.clear();
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  Serial.println(F("Save JSON: "));
  json.prettyPrintTo(Serial); 
  if (!json.success()) {
    Serial.println(F("Failed to parse JSON"));
    return 0;
  }

  sleepMemory.wakeCount = json[F("wakeCount")];
  sleepMemory.bFirstTime = json[F("bFirstTime")];
  sleepMemory.magicNumber = json[F("magicNumber")];
  sleepMemory.vacationMode = json[F("vacationMode")];
  sleepMemory.useFeritizer = json[F("useFeritizer")];
  sleepMemory.moistureLowerBound = json[F("moistureLowerBound")];
  sleepMemory.currentWatersInTank = json[F("currentWatersInTank")];
  sleepMemory.currentFertilizersInTank = json[F("currentFertilizersInTank")];
  sleepMemory.daysBetweenWaters = json[F("daysBetweenWaters")];
  sleepMemory.numberPumpRunsPerWater = json[F("numberPumpRunsPerWater")];
  sleepMemory.vacationModeLength = json[F("vacationModeLength")];
  memset(sleepMemory.token, '\0', 1024);
  strncpy(sleepMemory.token, json[F("token")], 1023);
  sleepMemory.demoMode = json[F("demoMode")];
  sleepMemory.demoFrequency = json["demoFrequency"];
  memset(sleepMemory.mySSID, '\0', 20);
  strncpy(sleepMemory.mySSID, json[F("SSID")], 20);
  memset(sleepMemory.myPASS, '\0', 20);
  strncpy(sleepMemory.myPASS, json[F("PASS")], 20);

  Serial.println();
  Serial.println(F("Loaded sleepMemory from /config.json"));
  printSleepMemory();
  Serial.println(F("##############################Read Data END##############################"));
  return 1;
}

// Connect to AP
void apConnect(bool rst, int timeout) {
  WiFiManager wifiManager;
  if(rst)
    wifiManager.resetSettings();

  char blank_token[1024];
  blank_token[0] = '\0';
  WiFiManagerParameter token_param("t", "token", blank_token, 1024);
  wifiManager.addParameter(&token_param);
  wifiManager.setAPStaticIPConfig(IPAddress(10, 0, 1, 1),
      IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));
  wifiManager.setConfigPortalTimeout(timeout);
  wifiManager.autoConnect("Smart Planter");
  memset(sleepMemory.token, '\0', 1024);
  strncpy(sleepMemory.token, token_param.getValue(), 1023);
  sleepMemory.token[1023] = '\0';
  Serial.print(F("token: "));
  Serial.println(sleepMemory.token);
  //Serial.println("WiFi Connected");
}

void initialize() {
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(F("======================================================================================"));

  readData();
}

void getConfiguration() {
  //const size_t bufferSize = JSON_OBJECT_SIZE(10) + 300; // changed to 500.
  //StaticJsonBuffer<700> jsonBuffer;

  //const char* host = "web.cecs.pdx.edu";
  //const int httpsPort = 443;

  // Use web browser to view and copy
  // SHA1 fingerprint of the certificate
  // TODO Expires On Tuesday, July 24, 2018 at 4:10:28 PM
  //const char* fingerprint = "05 89 14 F6 C4 D3 4F F5 6B 03 3C 92 7C FD 08 A5 14 82 98 1D";

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  //DynamicJsonBuffer jsonBuffer;
  jsonBuffer.clear();
  if (!client.connect(host, httpsPort)) {
    Serial.println(F("connection failed"));
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println(F("certificate matches"));
  } else {
    Serial.println(F("certificate doesn't match"));
    return;
  }

  //String url = "/~kuanju/smartplanter/api/sync/";

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + sleepMemory.token + "\r\n" +
               "Connection: close\r\n\r\n");

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("could not find end of headers"));
    return;
  }
  Serial.println(F("got config from server"));

  // Root of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  JsonObject& root = jsonBuffer.parseObject(client);
  // Check if parsing succeeds.
  if (!root.success()) {
    Serial.println(F("parseObject() failed"));
    return;
  }
  bool vacationMode = root[F("vacationMode")]; // true
  int vacationModeLength = root[F("vacationModeLength")]; // 2
  bool useFeritizer = root[F("useFeritizer")]; // true
  int moistureLowerBound = root[F("moistureLowerBound")]; // 20
  int daysBetweenWaters = root[F("daysBetweenWaters")]; // 7
  int numberWatersInTank = root[F("numberWatersInTank")]; // 16
  int currentWatersInTank = root[F("currentWatersInTank")]; // 16
  int numberPumpRunsPerWater = root[F("numberPumpRunsPerWater")]; // 1
  int numberFertilizersInTank = root[F("numberFertilizersInTank")]; // 8
  int currentFertilizersInTank = root[F("currentFertilizersInTank")]; // 8
  sleepMemory.demoMode = root[F("demoMode")]; // false
  sleepMemory.demoFrequency = root[F("demoFrequency")]; // 30 (seconds)

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();

  Serial.println(F("Parsing JSON configuration"));
  Serial.print(F("vacationMode: "));
  Serial.println(vacationMode);
  Serial.print(F("vacationModeLength: "));
  Serial.println(vacationModeLength);
  Serial.print(F("useFeritizer: "));
  Serial.println(useFeritizer);
  Serial.print(F("moistureLowerBound: "));
  Serial.println(moistureLowerBound);
  Serial.print(F("demoMode: "));
  Serial.println(sleepMemory.demoMode);
  Serial.print(F("demoFrequency: "));
  Serial.println(sleepMemory.demoFrequency);

  client.stop();

  Planter.configure(vacationMode, useFeritizer, moistureLowerBound, vacationModeLength, sleepMemory.demoMode, sleepMemory.demoFrequency);

  // save data into sleep memory
  sleepMemory.vacationMode = vacationMode;
  sleepMemory.useFeritizer = useFeritizer;
  sleepMemory.moistureLowerBound = moistureLowerBound;
  sleepMemory.vacationModeLength = vacationModeLength;

  saveData();
}

void memoryCorrupted() {
  sleepMemory.bFirstTime = true;
  sleepMemory.magicNumber = MAGIC_NUMBER;

  Serial.println(F("Memory corrupted. Setting up new planter"));
  apConnect(true, 1000);
  String mySSID = WiFi.SSID();
  String myPASS = WiFi.psk();
  Serial.print(F("sleepmemory mySSID:"));
  Serial.print(mySSID);
  Serial.print(myPASS);
  mySSID.toCharArray(sleepMemory.mySSID, 20);
  myPASS.toCharArray(sleepMemory.myPASS, 20);
  Serial.print(F("sleepmemory mySSID:"));
  Serial.println(sleepMemory.mySSID);
  Serial.println(sleepMemory.myPASS);
  getConfiguration();
  //saveData();
}

void wakeup() {
  Serial.println(F("Power restored. Getting configuration from server"));
  wificonnect();
  getConfiguration();
  //saveData();
}

// Send JSON data function
// This function gets the JSON string String by calling "Planter.getDataJson();"
bool sendServerUpdatedJSON(bool updateDaysBetweenWaters) {
  //const char* host = "web.cecs.pdx.edu";
  //const int httpsPort = 443;

  // Use web browser to view and copy
  // SHA1 fingerprint of the certificate
  // TODO Expires On Tuesday, July 24, 2018 at 4:10:28 PM
  //const char* fingerprint = "05 89 14 F6 C4 D3 4F F5 6B 03 3C 92 7C FD 08 A5 14 82 98 1D";

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  if (!client.connect(host, httpsPort)) {
    Serial.println(F("sendServerUpdatedJSON: connection failed"));
    return false;
  }
  Serial.println(F("sendServerUpdatedJSON: Connected to server"));

  if (!client.verify(fingerprint, host)) {
    Serial.println(F("sendServerUpdatedJSON: certificate doesn't match"));
    return false;
  }
  Serial.println(F("sendServerUpdatedJSON: Verification OK"));

  //String url = "/~kuanju/smartplanter/api/sync/";

  Serial.println(F("sendServerUpdatedJSON: Planter.getDataJson"));
  String data = Planter.getJsonData(updateDaysBetweenWaters);
  Serial.println(F("sendServerUpdatedJSON: Planter.getDataJson successfull"));
  client.print(String("PUT ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " + data.length() + "\r\n" +
               "Authorization: Bearer " + sleepMemory.token + "\r\n" +
               "Connection: close\r\n\r\n" + data);

  Serial.println(F("Sent request"));
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println(F("headers received"));
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

String getTime(){
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 13;
  String line;

  Serial.println(F("Getting timestamp"));

  if (!client.connect("128.138.141.172", httpPort)) {
    Serial.println(F("connection failed"));
    return "timeStampError";
  }
  
  // This will send the request to the server
  client.print(F("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 NodeMcu Lua;)\r\n\r\n"));
  delay(100);

  int count = 0;
  while(client.available())
  {
    line = client.readStringUntil('\r');

    if (line.indexOf("Date") != -1) {
      //Serial.print("=====>");
    } 
    else {
      //Serial.println("Line 1");
      Serial.print(count);
      Serial.print(line);
      /*
      Serial.print("Year: ");
      Serial.println(line.substring(7, 9));

      Serial.print("Month: ");
      Serial.println(line.substring(10, 12));
      
      Serial.print("Day: ");
      Serial.println(line.substring(13, 15));
      
      Serial.print("Hour: ");
      Serial.println(line.substring(16, 18));
      
      Serial.print("Minute: ");
      Serial.println(line.substring(19, 21));
      
      Serial.print("Second: ");
      Serial.println(line.substring(22, 24));
      
      Serial.println("Done");
      
      currentHour = line.substring(16, 18).toInt();
      currentMinute = line.substring(19, 21).toInt();
      */
    }
    ++count;
  }

  Serial.print(line);
  Serial.println(F("Getting timestamp Done"));

  return line;
}

void wificonnect() {
  int timeoutCnt;
  if(WiFi.status() != WL_CONNECTED) {
    //Serial.println(SSIDArr);
    //Serial.println(PASSArr);
    Serial.print("WiFi is not connected. Attempting to connect");

    timeoutCnt = 0;
    WiFi.persistent(false);
    WiFi.mode(WIFI_OFF);   // this is a temporary line, to be removed after SDK update to 1.5.4
    WiFi.mode(WIFI_STA);
    WiFi.begin(sleepMemory.mySSID, sleepMemory.myPASS);
    
    while(WiFi.status() != WL_CONNECTED && timeoutCnt < 20) {
      Serial.print(F("."));
      //WiFi.begin(SSIDArr, PASSArr);
      delay(500);
      ++timeoutCnt;
    }
    Serial.println("");
    if(WiFi.status() == WL_CONNECTED) {
      Serial.println(F("WiFi Reconnected"));
      Planter.NetworkErrLed.turnOff();
    } else
      Planter.NetworkErrLed.turnOn();
      
  } else {
    Planter.NetworkErrLed.turnOff();
    Serial.println(F("WiFi connection OK"));
  }
  Serial.println(F("WiFi connection test done"));
}
