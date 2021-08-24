/*
  Controller library for ESP 8266 Ultrasonic Radar Car.
  Created by White Jiang , 2020.08.25
*/
#ifndef Controller_H
#define Controller_H
#include<Arduino.h>
#include "GPIO.h"
class Position
{
    public:
        int x = 127;
        int y = 127;
};
class Controller{
    public:
        Position pos;
        GPIO *gpio;
        int speedCar = 1023;         // 0 - 1023.
        void control();
        Controller(GPIO *gpio){
          this->gpio=gpio;
        }
    private:
        void Up();
        void Down();
        void Left();
        void Right();
        void Stop();
};
#endif
