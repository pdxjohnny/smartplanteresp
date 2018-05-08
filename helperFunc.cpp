/*
 * File: helperFunc.cpp
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
  Serial.print("sleepMemory.demoMode: ");
  Serial.println(sleepMemory.demoMode);
  Serial.print("sleepMemory.demoFrequency: ");
  Serial.println(sleepMemory.demoFrequency);
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
  json["demoMode"] = sleepMemory.demoMode;
  json["demoFrequency"] = sleepMemory.demoFrequency;

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
  sleepMemory.demoMode = json["demoMode"];
  sleepMemory.demoFrequency = json["demoFrequency"];

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
  bDoneSleeping = false;
}

void goToSleep() {
  saveData();
  printSleepMemory();
  Serial.print("Sleeping for...");
  Serial.println(timerMinute);
  Serial.println("======================================================================================");
  Planter.sleep(timerMinute);
}

void getConfiguration() {
  const size_t bufferSize = JSON_OBJECT_SIZE(12) + 250;
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
  sleepMemory.demoMode = root["demoMode"]; // false
  sleepMemory.demoFrequency = root["demoFrequency"]; // 30 (seconds)

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

  Planter.configure(vacationMode, useFeritizer, moistureLowerBound, vacationModeLength, sleepMemory.demoMode, sleepMemory.demoFrequency);

  // save data into sleep memory
  sleepMemory.vacationMode = vacationMode;
  sleepMemory.useFeritizer = useFeritizer;
  sleepMemory.moistureLowerBound = moistureLowerBound;
  sleepMemory.vacationModeLength = vacationModeLength;
  
  /*
  // 1. Parse a JSON string into DOM.
  Serial.println("Loading Configuration.....");

  const char* json = "{\"configuration\": {\"macAddr\": \"macAddressW/OCols\",\"vacationMode\": \"V\",\"waterStartHour\": \"6\",\"waterPeriod\": \"4\",\"useMiracleGro\": \"0\",\"moistureLowerBound\": \"45\"}}";
  Serial.println("Loading Configuration.....");
  Document d;
  Serial.println("Loading Configuration.....");

  d.Parse(json);

  Serial.println("Loading Configuration.....");
  Serial.println(d["configuration"]["macAddr"].GetString());
  Serial.println(d["configuration"]["vacationMode"].GetBool());
  Serial.println(d["configuration"]["waterStartHour"].GetInt());
  Serial.println(d["configuration"]["waterPeriod"].GetInt());
  Serial.println(d["configuration"]["useMiracleGro"].GetBool());
  Serial.println(d["configuration"]["moistureLowerBound"].GetInt());
  
  //   std::cout << d["project"].GetString() << std::endl;
*/
}

void loadConfiguration() {
  Serial.println("ERROR. This function is no longer in use.");
  /*
  Planter.configure(6, 4, false, 45);
  sleepMemory.waterStartHour = 6;
  sleepMemory.waterFrequency = 4;
  sleepMemory.useMiracleGro = false;
  sleepMemory.moistureLowerBound = 45;
  */
}

void setTimer() {
  getTime();
  timer = calcTime();
  timerHalfHour = timer/30;
  timerMinute = timer%30;

  Serial.print("Timer half-hour: ");
  Serial.println(timerHalfHour);
  Serial.print("Timer minute: ");
  Serial.println(timerMinute);
  
  if(timerMinute == 0) {
    sleepMemory.wakeCount = timerHalfHour - 1;
    timerMinute = 30;
  }
  else {
    sleepMemory.wakeCount = timerHalfHour;
  }
}

void memoryCorrupted() {
  sleepMemory.bFirstTime = true;
  sleepMemory.magicNumber = MAGIC_NUMBER;
  bDoneSleeping = true;

  Serial.println("Memory corrupted");
  apConnect(true);
  getConfiguration();
  //getTime();
  setTimer();
}

void wakeup() {
  sleepMemory.wakeCount -= 1; // decrememnt the sleep counter
  sleepMemory.bFirstTime = false;
  Serial.print("count is ");
  Serial.println(sleepMemory.wakeCount);
  timerMinute = 30;

  
  if(sleepMemory.wakeCount == -1) {
    Serial.println("Watering ");
 
    apConnect(false);
    getConfiguration();
    setTimer();
    // TODO: water the plant here
    Serial.print("sendServerUpdatedJSON: ");
    Serial.println(sendServerUpdatedJSON());
  }
  else if(sleepMemory.wakeCount < -1) {
    Serial.println("ERROR");
    apConnect(true);
    getConfiguration();
    //getTime();
    setTimer();
  }
  else {
    //Serial.println("Sleep longer..."); 
  }
}

int getTime(){
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 13;

  if (!client.connect("128.138.141.172", httpPort)) {
    Serial.println("connection failed");
    return 1;
  }
  
  // This will send the request to the server
  client.print("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 NodeMcu Lua;)\r\n\r\n");
  delay(100);

  while(client.available())
  {
    String line = client.readStringUntil('\r');

    if (line.indexOf("Date") != -1) {
      Serial.print("=====>");
    } 
    else {
      //Serial.println("Line 1");
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
      */
      currentHour = line.substring(16, 18).toInt();
      currentMinute = line.substring(19, 21).toInt();
    }
  }

  return 0;
}

int calcTime() {
  int hour, minute;
  hour = 24 - currentHour;
  if(currentMinute == 0) {
    minute = 0;
  }
  else {
    hour = hour - 1;
    minute = 60 - currentMinute;
  }

  while(hour > 8)
    hour -= 8;

  return hour*60 + minute;
/*
  while(currentHour >= waterStartHour) 
    waterStartHour += waterFrequency; // Make startHour to be the hour of the next watering schedule of the day

  return waterStartHour * 60 - (currentHour * 60 + currentMinute); // return watering timer (in minutes)
*/
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
  String data = Planter.getDataJson();
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
/*
  system_rtc_mem_read(64, &booted, sizeof(booted));
  Serial.begin(9600);
  Serial.print("Count: ");
  Serial.println(booted);
  pinMode(D4, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  // Case 1: Booted but not timer not reached yet
  if(booted == 1) {
    //Serial.begin(9600);
    Serial.println();
    Serial.println("Waking up...");
    booted += 1;
    system_rtc_mem_write(64, &booted, sizeof(booted));
    //blinkLED();
    getTime();
    ESP.deepSleep(600*1000000);  
  }
  // Case 2: Booted and timer reached
  else if(booted < 6) {
    Serial.println();
    Serial.println("Waking up...");
    booted = 0;
    system_rtc_mem_write(64, &booted, sizeof(booted));
    //blinkLED();
    getTime();
    ESP.deepSleep(600*1000000);  
  }
  // Case 3: Not booted before
  else {
    Serial.println();
    Serial.println("normal boot");
    Serial.println("setting up");
    booted = 1;
    system_rtc_mem_write(64, &booted, sizeof(booted));
    //blinkLED();
    getTime();
    ESP.deepSleep(600*1000000);
  }
  Serial.println("Does it reach here?");
}

void blinkLED() {
  for(int i=0; i<2; ++i) {
    digitalWrite(D4, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is active low on the ESP-01)
    delay(500);                      // Wait for a second
    digitalWrite(D4, HIGH);  // Turn the LED off by making the voltage HIGH
    delay(500);                      // Wait for two seconds (to demonstrate the active low LED)
  }
}
*/
