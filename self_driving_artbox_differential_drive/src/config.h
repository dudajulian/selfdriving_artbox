#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>


// ============================
// WiFi credentials
// ============================


constexpr char WIFI_SSID[] = "Poolparty";
constexpr char WIFI_PASSWORD[] = "q3JBq8G4H5BhG1qg";

// ============================
// Controller-specific configuration
// ============================

#ifdef CONTROLLER_LEFT

constexpr char CONTROLLER_NAME[] = "LEFT";

constexpr uint8_t EN_PIN  = D1;
constexpr uint8_t IN1_PIN = D2;
constexpr uint8_t IN2_PIN = D3;

constexpr uint8_t ENC_A_PIN = D5;
constexpr uint8_t ENC_B_PIN = D6;

const IPAddress LOCAL_IP(192,168,8,50);

#elif defined(CONTROLLER_RIGHT)

constexpr char CONTROLLER_NAME[] = "RIGHT";

constexpr uint8_t EN_PIN  = D1;
constexpr uint8_t IN1_PIN = D2;
constexpr uint8_t IN2_PIN = D3;

constexpr uint8_t ENC_A_PIN = D5;
constexpr uint8_t ENC_B_PIN = D6;

const IPAddress LOCAL_IP(192,168,8,51);

#else

#error "No controller selected!"

#endif

// ============================
// Network
// ============================

const IPAddress GATEWAY(192,168,8,1);
const IPAddress SUBNET(255,255,255,0);