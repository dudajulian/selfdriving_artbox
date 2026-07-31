#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor
{
public:
    Motor(int en, int in1, int in2);

    void begin();

    void setSpeed(int speed);

    void stop();

private:
    int _en;
    int _in1;
    int _in2;
};

#endif