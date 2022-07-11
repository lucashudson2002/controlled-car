//**bibliotecas**
#include <Servo.h>
//ultrassonico
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

//**variáveis**
//B são as rodas da esquerda, e A são as rodas da direita
TB6612FNG robo(IN1B, IN2B, IN1A, IN2A, PWMB, PWMA, STBY);

//**declaração das funções**
void autonomo();
void bluetooth();
void infrared();
void radio();

void setup(){
	Serial.begin(9600);
  robo.begin();
}

void loop(){
	
}

//**definição das funções**
void autonomo(){

}
void bluetooth(){

}
void infrared(){

}
void radio(){

}
