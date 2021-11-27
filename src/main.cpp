#include <M5Core2.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_PWMServoDriver.h>
#include <fauxmoESP.h>
#include "wifi_setting.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

#define SERVOMIN 102  // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 512  // This is the 'maximum' pulse length count (out of 4096)
#define USMIN 500     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 102
#define USMAX 2500    // This is the rounded 'maximum' microsecond length based on the maximum pulse of 512
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

fauxmoESP fauxmo;

void setup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  M5.begin(true, true, true, true, kMBusModeInput);
  pwm.begin();
  pwm.setPWMFreq(50);
  M5.Lcd.setCursor(115, 0, 4);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Lcd.print("Servo2");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
  // fauxmo
  fauxmo.createServer(true);
  fauxmo.setPort(80);
  fauxmo.enable(true);
  fauxmo.addDevice("intercom");
  fauxmo.addDevice("door");
  fauxmo.onSetState([](unsigned char device_id, const char *device_name, bool state, unsigned char value)
                    { 
                      switch (device_id)
                      {
                        // intercom
                      case 0: 
                        Serial.printf("intercom: %s\n", state ? "ON" : "OFF");
                        break;
                      case 1:
                        Serial.printf("door: %s\n", state ? "ON" : "OFF");
                      default:
                        break;
                      }
                    });
}

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

void loop()
{
  // for (int i = 0; i < 16; i++)
  // {
  //   setServoPulse(i, 0.5);
  // }
  // delay(500);
  // for (int i = 0; i < 16; i++)
  // {
  //   setServoPulse(i, 2.5);
  // }
  // delay(500);
  fauxmo.handle();
}