#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "motor.h"
#include "encoder.h"
#include "config.h"




// ============================
// Objects
// ============================

ESP8266WebServer server(80);

Motor motor(EN_PIN, IN1_PIN, IN2_PIN);

Encoder encoder(ENC_A_PIN, ENC_B_PIN);




// ============================
// Motor endpoint
// ============================
void sendCorsHeaders()
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "*");
}

void handleMotor()
{
    sendCorsHeaders();

    if (!server.hasArg("speed"))
    {
        server.send(400, "text/plain", "Missing speed parameter");
        return;
    }

    int speed = server.arg("speed").toInt();

    speed = constrain(speed, -1023, 1023);

    Serial.print("Requested speed: ");
    Serial.println(speed);

    motor.setSpeed(speed);

    server.send(200, "text/plain", "OK");
}


void handleEncoder()
{
    sendCorsHeaders();

    String json = "{";
    json += "\"controller\":\"";
    json += CONTROLLER_NAME;
    json += "\",";
    json += "\"count\":";
    json += String(encoder.getCount());
    json += ",";
    json += "\"time\":";
    json += String(millis());
    json += "}";

    server.send(200, "application/json", json);
}


void handleOptions()
{
    sendCorsHeaders();

    server.send(204);
}

// ============================
// Status Endpoint
// ============================

void handleStatus()
{
    sendCorsHeaders();

    String json = "{";
    json += "\"controller\":\"";
    json += CONTROLLER_NAME;
    json += "\"";
    json += "}";

    server.send(200, "application/json", json);
}
// ============================
// Setup
// ============================

void setup()
{
    Serial.begin(9600);
    
    analogWriteRange(1023);
    analogWriteFreq(1000);

    motor.begin();

    encoder.begin();

    Serial.println();
    Serial.println("Connecting to WiFi...");

    
    WiFi.config(LOCAL_IP, GATEWAY, SUBNET);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("[");
    Serial.print(CONTROLLER_NAME);
    Serial.print("] IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/motor", HTTP_GET, handleMotor);
    server.on("/encoder", HTTP_GET, handleEncoder);
    server.on("/status", HTTP_GET, handleStatus);

    server.on("/motor", HTTP_OPTIONS, handleOptions);
    server.on("/encoder", HTTP_OPTIONS, handleOptions);
    server.on("/status", HTTP_OPTIONS, handleOptions);

    server.begin();

    Serial.println("HTTP Server started");
}

// ============================
// Main loop
// ============================
void loop()
{
    server.handleClient();
    
}