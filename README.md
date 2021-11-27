# Alexa対応、インターフォン自動化IoTデバイス

インターフォンを自動で対応するためのデバイスのソースコードです。Alexaに対応しています。

## 使用ハードウェア

- [M5Stack Core2](https://www.switch-science.com/catalog/6530/)
- [M5Stack用Servo2モジュール](https://www.switch-science.com/catalog/6737/)
- [M5GO Bottom2](https://www.switch-science.com/catalog/6785/)

## Wifiへの接続

以下のファイルを作成してください。

```lib\wifi_setting\wifi_setting.h:c
#define WIFI_SSID "your ssid"
#define WIFI_PASS "your pass"
```

## 参考

- [fauxmoESP で スマートホームデバイス自作](https://homemadegarbage.com/espalexa04)
- [M5-ProductExampleCodes Servo2](https://github.com/m5stack/M5-ProductExampleCodes/tree/master/Module/SERVO2)
