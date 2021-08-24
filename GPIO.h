#ifndef GPIO_H
#define GPIO_H
#include<Arduino.h>
#define D0 16
#define D1 5  // GPIO05(D1)
#define D2 4  // GPIO04(D2)
#define D3 0  // GPIO00(D3)
#define D4 2  // GPIO02(D4)
#define D5 14 // GPIO14(D5)
#define D6 12 // GPIO12(D6)
#define D7 13 // GPIO13(D7)
#define D8 15 // GPIO15(D8)
#define TX 1
#define RX 3

class GPIO{
  public:
    const int ServoPin=D1; // sg90
    const int ENA=D2; // L298N enable/seepd motors Left
    const int ENB=D3; // L298N enable/seepd motors Right
    const int IN1=D5; // L298N in1 motors Right 
    const int IN2=D6; // L298N in2 motors Right 
    const int IN3=D7; // L298N in3 motors Left
    const int IN4=D8; // L298N in4 motors Left 
    const int TrigPin = D4; // ultrasonic distance sensor
    const int EchoPin = D0; // ultrasonic distance sensor
    
};
#endif
