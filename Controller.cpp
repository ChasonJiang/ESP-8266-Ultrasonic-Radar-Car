#include<Arduino.h>
#include"Controller.h"

void Controller::Up(){
    digitalWrite(gpio->IN1, LOW);
    digitalWrite(gpio->IN2, HIGH);
    digitalWrite(gpio->IN3, LOW);
    digitalWrite(gpio->IN4, HIGH);
}

void Controller::Down(){
    digitalWrite(gpio->IN1, HIGH);
    digitalWrite(gpio->IN2, LOW);
    digitalWrite(gpio->IN3, HIGH);
    digitalWrite(gpio->IN4, LOW);
}

void Controller::Left(){
    digitalWrite(gpio->IN1, LOW);
    digitalWrite(gpio->IN2, HIGH);
    digitalWrite(gpio->IN3, HIGH);
    digitalWrite(gpio->IN4, LOW);
}

void Controller::Right(){
    digitalWrite(gpio->IN1, HIGH);
    digitalWrite(gpio->IN2, LOW);
    digitalWrite(gpio->IN3, LOW);
    digitalWrite(gpio->IN4, HIGH);
}

void Controller::Stop(){
    digitalWrite(gpio->IN1, LOW);
    digitalWrite(gpio->IN2, LOW);
    digitalWrite(gpio->IN3, LOW);
    digitalWrite(gpio->IN4, LOW);
}

void Controller::control()
{
    float x = pos.x / 127.0;
    float y = pos.y / 127.0;

    if (y < 1)
    {

        if (x < 1 && x > 0.35)
        {
            x = (x - 0.35) / 0.65;
            y = 1.0 - y;
            analogWrite(gpio->ENA, speedCar * y * x);
            analogWrite(gpio->ENB, speedCar * y);
            Up();
        }
        else if (x <= 0.35 && x > 0.0)
        {
            x = 1.0 - (x - 0.0) / 0.35;
            //          y=1.0-y;
            analogWrite(gpio->ENA, speedCar * y * x);
            analogWrite(gpio->ENB, speedCar * y);
            Left();
        }
        else if (x < 1.65 && x > 1.0)
        {
            x = 1.0 - (x - 1.0) / 0.65;
            y = 1.0 - y;
            analogWrite(gpio->ENA, speedCar * y);
            analogWrite(gpio->ENB, speedCar * y * x);
            Up();
        }
        else if (x < 2.0 && x >= 1.65)
        {
            x = (x - 1.65) / 0.35;
            //          y=1.0-y;
            analogWrite(gpio->ENA, speedCar * y);
            analogWrite(gpio->ENB, speedCar * y * x);
            Right();
        }
    }
    else if (y > 1)
    {
        if (x < 1 && x > 0.35)
        {
            x = (x - 0.35) / 0.65;
            y = y - 1.0;
            analogWrite(gpio->ENA, speedCar * y * x);
            analogWrite(gpio->ENB, speedCar * y);
            Down();
        }
        else if (x <= 0.35 && x > 0.0)
        {
            x = 1.0 - (x - 0.0) / 0.35;
            y = 2.0 - y;
            analogWrite(gpio->ENA, speedCar * y * x);
            analogWrite(gpio->ENB, speedCar * y);
            Right();
        }
        else if (x < 1.65 && x > 1.0)
        {
            x = 1.0 - (x - 1.0) / 0.65;
            y = y - 1.0;
            analogWrite(gpio->ENA, speedCar * y);
            analogWrite(gpio->ENB, speedCar * y * x);
            Down();
        }
        else if (x < 2.0 && x >= 1.65)
        {
            x = (x - 1.65) / 0.35;
            y = 2.0 - y;
            analogWrite(gpio->ENA, speedCar * y);
            analogWrite(gpio->ENB, speedCar * y * x);
            Left();
        }
    }
    else if (x == 0.0 && y == 1.0)
    {
        analogWrite(gpio->ENA, speedCar * 1.0);
        analogWrite(gpio->ENB, speedCar * 1.0);
        Left();
    }
    else if (x == 2.0 && y == 1.0)
    {
        analogWrite(gpio->ENA, speedCar * 1.0);
        analogWrite(gpio->ENB, speedCar * 1.0);
        Right();
    }
    else
    {
        analogWrite(gpio->ENA, speedCar * 1.0);
        analogWrite(gpio->ENB, speedCar * 1.0);
        Stop();
    }
}
