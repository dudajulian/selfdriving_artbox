#include "encoder.h"

// Static members
volatile long Encoder::_count = 0;

uint8_t Encoder::_pinA = 0;
uint8_t Encoder::_pinB = 0;

// ----------------------------------------------------

Encoder::Encoder(uint8_t pinA, uint8_t pinB)
{
    _pinA = pinA;
    _pinB = pinB;

    _lastTime = 0;
    _lastCount = 0;
}

// ----------------------------------------------------

void Encoder::begin()
{
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);

    _count = 0;
    _lastCount = 0;
    _lastTime = millis();

    // Only one interrupt on channel A
    attachInterrupt(digitalPinToInterrupt(_pinA), isrA, RISING);
}

// ----------------------------------------------------

void IRAM_ATTR Encoder::isrA()
{
    // Determine direction from channel B
    if (digitalRead(_pinB))
        _count++;
    else
        _count--;
}

// ----------------------------------------------------

long Encoder::getCount()
{
    noInterrupts();
    long count = _count;
    interrupts();

    return count;
}

// ----------------------------------------------------

void Encoder::reset()
{
    noInterrupts();
    _count = 0;
    interrupts();

    _lastCount = 0;
    _lastTime = millis();
}

// ----------------------------------------------------

float Encoder::getSpeed()
{
    unsigned long now = millis();

    float dt = (now - _lastTime) / 1000.0f;

    if (dt <= 0.0f)
        return 0.0f;

    noInterrupts();
    long current = _count;
    interrupts();

    float speed = (current - _lastCount) / dt;

    _lastCount = current;
    _lastTime = now;

    return speed;
}