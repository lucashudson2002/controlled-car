/*
Program: Controlled Car (Autonomous, Bluetooth, Infrared, Radio)
Author: Lucas Dias Hudson
Date: July 13, 2022
GitHub: https://github.com/lucashudson2002/Controlled-Car
*/

//**LIBRARIES**
#include <Servo.h>
#include <HCSR04.h>
#include <TB6612FNG.h>
#include <RF24.h>
#include <SPI.h>
#include <IRremote.h>

//**PINOUT**
//HCSR04 + SERVO
#define TRIG A3
#define ECHO A4
#define SERVO A6
//PONTE-H
#define STBY 2
#define IN1A 3
#define IN2A 4
#define PWMA 5
#define PWMB 6
#define IN1B 7
#define IN2B 8
//INFRARED
#define IR A5        
//NRF24L01
#define CE 9
#define CSN 10
#define MOSI 11
#define MISO 12
#define SCK 13
//BLUETOOTH
#define TX 0
#define RX 1
//BATERIA
#define BUZZER A0
#define LED A1
#define BATTERY A7

//**OTHERS DEFINE**
#define DIST_MIN_CM 30
#define AUTONOMOUS 1
#define BLUETOOTH 2
#define INFRARED 3
#define RADIO 4
#define VOLTAGE_MIN 7 //testar se essa é a mínima
#define VOLTAGE_MAX 11.1 //3 lítios de 3.7V, na realidade é essa?
#define U_TURN 1000 //testar

//**VARIABLES**
TB6612FNG robot(IN1B, IN2B, IN1A, IN2A, PWMB, PWMA, STBY); //B são as rodas da esquerda, e A são as rodas da direita
Servo servo;
HCSR04 hcsr04(TRIG, ECHO);
//objeto pro infrared
//objeto pro nrf
byte dir = 's';
byte mode = AUTONOMOUS;
bool on_off = true;
byte battery;
bool horn = false;

//**DECLARATION OF FUNCTIONS**
void autonomous();
//FAZER: variar velocidade da direção e angulo da curva de acordo com o joystick
void bluetooth();
void infrared();
void radio();

void measure_battery();
bool obstacle();
void send_data();
void verify_on_off();

void setup(){
  Serial.begin(9600);
  robot.begin();
  hcsr04.begin();
  servo.attach(SERVO);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BATTERY, INPUT);

  robot.set_pwm(255);
  servo.write(90);
  //FAZER: interrupção pro led de acordo com a bateria e buzzer tocar nível crítico
  delay(1000);
}

void loop(){
  if (on_off){
    switch(mode){
      case AUTONOMOUS:
        autonomous();
        break;
      case BLUETOOTH:
        bluetooth();
        break;
      case INFRARED:
        infrared();
        break;
      case RADIO:
        radio();
        break;
      default:
        autonomous();
        mode = AUTONOMOUS;
    }
  }
  else{
    verify_on_off();
  }

  measure_battery();
  send_data();
  if (horn)
    digitalWrite(BUZZER, HIGH);
  else
    digitalWrite(BUZZER, LOW);
  
  switch(dir){
    case 's':
      robot.stop();
      break;
    case 'B':
      robot.brake();
      break;
    case 'f':
      robot.forward();
      break;
    case 'b':
      robot.backward();
      break;
    case 'l':
      robot.left();
      break;        
    case 'r':
      robot.right();
      break;
    case 'f'+'l':
      robot.forward_left();
      break;
    case 'f'+'r':
      robot.forward_right();
      break;
    case 'b'+'l':
      robot.backward_left();
      break;
    case 'b'+'r':
      robot.backward_right();
      break;
  }
}

