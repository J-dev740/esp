 #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <FS.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
  #include<Servo.h>


  const char* ssid = "jithu";
  const char* password = "12345678";
  const int output=2;
  //String sliderValue = "0";
  //const char* PARAM_INPUT = "value";

//#define enA 9
//#define in1 2
//#define in2 3
//#define enB 10
//#define in3 4
//#define in4 5

//int xAxis, yAxis,pot1,pot2;
//unsigned int  x = 0;
//unsigned int  y = 0;
unsigned long lastReceiveTime = 0;
//unsigned long currentTime = 0;

//int motorSpeedA = 0;
//int motorSpeedB = 0;

//const int trigPin = 4; //set pin variables
//const int echoPin =5 ;

String obj;
int trigState = LOW; //state of trigPin
int interval = 2; // interval in milliseconds at which trigPin turns on
int interval2 = 10; //time in milliseconds at which the distance is printed 
int duration, distance; //variables
unsigned long int previousMillis = 0; //microsecond at which the pin was last writen
unsigned long currentMillis;


//#define button 4

RF24 radio(4,5); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
Servo neck,head;
boolean buttonState = 0;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  short int x = 10;
  short int y = 30;
  short int z = 40;
};

Data_Package data; //Create a variable with the above structure




// Create AsyncWebServer object on port 80
AsyncWebServer server(80);




  void setup()
{
  // Serial port for debugging purposes
//  Serial.begin(115200);

  //pinMode(button, INPUT);
 // pinMode(in1, OUTPUT);
 // pinMode(in2, OUTPUT);
 // pinMode(in3, OUTPUT);
 // pinMode(in4, OUTPUT);
 // neck.attach(0);
  //head.attach(2);

  Serial.begin(115200); //begin serial communication
  radio.begin();
 // radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();




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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index1.html");
    });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", obj.c_str());
  });


  server.begin();
  
  
}

 
void loop()
{
  
 // currentTime=millis();
  //check();
//if ( currentTime - lastReceiveTime > 1000 ) { //if no response for >= 1sec
  //  resetData();// to prevent unwanted behaviour
//  }
  

  if ( radio.available()) {
  //check();
 // lastReceiveTime=currentTime;
      radio.read(&data, sizeof(Data_Package));
          if(millis()-lastReceiveTime>10){
      lastReceiveTime=millis();
  obj=String(data.x)+" "+String(data.y)+" "+String(data.z);
  Serial.println(obj);
    }




  //  xAxis = data.a;
  //  yAxis = data.b;
 
  //place for servo control
  //if ( (currentTime - lastReceiveTime)>=500)
  //{
  //  lastReceiveTime=currentTime;
  //    neck.write(pot1);
  //check();
  //delay(5);
  //    head.write(pot2);
 // delay(5);
 //   } 
                           }
        //                   else Serial.println("connection not found");

//Serial.print(data.a );
//Serial.print(" ");
//Serial.print(data.b);
//Serial.print(" ");
//Serial.print(data.c);
//Serial.print(" ");
//Serial.print(data.d);
//Serial.println(" ");

  
  

}




void resetData() {
  // if no radio connection - Set initial default values
data.x=0;
data.y=0;
data.z=0;
}
