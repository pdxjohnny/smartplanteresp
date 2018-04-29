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

void saveData() {
  ESP.rtcUserMemoryWrite((uint32_t)RTCMemOffset, (uint32_t*) &sleepMemory, sizeof(sleepMemory));
}

int readData() {
  ESP.rtcUserMemoryRead((uint32_t)RTCMemOffset, (uint32_t*) &sleepMemory, sizeof(sleepMemory));
  return 1;
}

// Connect to AP
void apConnect(bool rst) {
  if(rst)
    wifiManager.resetSettings();

  WiFiManagerParameter token_param("token", "token", sleepMemory.token, 1024);
  wifiManager.addParameter(&token_param);
  wifiManager.setAPStaticIPConfig(IPAddress(10, 0, 1, 1),
      IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));
  // wifiManager.setConfigPortalTimeout(300);
  wifiManager.autoConnect("Smart Planter");
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
  Serial.println("Data is saved");
  Serial.println(sleepMemory.vacationMode);
  Serial.println(sleepMemory.useFeritizer);
  Serial.println(sleepMemory.moistureLowerBound);
  Serial.println(sleepMemory.currentWatersInTank);
  Serial.println(sleepMemory.currentFertilizersInTank);
  Serial.print("Sleeping for...");
  Serial.println(timerMinute);
  Serial.println("======================================================================================");
  Planter.sleep(timerMinute);
}

void getConfiguration() {
  // Old Json
  // "{\"configuration\": {\"macAddr\": \"macAddressW/OCols\",\"vacationMode\": \"V\",\"waterStartHour\": \"6\",\"waterPeriod\": \"4\",\"useMiracleGro\": \"0\",\"moistureLowerBound\": \"45\"}}";

  StaticJsonBuffer<300> jsonBuffer;

  // StaticJsonBuffer allocates memory on the stack, it can be
  // replaced by DynamicJsonBuffer which allocates in the heap.
  //
  // DynamicJsonBuffer  jsonBuffer(200);

  // JSON input string.
  //
  // It's better to use a char[] as shown here.
  // If you use a const char* or a String, ArduinoJson will
  // have to make a copy of the input in the JsonBuffer.
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
  
  const char* host = "web.cecs.pdx.edu";
  const int httpsPort = 443;

  // Use web browser to view and copy
  // SHA1 fingerprint of the certificate
  const char* fingerprint = "FA 94 C4 62 D6 F7 A6 60 AD 43 0F 1B C4 F1 84 85 EE 29 7D A0";

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
    return;
  }

  // Root of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  JsonObject& root = jsonBuffer.parseObject(client);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();
  bool vacationMode = root["vacationMode"];
  bool useFeritizer = root["useFeritizer"];
  int moistureLowerBound = root["moistureLowerBound"];
  int numberWatersInTank = root["numberWatersInTank"];
  int numberFertilizersInTank = root["numberFertilizersInTank"];
  int currentWatersInTank = root["currentWatersInTank"];
  int currentFertilizersInTank = root["currentFertilizersInTank"];
  int daysBetweenWaters = root["daysBetweenWaters"];
  int numberPumpRunsPerWater = root["numberPumpRunsPerWater"];
  int vacationModeLength = root["vacationModeLength"];
  int temperature = root["temperature"];
  int light = root["light"];
  int moisture = root["moisture"];

  Serial.println("Parsing JSON configuration");
  Serial.println(vacationMode);  // read
  Serial.println(useFeritizer);  // read
  Serial.println(moistureLowerBound);  // read
  Serial.println(numberWatersInTank);  // write? (currently assumed it is hard-coded in planter sw)
  Serial.println(numberFertilizersInTank); // write? (currently assumed it is hard-coded in planter sw)
  Serial.println(currentWatersInTank); // write
  Serial.println(currentFertilizersInTank); // write
  Serial.println(daysBetweenWaters);  // write
  Serial.println(numberPumpRunsPerWater);  // write
  Serial.println(vacationModeLength);  // read
  Serial.println(temperature);  // write
  Serial.println(light);  // write
  Serial.println(moisture);  // write

  Planter.configure(vacationMode, useFeritizer, moistureLowerBound, vacationModeLength);

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

  /* Water the plant */
  
  if(sleepMemory.wakeCount == -1) {
    Serial.println("Watering ");
    // connect to internet
    apConnect(false);
    getConfiguration();
    setTimer();
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
