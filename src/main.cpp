#include <M5Core2.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_PWMServoDriver.h>
#include <fauxmoESP.h>
#include <ArduinoOTA.h>
#include "wifi_setting.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

#define SERVOMIN 102  // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 512  // This is the 'maximum' pulse length count (out of 4096)
#define USMIN 500     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 102
#define USMAX 2500    // This is the rounded 'maximum' microsecond length based on the maximum pulse of 512
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

fauxmoESP fauxmo;

void setServoPulse(uint8_t n, double pulse)
{
  double pulselength;
  pulselength = 1000000; // 1,000,000 us per second
  pulselength /= 50;     // 50 Hz
  Serial.print(pulselength);
  Serial.println(" us per period");
  pulselength /= 4096; // 12 bits of resolution
  Serial.print(pulselength);
  Serial.println(" us per bit");
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void servo_angle_write(uint8_t n, int Angle)
{
  double pulse = Angle;
  pulse = pulse / 90 + 0.5;
  setServoPulse(n, pulse);
}

void wifiSetup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void otaSetup()
{
  ArduinoOTA.setHostname("m5stack-core2");

  ArduinoOTA
      .onStart([]()
               {
                 String type;
                 if (ArduinoOTA.getCommand() == U_FLASH)
                   type = "sketch";
                 else // U_SPIFFS
                   type = "filesystem";
                 Serial.println("Start updating " + type);
               })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
                 Serial.printf("Error[%u]: ", error);
                 if (error == OTA_AUTH_ERROR)
                   Serial.println("Auth Failed");
                 else if (error == OTA_BEGIN_ERROR)
                   Serial.println("Begin Failed");
                 else if (error == OTA_CONNECT_ERROR)
                   Serial.println("Connect Failed");
                 else if (error == OTA_RECEIVE_ERROR)
                   Serial.println("Receive Failed");
                 else if (error == OTA_END_ERROR)
                   Serial.println("End Failed");
               });

  ArduinoOTA.begin();
}

void alexaDeviceSetup()
{
  fauxmo.createServer(true);
  fauxmo.setPort(80);
  fauxmo.enable(true);
  fauxmo.addDevice("intercom");
  fauxmo.addDevice("door");
  fauxmo.onSetState([](unsigned char device_id, const char *device_name, bool state, unsigned char value)
                    {
                      Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
                      servo_angle_write(device_id, 90);
                      delay(500);
                      servo_angle_write(device_id, 70);
                      delay(500);
                      servo_angle_write(device_id, 90);
                    });
}

void servoSetup() {
  pwm.begin();
  pwm.setPWMFreq(50);
}

void setup()
{
  // https://docs.m5stack.com/en/module/servo2?id=module-servo-2
  M5.begin(true, true, true, false, kMBusModeInput);
  // servo2 moduleの対応Pinに合わせるため
  Wire.begin(21, 22);
  wifiSetup();
  otaSetup();
  alexaDeviceSetup();
  servoSetup();
}

void loop()
{
  fauxmo.handle();
  ArduinoOTA.handle();
}