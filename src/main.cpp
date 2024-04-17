#include <esp32-hal-gpio.h>
#include <Arduino.h>
#include <ESP32Servo.h>

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

Servo servo1;
Servo servo2;

int mode =     0;
int tof = readTOF();

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

  servo1.attach(18);
  servo2.attach(5);

  Serial.begin(115200);
}

void loop() {
  switch (mode)
  {
  case 0: //modo normal / vai esperar msg do mqtt ou input do botoes para ligar os atuadores /
    tof = readTOF();

    break;
  case 1: //modo cinema / desliga todas as luzes, tranca todas as portas e o led strip da sala de cinema liga em amarelo /
    tof = readTOF();
    
    break;

  case 2: //modo lockdown / tranca todas as portas liga todas as luzes e pisca os dois led strip em vermelho /
    
    break;
  }
}

int readTOF(){
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  int duration = pulseIn(ECHO, HIGH);

  return duration/58;
}