//**DEFINITION OF FUNCTIONS**
void autonomous(){
  if (obstacle()){
    float dist_left, dist_right;
    servo.write(0); //0 é esquerda mesmo?
    while (servo.read() != 0){}
    dist_left = hcsr04.distance_cm();
    servo.write(180); //180 é direita mesmo?
    while (servo.read() != 180){}
    dist_right = hcsr04.distance_cm();
    servo.write(90);
    //os dois lados 100% livres, vira pra qualquer lado
    if (dist_left == -1 && dist_right == -1){
      robot.right();
      delay(U_TURN/2);
    }
    //esquerda 100% livre, vai pra ela
    else if(dist_left == -1 && dist_right !=-1){
      robot.left();
      delay(U_TURN/2);
    }
    //direita 100% livre, vai pra ela
    else if(dist_left != -1 && dist_right == -1){
      robot.right();
      delay(U_TURN/2);
    }
    //os dois lados bloqueados, dá meia volta
    else if (dist_left < DIST_MIN_CM && dist_right < DIST_MIN_CM){
      robot.right();
      delay(U_TURN);
    }
    //de resto, vai pro melhor lado, nesse caso, direita mais livre
    else if (dist_left < dist_right){
      robot.right();
      delay(U_TURN/2);
    }
    //nesse caso, esquerda mais livre
    else{
      robot.left();
      delay(U_TURN/2);
    }
    robot.forward();
  }
  dir = 'f';
}

//x- y- m- off on brake horn
void bluetooth(){
  if (Serial.available()){
    String received;
    received = Serial.readStringUntil('.');
    received.replace(".", "");
    switch(received[0]){
      case 'x':
        int joystickx, joysticky;
        received.replace("x-", "");
        joystickx = received.toInt();
        received = Serial.readStringUntil('.');
        received.replace(".", "");
        received.replace("y-", "");
        joysticky = received.toInt();
        if (joystickx == 512 && joysticky == 512)
          dir = 's';
        else if (joystickx > 412 && joystickx < 612 && joysticky < 512)
          dir = 'f';
        else if (joystickx > 412 && joystickx < 612 && joysticky > 512)
          dir = 'b';
        else if (joystickx < 512 && joysticky > 412 && joysticky < 612)
          dir = 'l';
        else if (joystickx > 512 && joysticky > 412 && joysticky < 612)
          dir = 'r';
        else if (joystickx < 412 && joysticky < 512)
          dir = 'f'+'l';
        else if (joystickx > 612 && joysticky < 512)
          dir = 'f'+'r';
        else if (joystickx < 412 && joysticky > 512)
          dir = 'b'+'l';
        else if (joystickx > 612 && joysticky > 512)
          dir = 'b'+'r';
        break;
      case 'm':
        received.replace("m-", "");
        if (received == "autonomous")
          mode = AUTONOMOUS;
        else if (received == "bluetooth")
          mode = BLUETOOTH;
        else if (received == "infrared")
          mode = INFRARED;
        else if (received == "radio")
          mode = RADIO;
        break;
      case 'o':
        if (received == "off"){
          dir = 's';
          on_off = false;
        }
        else if (received == "on")
          on_off = true;
        break;
      case 'b':
        if (received == "brake")
          dir = 'B';
        break;
       case 'h':
        if (received == "horn")
          horn = !horn;
        break;
    }
  }
}

void infrared(){

}

void radio(){

}

void measure_battery(){
  int sensorValue = analogRead(BATTERY);
  float voltage = sensorValue*5.0/341.0;
  voltage = constrain(voltage, VOLTAGE_MIN, VOLTAGE_MAX);
  battery = map(voltage, VOLTAGE_MIN, VOLTAGE_MAX, 0, 100);
}

bool obstacle(){
  if (hcsr04.distance_cm() < DIST_MIN_CM){
    robot.brake();
    delay(100);
    robot.backward();
    delay(500);
    robot.brake();
    return true;
  }
  return false;
}

void send_data(){
  String data = "b-"+String(battery)+".";
  Serial.print(data);
  switch (mode){
    case AUTONOMOUS:
      data = "m-autonomous.";
      break;
    case BLUETOOTH:
      data = "m-bluetooth.";
      break;
    case INFRARED:
      data = "m-infrared.";
      break;
    case RADIO:
      data = "m-radio.";
      break;
  }
  Serial.print(data);
}

void verify_on_off(){
  if (Serial.available()){
    String received;
    received = Serial.readStringUntil('.');
    received.replace(".", "");
    switch(received[0]){
      case 'o':
        if (received == "on")
          on_off = true;
        break;
      case 'm':
        received.replace("m-", "");
        if (received == "autonomous")
          mode = AUTONOMOUS;
        else if (received == "bluetooth")
          mode = BLUETOOTH;
        else if (received == "infrared")
          mode = INFRARED;
        else if (received == "radio")
          mode = RADIO;
        break;
      case 'h':
        if (received == "horn")
          horn = !horn;
        break;
    }
  }
}
