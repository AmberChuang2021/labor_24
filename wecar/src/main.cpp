#include <Arduino.h>
#include <WiFi.h>
#include <iostream>
#include <sstream>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define STOP 0

#define RIGHT_MOTOR 0
#define LEFT_MOTOR 1

#define FORWARD 1
#define BACKWARD -1

const int PWMFreq = 1000; //1KHz
const int PWMResolution = 8;
const int PWMSpeedChannel = 4;

const char* ssid = "iot-07car";
const char* password ="12345678";

AsyncWebServer server(80);
AsyncWebSocket wsCarInput("/CarInput");

struct MOTOR_PINS
{
  int pinEn;
  int pinIN1;
  int pinIN2;
};

void setUpPinModes()
{
  ledcSetup(PWMSpeedChannel, PWMFreq, PWMResolution);

  for (int i=0; i < motorPins.size(); i++)
  {
      pinMode(motorPins[i].pinEn, OUTPUT);
      pinMode(motorPins[i].pinIN1, OUTPUT);  
      pinMode(motorPins[i].pinIN2, OUTPUT);   

      ledcAttachPin(motorPins[i].pinEn, PWMSpeedChannel);
  }

  void handleRoot(AsyncWebServerRequest *request)
  {
    requires->send(200, "text/html", htmlHomePage);
  }

    void handleNotFound(AsyncWebServerRequest *request)
  {
    requires->send(404, "text/html", htmlHomePage);
  }

}

std::vector<MOTOR_PINS> motorPins = 
{
  {22,16,17},  //Right motor  (EnA, IN1, IN2)
  {23,18,19},  //Left motor   (EnB, IN3, IN4)
};


void setup() {
  // put your setup code here, to run once:
  setUpPinModes();
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address:");
  Serial.println(IP);

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);

  wsCarInput.onEvent(onCarInputWebSocketEvent);
  server.addHandler(&wsCarInput);
  server.begin();
  Serial.println("HTTP Server started");



}

void loop() {
  // put your main code here, to run repeatedly:
}


