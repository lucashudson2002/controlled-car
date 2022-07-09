//**bibliotecas**
#include <Servo.h>
//ultrassonico
#include "TB6612FNG.h"
//nrf
//ir

//**pinos**
//HCSR04 + SERVO
#define TRIG A3
#define ECHO A4
#define SERVO A6
//PONTE-H
#define STBY 2
#define AIN1 3
#define AIN2 4
#define PWMA 5
#define PWMB 6
#define BIN1 7
#define BIN2 8
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

//**declaração funcoes**
void autonomo();
void bluetooth();
void infrared();
void radio();

void setup(){
	Serial.begin(9600);
}

void loop(){
	
}

//**definição funcoes**
void autonomo(){

}
void bluetooth(){

}
void infrared(){

}
void radio(){

}
