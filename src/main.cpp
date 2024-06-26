#include <esp32-hal-gpio.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WString.h>
#include <JQ6500_Serial.h>
#include "mp3.h"

Servo servo1;
Servo servo2;

JQ6500_Serial mp3 = JQ6500_Serial(Serial2);

const char* ssid = "VORTEX"; // VORTEX | Wokwi-GUEST
const char* password = "pprKN@Fv"; // pprKN@Fv | (no password)
const char* mqtt_server = "broker.hivemq.com"; // MQTT broker

WiFiClient wakiClient;
PubSubClient client(wakiClient);

#define led1   15 //tava no 14
#define led2   12
#define led3   32
#define led4   33

#define lamp1  22
#define lamp2  21
#define lamp3  19

#define door1  18
#define door2   5

#define TRIG   25
#define ECHO   26

// #define button 35 // tava no 26

#define RED_LED 0//26
#define BLUE_LED 0//14
#define GREEN_LED 0//27

#define mp3_busy_pin 4

//overall brightness value for the strip leds
int brightness = 255;

//individual brightness values for the red, green and blue LEDs in the strip
int gBright = 100; 
int rBright = 100;
int bBright = 100;

int fadeSpeed = 10;

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

int mode = 0;
// int tof = readTOF();
int pos1 = 0;
int pos2 = 0;
long lastMsg = 0;
char msg[50];
int value = 0;


int readTOF(){
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    int duration = pulseIn(ECHO, HIGH);

    return duration/58;
}

void TurnOn(){
   
    for (int i=0;i<256; i++){
      
      analogWrite(RED_LED, rBright);
      rBright +=1;
      delay(fadeSpeed);

    }
    
    for (int i=0;i<256; i++){
      
      analogWrite(BLUE_LED, bBright);
      bBright += 1;
      delay(fadeSpeed);
    }  

    for (int i=0;i<256; i++){
      
      analogWrite(GREEN_LED, gBright);
      gBright +=1;
      delay(fadeSpeed);
    }  
}

void hexToRGB(String hex, int& red, int& green, int& blue){
  String subHex = hex.substring(0);
  unsigned long hexValue = 255 - strtoul(subHex.c_str(), NULL, 16);
  red = (hexValue >> 16) & 0xFF;
  green = (hexValue >> 8) & 0xFF;
  blue = hexValue & 0xFF;
}

void setup_wifi() {
  delay(100);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(String topic, byte* payload, unsigned int length) {
  String str;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
     str+=((char)payload[i]);  
  }
    Serial.print(str);
if (topic = "ian_kratos")  /// esp32 subscribe topic   
    Serial.print(" ");

    if(str[0] == '0'){ //leds
      if(str[1] == '1'){ //led 1
        if(str[2] == '0'){digitalWrite(led1,LOW);} //off
        else{digitalWrite(led1,HIGH);} //on
      }

      else if(str[1] == '2'){ //led 2
        if(str[2] == '0'){digitalWrite(led2,LOW);} //off
        else{digitalWrite(led2,HIGH);} //on
      }

      else if(str[1] == '3'){ //led 3
        if(str[2] == '0'){digitalWrite(led3,LOW);} //off
        else{digitalWrite(led3,HIGH);} //on
      }

      else if(str[1] == '4'){ //led 4
        if(str[2] == '0'){digitalWrite(led4,LOW);} //off
        else{digitalWrite(led4,HIGH);} //on
      }
    }

    else if(str[0] == '1'){ //servo 1
      if(str[1] == '1'){ //servo 1 abra a porta
        Serial.println("porta da frente aberta!");
        servo1.write(90); 
      }else if (str[1] == '2'){ //servo 1 feche a porta
        Serial.println("porta da frente fechada!");
        servo1.write(0);
      }
    }
    
    else if (str[0] == '2'){ // servo 2
      if(str[1] == '1'){ //servo 2 abra a porta
        Serial.println("porta de trás aberta!");
        servo2.write(90); 
      }else if (str[1] == '2'){ //servo 2 feche a porta
        Serial.println("porta de trás fechada!");
        servo2.write(0);
      }
    }

    else if(str[0] == '3'){ //lampadas
      if(str[1] == '1'){ //lamp 1
        if(str[2] == '0'){digitalWrite(lamp1,HIGH);}
        else{digitalWrite(lamp1,LOW);}
      }
      else if (str[1] == '2'){ //lamp 2
        if(str[2] == '0'){digitalWrite(lamp2,HIGH);}
        else{digitalWrite(lamp2,LOW);}
      }
      else if (str[1] == '3'){
        if(str[2] == '0'){digitalWrite(lamp3,HIGH);}
        else{digitalWrite(lamp3,LOW);}        
      }      
    } 

    else if(str[0] == '4'){ //ultra som
      if(str[1] == '1'){ //tof 1
        Serial.println();
        Serial.print(readTOF()); Serial.println(" Cm");
      }
    }

    else if(str[0] == '5'){ //led strip
      int r,g,b;
      hexToRGB(str, r, g, b);
      analogWrite(RED_LED,r);
      analogWrite(GREEN_LED,g);
      analogWrite(BLUE_LED,b);
      Serial.println();
    }

    else if(str[0] == '6'){
      mp3.play();
      Serial.println("mp3 playing");
    }
    
 }

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("wakiClient")) {
      Serial.println("connected");
      client.subscribe("ian_kratos");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds"); 
      delay(5000);
    }
  }
}


void setup() {
  pinMode(led1,   OUTPUT);
  pinMode(led2,   OUTPUT);
  pinMode(led3,   OUTPUT);
  pinMode(led4,   OUTPUT);

  pinMode(lamp1,  OUTPUT);
  pinMode(lamp2,  OUTPUT);
  pinMode(lamp3,  OUTPUT);
 
  pinMode(door1,  OUTPUT);
  pinMode(door2,  OUTPUT);
 
  pinMode(TRIG,   OUTPUT);
  pinMode(ECHO,   INPUT);

  pinMode(mp3_busy_pin, INPUT);

  digitalWrite(lamp1, HIGH);
  digitalWrite(lamp2, HIGH);
  digitalWrite(lamp3, HIGH);

  // pinMode(button, INPUT_PULLUP);

  servo1.attach(18);
  servo2.attach(5);

  Serial.begin(115200);
  Serial2.begin(9600);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  mp3.reset();
  mp3.setLoopMode(MP3_LOOP_NONE); // se der erro tente MP3_LOOP_ONE_STOP
  mp3.setVolume(30);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
  delay(100);

}