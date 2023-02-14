/*
Program: Controlled Car (Autonomous, Bluetooth, Infrared, Radio)
Author: Lucas Dias Hudson
Date: July 17, 2022
GitHub: https://github.com/lucashudson2002/Controlled-Car
Prerequisites:
  https://github.com/lucashudson2002/TB6612FNG
  https://github.com/lucashudson2002/HCSR04
  https://github.com/nRF24/RF24
  https://github.com/Arduino-IRremote/Arduino-IRremote
*/

//**LIBRARIES**
#include <Servo.h>
#include <HCSR04.h>
#include <TB6612FNG.h>
#include <RF24.h>
#include <nRF24L01.h>
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
#define WAITING_TIME_SEND 100

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
float proportion = 0.5;
byte pwm = 255;
unsigned long waiting_time_send = 0;

//**DECLARATION OF FUNCTIONS FOR THE 4 MODES**
void autonomous();
void bluetooth();
void infrared(); //FAZER
void radio(); //FAZER

//**DECLARATION OF OTHERS FUNCTIONS**
void measure_battery();
void nod();
bool obstacle();
void send_data();
void verify_bluetooth();
void verify_radio();
void verify_infrared();

void setup(){
  Serial.begin(9600);
  robot.begin();
  hcsr04.begin();
  servo.attach(SERVO);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BATTERY, INPUT);

  robot.set_pwm(pwm);
  servo.write(90);
  //FAZER: interrupção pro led de acordo com a bateria e buzzer tocar no nível crítico
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
  
  if (mode != BLUETOOTH || !on_off)
    verify_bluetooth();
  if (mode != RADIO)
    verify_radio(); //FAZER
  if (mode != INFRARED)
    verify_infrared();
  measure_battery();
  send_data();
  if (horn)
    digitalWrite(BUZZER, HIGH);
  else
    digitalWrite(BUZZER, LOW);

  robot.set_pwm(pwm);
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
      robot.forward_left(proportion);
      break;
    case 'f'+'r':
      robot.forward_right(proportion);
      break;
    case 'b'+'l':
      robot.backward_left(proportion);
      break;
    case 'b'+'r':
      robot.backward_right(proportion);
      break;
  }
}

//**DEFINITION OF FUNCTIONS**
void autonomous(){
  if (obstacle()){
    float dist_left, dist_right;
    servo.write(180);
    while (servo.read() != 180){}
    dist_left = hcsr04.distance_cm();
    servo.write(0);
    while (servo.read() != 0){}
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
    robot.brake();
  }
  dir = 'f';
  pwm = 255;
}

//d- m- off on brake horn p- a-
void bluetooth(){
  if (Serial.available()){
    String received;
    received = Serial.readStringUntil('/');
    received.replace("/", "");
    switch(received[0]){
      case 'd':
        received.replace("d-", "");
        if (received == "s")
          dir = 's';
        else if (received == "f")
          dir = 'f';
        else if (received == "b")
          dir = 'b';
        else if (received == "l")
          dir = 'l';
        else if (received == "r")
          dir = 'r';
        else if (received == "fl")
          dir = 'f'+'l';
        else if (received == "fr")
          dir = 'f'+'r';
        else if (received == "bl")
          dir = 'b'+'l';
        else if (received == "br")
          dir = 'b'+'r';
        break;
      case 'm':
        byte mode_;
        received.replace("m-", "");
        if (received == "autonomous")
          mode_ = AUTONOMOUS;
        else if (received == "bluetooth")
          mode_ = BLUETOOTH;
        else if (received == "infrared")
          mode_ = INFRARED;
        else if (received == "radio")
          mode_ = RADIO;
        if (mode_ != mode){
          mode = mode_;
          dir = 's';
        }
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
      case 'p':
        received.replace("p-", "");
        pwm = received.toInt();
        break;
      case 'a':
        received.replace("a-", "");
        proportion = received.toFloat();
        break;
    }
  }
  nod();
}

void infrared(){

}

//dir pwm prop mode horn brake
void radio(){
  
}

void measure_battery(){
  int sensor = analogRead(BATTERY);
  float voltage = sensor*5.0/341.0;
  voltage = constrain(voltage, VOLTAGE_MIN, VOLTAGE_MAX);
  battery = map(voltage, VOLTAGE_MIN, VOLTAGE_MAX, 0, 100);
}

void nod(){
  if (obstacle()){
    servo.write(135);
    while (servo.read() != 135){}
    servo.write(45);
    while (servo.read() != 45){}
    servo.write(112);
    while (servo.read() != 112){}
    servo.write(90);
    while (servo.read() != 90){}
  }
}

bool obstacle(){
  float distance = hcsr04.distance_cm();
  if (distance < DIST_MIN_CM && distance != -1 && distance != 0 ){
    robot.brake();
    delay(100);
    robot.set_pwm(255);
    robot.backward();
    delay(500);
    robot.brake();
    robot.set_pwm(pwm);
    return true;
  }
  return false;
}

void send_data(){
  if (millis() - waiting_time_send > WAITING_TIME_SEND){
    waiting_time_send = millis();
    String data = "b-"+String(battery)+"/";
    Serial.print(data);
    switch (mode){
      case AUTONOMOUS:
        data = "m-autonomous/";
        break;
      case BLUETOOTH:
        data = "m-bluetooth/";
        break;
      case INFRARED:
        data = "m-infrared/";
        break;
      case RADIO:
        data = "m-radio/";
        break;
    }
    Serial.print(data);
  }
}

void verify_bluetooth(){
  if (Serial.available()){
    String received;
    received = Serial.readStringUntil('/');
    received.replace("/", "");
    switch(received[0]){
      case 'o':
        if (received == "off"){
          dir = 's';
          on_off = false;
        }
        else if (received == "on")
          on_off = true;
        break;
      case 'm':
        int mode_;
        received.replace("m-", "");
        if (received == "autonomous")
          mode_ = AUTONOMOUS;
        else if (received == "bluetooth")
          mode_ = BLUETOOTH;
        else if (received == "infrared")
          mode_ = INFRARED;
        else if (received == "radio")
          mode_ = RADIO;
        if (mode_ != mode){
          mode = mode_;
          dir = 's';
        }
        break;
      case 'h':
        if (received == "horn")
          horn = !horn;
        break;
    }
  }
}

void verify_radio(){
  //se receber mudança no toggle
}

void verify_infrared(){
  //se receber botão onf/off
}
