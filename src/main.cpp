#include <esp32-hal-gpio.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WString.h>

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
        if(str[2] == '0'){digitalWrite(lamp1,LOW);}
        else{digitalWrite(lamp1,HIGH);}
      }
      else if (str[1] == '2'){ //lamp 2
        if(str[2] == '0'){digitalWrite(lamp2,LOW);}
        else{digitalWrite(lamp2,HIGH);}
      }
      else if (str[1] == '3'){
        if(str[2] == '0'){digitalWrite(lamp3,LOW);}
        else{digitalWrite(lamp3,HIGH);}        
      }      
    } 

    else if(str[0] == '4'){ //ultra som
      if(str[1] == '1'){ //tof 1
        Serial.println();
        Serial.print(readTOF()); Serial.println(" Cm");
      }else if(str[1] == '2'){ //tof 2 *adicionar o outro tof*
        Serial.println();
        Serial.print(readTOF()); Serial.println(" Cm");
      }
    

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
