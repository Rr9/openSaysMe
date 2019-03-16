#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>        // Include the mDNS library
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

void connect();
void redirect();
void handleRoot();
void handleNotFound();

IPAddress ip(192, 168, 1, 144);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
const char* ssid = "SSID";
const char* password = "NETWORK PASSWORD";
bool doorOpen = false;
bool lightOn = false;
String webpage = "<form action=\"/DOOR\" method=\"GET\"><input type=\"submit\" value=\"Open/Close\"></form> <form action=\"/LIGHT\" method=\"GET\"><input type=\"submit\" value=\"Lights\"></form>";
const int doorSwitch = 4;
const int lightSwitch = 0;

ESP8266WebServer server(80); 

void setup() {
  //Serial.begin(115200); 
  //Serial.println("Serial started");

  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(doorSwitch, OUTPUT); 
  pinMode(lightSwitch, OUTPUT);  
  digitalWrite(doorSwitch, HIGH);
  digitalWrite(lightSwitch, HIGH);
  
//  Serial.print("Connecting to ");
//  Serial.print(ssid); 
//  Serial.println(" ...");

  int i = 0;
//  Serial.print("MAC: ");
//  Serial.println(WiFi.macAddress());
  connect();
  
//  Serial.println("Connection established!");  
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
//  Serial.println("Connection established!");  
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);     
  server.on("/DOOR", HTTP_GET, handleDoor);  
  server.on("/LIGHT", HTTP_GET, handleLight); 
  server.onNotFound(handleNotFound);

  server.begin();
  //Serial.println("Server started");
  
  digitalWrite(lightSwitch, HIGH);
  digitalWrite(doorSwitch, HIGH);
  
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
   });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
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
  server.handleClient(); 
  ArduinoOTA.handle();
  if(WiFi.status() != WL_CONNECTED){
    int j = 0;
    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
      delay(500);
      Serial.println(++j);
      delay(500);
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
  server.send(200, "text/html", webpage);
}

void handleDoor() {                          
  digitalWrite(doorSwitch,!digitalRead(doorSwitch));
  redirect();
  delay(500);
  digitalWrite(doorSwitch,!digitalRead(doorSwitch));
}

void handleLight() {              
  lightOn = !lightOn;
  digitalWrite(lightSwitch,!digitalRead(lightSwitch));
  delay(50);
  redirect();
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}

void redirect(){
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

