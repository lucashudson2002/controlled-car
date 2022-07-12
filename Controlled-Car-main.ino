//**bibliotecas**
#include <Servo.h>
#include <HCSR04.h>
#include <TB6612FNG.h>
#include <RF24.h>
#include <SPI.h>
#include <IRremote.h>

//**pinos**
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
#define DIVISOR A7

//**outros define**
#define DIST_MIN 30
#define AUTONOMO 1
#define BLUETOOTH 2
#define INFRARED 3
#define RADIO 4

//**variáveis**
TB6612FNG robo(IN1B, IN2B, IN1A, IN2A, PWMB, PWMA, STBY); //B são as rodas da esquerda, e A são as rodas da direita
Servo servo;
HCSR04 hcsr04(TRIG, ECHO);
//objeto pro infrared
//objeto pro nrf
byte direcao = 's';
byte modo;

//**declaração das funções**
void autonomo();
void bluetooth();
void infrared();
void radio();

void setup(){
	Serial.begin(9600);
  robo.begin();
  hcsr04.begin();
  servo.attach(SERVO);
}

void loop(){
	switch(modo){
    case AUTONOMO:
      autonomo();
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
      autonomo();
      modo = 1;
	}

  switch(direcao){
    case 's':
      robo.stop();
      break;
    case 'B':
      robo.brake();
      break;
    case 'f':
      robo.forward();
      break;
    case 'b':
      robo.backward();
      break;
    case 'l':
      robo.left();
      break;
    case 'r':
      robo.right();
      break;
    case 'f'+'l':
      robo.forward_left();
      break;
    case 'f'+'r':
      robo.forward_right();
      break;
    case 'b'+'l':
      robo.backward_left();
      break;
    case 'b'+'r':
      robo.backward_right();
      break;
  }
}

//**definição das funções**
void autonomo(){
  if (hcsr04.distance_cm() < DIST_MIN){
    
  }
}
void bluetooth(){

}
void infrared(){

}
void radio(){

}
