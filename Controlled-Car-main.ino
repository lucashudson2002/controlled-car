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
#define DIST_MIN_CM 30
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
byte modo = 2;
bool on_off = true;
byte battery;

//**declaração das funções**
void autonomo();
void bluetooth();
void infrared();
void radio();
void verify_on_off();
void send_data();
void measure_battery();

void setup(){
  Serial.begin(9600);
  robo.begin();
  hcsr04.begin();
  servo.attach(SERVO);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(DIVISOR, INPUT);
}

void loop(){
	if (on_off){
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
				modo = AUTONOMO;
		}
	}
	else{
		verify_on_off();
	}
	
	send_data();
	
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
  if (hcsr04.distance_cm() < DIST_MIN_CM){
    
  }
}
//x- y- m- off on brake
void bluetooth(){
	if (Serial.avaialble()){
		String received;
		received = Serial.readStringUntil('.');
		received.replace('.', '');
		switch(received[0]){
			case 'x':
				int joystickx, joysticky;
				received.replace('x-', '');
				joystickx = received.toInt();
				received = Serial.readStringUntil('.');
				received.replace('.', '');
				received.replace('y-', '');
				joysticky = received.toInt();
				if (joystickx == 512 && joysticky == 512)
					direcao = 's';
				else if (joystickx > 412 && joystickx < 612 && joysticky > 512)
					direcao = 'f';
				else if (joystickx > 412 && joystickx < 612 && joysticky < 512)
					direcao = 'b';
				else if (joystickx < 512 && joysticky > 412 && joysticky < 612)
					direcao = 'l';
				else if (joystickx > 512 && joysticky > 412 && joysticky < 612)
					direcao = 'r';
				else if (joystickx < 412 && joysticky > 512)
					direcao = 'f'+'l';
				else if (joystickx > 612 && joysticky > 512)
					direcao = 'f'+'r';
				else if (joystickx < 412 && joysticky < 512)
					direcao = 'b'+'l';
				else if (joystickx > 612 && joysticky < 512)
					direcao = 'b'+'r';
				break;
			case 'm':
				received.replace('m-', '');
				if (received == "autonomo")
					modo = AUTONOMO;
				else if (received == "bluetooth")
					modo = BLUETOOTH;
				else if (receivfed == "infrared")
					modo = INFRARED;
				else if (received == "radio")
					modo = RADIO;
				break;
			case 'o':
				if (received == "off"){
					dircao = 's';
					on_off = false;
				}
				else if (received == "on")
					on_off = true;
				break;
			case 'b':
				if (received == "brake")
					direcao = 'B';
				break;
		}
	}
}
void infrared(){

}
void radio(){

}

void verify_on_off(){
	if (Serial.available()){
		String received;
		received = Serial.readStringUntil('.');
		received.replace('.', '');
		switch(received[0]){
			case 'o':
				if (received == "on")
					on_off = true;
				break;
			case 'm':
				received.replace('m-', '');
				if (received == "autonomo")
					modo = AUTONOMO;
				else if (received == "bluetooth")
					modo = BLUETOOTH;
				else if (receivfed == "infrared")
					modo = INFRARED;
				else if (received == "radio")
					modo = RADIO;
				break;
		}
	}
}

void send_data(){
	
}
void measure_battery(){
	
}
