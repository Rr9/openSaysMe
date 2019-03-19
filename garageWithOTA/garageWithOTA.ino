#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>        // Include the mDNS library
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FS.h>

void connect();
void redirect();
void handleRoot();
void handleNotFound();
void servePage();
void apiStatus();
void calibrate();
void restartESP();
void updateESP();

IPAddress ip(192, 168, 1, 144);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
const char* ssid = "Sikdar";
const char* password = "suvrajit";
bool doorOpen = false;
bool lightOn = false;
const int doorSwitch = 4;
const int lightSwitch = 0;

bool otaFlag = true;
uint16_t timeElapsed = 0;

ESP8266WebServer server(80); 

void setup() {
  //Serial.begin(115200); 

  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(doorSwitch, OUTPUT); 
  pinMode(lightSwitch, OUTPUT);  
  digitalWrite(doorSwitch, HIGH);
  digitalWrite(lightSwitch, HIGH);
  
//  Serial.print("Connecting to ");
//  Serial.print(ssid); 

  int i = 0;
  connect();
   
//  Serial.print("MAC: ");
//  Serial.println(WiFi.macAddress());
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());

  SPIFFS.begin();

  
  server.on("/", HTTP_GET, handleRoot);     
  server.on("/DOOR", HTTP_GET, handleDoor);  
  server.on("/LIGHT", HTTP_GET, handleLight); 
  server.on("/status", HTTP_GET, apiStatus); 
  server.on("/calibrate", HTTP_GET, calibrate); 
  server.on("/restart", HTTP_GET, restartESP); 
  server.on("/update", HTTP_GET, updateESP); 
  server.onNotFound(handleNotFound);

  server.begin();
  //Serial.println("Server started");
  
  digitalWrite(lightSwitch, HIGH);
  digitalWrite(doorSwitch, HIGH);
  

  //ArduinoOTA.setPassword("admin");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nUpdate Ended");
   });
   
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
   });
   
  ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
   });
  
}

void loop() {
  if(otaFlag){
    uint16_t timeStart = millis();
    while(timeElapsed < 30000){
      ArduinoOTA.handle();
      timeElapsed = millis()-timeStart;
      delay(10);
    }
    otaFlag = false;
  }
  server.handleClient(); 
  
  if(WiFi.status() != WL_CONNECTED){
    int j = 0;
      while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }
  }
}

void connect(){
  int i = 0;

  WiFi.config(ip, gateway, subnet);
  delay(500);
  WiFi.disconnect(true);
  delay(1000);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    //Serial.println(++i);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  }
    //MDNS.begin("garage");
  
//  Serial.println("Connection established!");  
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
}

void handleRoot() {  
  File webpageFile = SPIFFS.open("/index.html", "r");
  server.streamFile(webpageFile, "text/html");
  //server.send(200, "text/html", webpageFile);
  webpageFile.close();
}

void handleDoor() {                          
  digitalWrite(doorSwitch,!digitalRead(doorSwitch));
  doorOpen = !doorOpen;
  if(server.arg("api").toInt()!=1){
    redirect();
  }else{
    server.send(200,"text/plain", "Done");
  }
  delay(200);
  digitalWrite(doorSwitch,!digitalRead(doorSwitch));
}

void handleLight() {              
  lightOn = !lightOn;
  digitalWrite(lightSwitch,!digitalRead(lightSwitch));
  delay(50);
  doorOpen = !doorOpen;
  if(server.arg("api").toInt()!=1){
    redirect();
  }else{
    server.send(200,"text/plain", "Done"); 
  }
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}

void redirect(){
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void apiStatus(){
  server.send(200,"text/plain", String(doorOpen) + "," + String(lightOn));
}

void calibrate(){
  doorOpen = false;
  lightOn = false;
  server.send(200,"text/plain", String(doorOpen) + "," + String(lightOn));
}

void restartESP(){
    server.send(200,"text/plain", "Restarting...");
    delay(1000);
    ESP.restart();
}

void updateESP(){
    server.send(200,"text/plain", "Update mode...");
    otaFlag = true;
    timeElapsed = 0;
}
