/*
  Program: NRF24L01 controll for the Controlled Car
  Author: Lucas Dias Hudson
  Date: July 15, 2022
  GitHub: https://github.com/lucashudson2002/Controlled-Car
  Prerequisites:
  https://github.com/nRF24/RF24
*/
//**LIBRARIES**
#include <RF24.h>
#include <SPI.h>

//**PINOUT**
#define JOYSTICK_X A0//cáculo das direções
#define JOYSTICK_Y A1//cáculo das direções
#define JOYSTICK_BTN 2//brake
#define TOGGLE 3//alternar modo entre radio e autonomous
#define BTN 4//horn enquanto estiver clicando no botão
#define CE
#define CSN

//**OTHERS DEFINE**
#define CENTER_JOY_X 512.0 //testar
#define CENTER_JOY_Y 512.0 //testar
#define INF_JOY 412.0
#define SUP_JOY 612.0
#define RAD2DEG 180/3.1415926

//**VARIABLES**
int joy_x = CENTER_JOY_X;
int joy_y = CENTER_JOY_Y;
char dir = 's';
byte pwm = 0;
float prop = 1;
char prev_dir = 's';
byte prev_pwm = 0;
float prev_prop = 1;


//**CALCULATION FUNCTIONS**
void reading();
void calculate_direction();
void calculate_pwm();
void calculate_prop();
void send_data();

//pinMode(JOYSTICK_X, INPUT);

void setup() {
  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(JOYSTICK_BTN, INPUT_PULLUP);
  pinMode(TOGGLE, INPUT);
  pinMode(BTN, INPUT_PULLUP);
}

void loop() {
  reading();
  calculate_direction();
  calculate_pwm();
  calculate_prop();
  send_data();
}
//horn brake modo
void reading(){

}

void calculate_direction() {
  if (joy_x == CENTER_JOY_X && joy_y == CENTER_JOY_Y)
    dir = 's';
  else if (joy_x >= INF_JOY && joy_x <= SUP_JOY && joy_y < CENTER_JOY_Y)
    dir = 'f';
  else if (joy_x >= INF_JOY && joy_x <= SUP_JOY && joy_y > CENTER_JOY_Y)
    dir = 'b';
  else if (joy_x < CENTER_JOY_X && joy_y >= INF_JOY && joy_y <= SUP_JOY)
    dir = 'l';
  else if (joy_x > CENTER_JOY_X && joy_y >= INF_JOY && joy_y <= SUP_JOY)
    dir = 'r';
  else if (joy_x <= INF_JOY && joy_y < CENTER_JOY_Y)
    dir = 'f' + 'l';
  else if (joy_x >= SUP_JOY && joy_y < CENTER_JOY_Y)
    dir = 'f' + 'r';
  else if (joy_x <= INF_JOY && joy_y > CENTER_JOY_Y)
    dir = 'b' + 'l';
  else if (joy_x >= SUP_JOY && joy_y > CENTER_JOY_Y)
    dir = 'b' + 'r';
}

void calculate_pwm() {
  float radius = sqrt(pow(joy_x - CENTER_JOY_X, 2) + pow(joy_y - CENTER_JOY_Y, 2));
  if (radius > 350)
    pwm = 255;
  else
    pwm = radius * 255 / 350;
  }

void calculate_prop() {
  float m = (float)(joy_y - INF_JOY)/(joy_x - SUP_JOY);
  int angle = atan(m)*RAD2DEG;
  if (dir == 'f'+'r')
    prop = map(angle, -90, 0, 0, 1);
  else if (dir == 'f'+'l')
    prop = map(angle, 0, 90, 1, 0);
  else if (dir == 'b'+'r')
    prop = map(angle, 0, 90, 1, 0);
  else if (dir == 'b'+'l')
    prop = map(angle, -90, 0, 0, 1);
}

void send_data() {
  if (dir != prev_dir) {
    //send dir
    prev_dir = dir;
  }
  if (pwm != prev_pwm) {
    //send pwm
    prev_pwm = pwm;
  }
  if (prop != prev_prop) {
    //send prop
    prev_prop = prop;
  }
}
