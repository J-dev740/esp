/* Get tilt angles on X and Y, and rotation angle on Z
 * Angles are given in degrees, displays on SSD1306 OLED
 * 
 * License: MIT
 */

 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_light.h>



#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define Pi  3.14159
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
MPU6050 mpu(Wire);
unsigned long timer = 0;
const int trigPin = 4; //set pin variables
const int echoPin =5 ;
String obj;




int trigState = LOW; //state of trigPin
int interval = 2; // interval in milliseconds at which trigPin turns on
int interval2 = 10; //time in milliseconds at which the distance is printed 
int duration, distance; //variables
unsigned long previousMillis = 0; //microsecond at which the pin was last writen
unsigned long currentMillis;



RF24 radio(7, 8); // CE, CSN 13(sclk) 11(mosi) 12(miso)
const byte addresses[][6] = {"00001", "00002"};
boolean buttonState = 0;
int potxValue,potyValue,neck,head;
//Servo neck,head;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
 short int a = 0;
 short int b = 0;
 short int c = 0;
};

Data_Package data; //Create a variable with the above structure






void setup() {
  Serial.begin(115200);                           // Ensure serial monitor set to this value also    

  pinMode(trigPin,OUTPUT); //set pinmodes
  pinMode(echoPin,INPUT);

  
  digitalWrite(trigPin,LOW);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))  // Address 0x3C for most of these displays, if doesn't work try 0x3D 
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);                                      // Don't proceed, loop forever
  } 
  display.setTextSize(1);             
  display.setTextColor(SSD1306_WHITE);            // Draw white text
  display.clearDisplay();                         
  Wire.begin();
  mpu.begin();
  display.println(F("Calculating gyro offset, do not move MPU6050"));
  display.display();        
  mpu.calcGyroOffsets();                          // This does the calibration
  display.setTextSize(2); 



 //Serial.begin(115200);
  //pinMode(12, OUTPUT);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
 // radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

}

void loop() {
  check();



  
  mpu.update();  
  if((millis()-timer)>10)                         // print data every 10ms
  {   

  data.c = distance*sin(mpu.getAngleY()*Pi/180);//z
  data.a = distance*cos(mpu.getAngleY()*Pi/180)*cos(mpu.getAngleZ()*Pi/180);//x
  data.b = distance*cos(mpu.getAngleY()*Pi/180)*sin(mpu.getAngleZ()*Pi/180);//y


    display.clearDisplay();                       // clear screen
    display.setCursor(0,0);                         
    display.print("P : ");
    display.println(mpu.getAngleY());//gives vertical inclination w.r.to horizontal plane
    display.print("D : ");
    display.println(distance);
    display.print("Y : ");
    display.print(mpu.getAngleZ());//gives the horizontal inclination
    display.display();                            // display data
    timer = millis(); 

     radio.write(&data, sizeof(Data_Package));
       obj=String(data.a)+" "+String(data.b)+" "+String(data.c);
  Serial.println(obj);
    check(); 
  }
  //Serial.println(distance);
}


void check()
  { 
  currentMillis = micros(); //time in microseconds from which the code was started
  
  if (trigState==HIGH &&currentMillis-previousMillis >= interval) 
    { 
    previousMillis = currentMillis;
    trigState=LOW;
    digitalWrite(trigPin,trigState);
    duration = pulseIn(echoPin,HIGH);
    distance = (duration/2) / 29.1;
    
    if(distance <4) distance =2;
  
    }

  if (trigState==LOW && currentMillis-previousMillis >= interval2)
    {
  previousMillis = currentMillis;
  trigState=HIGH;
  digitalWrite(trigPin,trigState);
    }
  }






  
