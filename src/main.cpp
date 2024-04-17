#include <esp32-hal-gpio.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <PubSubClient.h>
#include <WiFi.h>

Servo servo1;
Servo servo2;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";// MQTT broker

WiFiClient espClient;
PubSubClient client(espClient);

#define led1   14
#define led2   12
#define led3   32
#define led4   33

#define lamp1  22
#define lamp2  21
#define lamp3  19

#define door1  18
#define door2   5

#define TRIG   17
#define ECHO   16

#define button 26

int mode = 0;
// int tof = readTOF();
int pos1 = 0;
int pos2 = 0;
long lastMsg = 0;
char msg[50];
int value = 0;


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

  pinMode(button, INPUT_PULLUP);

  servo1.attach(18);
  servo2.attach(5);

  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
 delay(100);
}

int readTOF(){
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  int duration = pulseIn(ECHO, HIGH);

  return duration/58;
}

void setup_wifi() {
  delay(10);
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

void callback(char* topic, byte* payload, unsigned int length) {
   String str;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
     str+=((char)payload[i]);  
  }
    Serial.print(str);
if (topic ="ian_kratos")  /// esp32 subscribe topic   
    Serial.print(" ");
    split(str,' ');
    if(str[0] == 1){
      int status = str[1].stoi();   
      int pos = map(status, 1, 100, 0, 180);
      Serial.println(pos);
      servo1.write(pos);
      delay(15);
    }else{
      int status = str[1].stoi();   
      int pos = map(status, 1, 100, 0, 180);
      Serial.println(pos);
      servo2.write(pos);
      delay(15);
    }
     
 }

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESPClient")) {
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

void split (String str, char seperator)  
{  
  int currIndex = 0, i = 0;  
  int startIndex = 0, endIndex = 0;  
  while (i <= len(str)){
    if (str[i] == seperator || i == len(str)){  
      endIndex = i;  
      String subStr = "";  
      subStr.append(str, startIndex, endIndex - startIndex);  
      strings[currIndex] = subStr;  
      currIndex += 1;  
      startIndex = endIndex + 1;  
    }  
    i++;  
  }     
}  