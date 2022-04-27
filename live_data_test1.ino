 #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <FS.h>



const char* ssid = "jithu";
const char* password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


String random_fun() {

  float t = random(25,46);
  Serial.println(t);
  return String(t);
}








  void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", random_fun().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
