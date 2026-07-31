#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder
{
public:
    Encoder(uint8_t pinA, uint8_t pinB);

    void begin();

    long getCount();
    void reset();

    float getSpeed();      // pulses per second

private:
    static void IRAM_ATTR isrA();

    static volatile long _count;

    static uint8_t _pinA;
    static uint8_t _pinB;

    unsigned long _lastTime;
    long _lastCount;
};

#endif