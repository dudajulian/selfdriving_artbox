#include "motor.h"

Motor::Motor(int en, int in1, int in2)
{
    _en = en;
    _in1 = in1;
    _in2 = in2;
}

void Motor::begin()
{
    pinMode(_en, OUTPUT);
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);

    stop();
}

void Motor::setSpeed(int speed)
{
    speed = constrain(speed, -1023, 1023);

    Serial.print("Requested PWM: ");
    Serial.println(abs(speed));

    if (speed > 0)
    {
        digitalWrite(_in1, HIGH);
        digitalWrite(_in2, LOW);
        analogWrite(_en, speed);
    }
    else if (speed < 0)
    {
        digitalWrite(_in1, LOW);
        digitalWrite(_in2, HIGH);
        analogWrite(_en, -speed);
    }
    else
    {
        stop();
    }
}

void Motor::stop()
{
    analogWrite(_en,0);

    digitalWrite(_in1,LOW);
    digitalWrite(_in2,LOW);
}