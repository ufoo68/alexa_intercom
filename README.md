# Alexa対応、インターフォン自動化IoTデバイス

インターフォンを自動で対応するためのデバイスのソースコードです。Alexaに対応しています。↓イメージはこんな感じです。

![img](https://pbs.twimg.com/media/FFcAhW0aAAE_T0l?format=jpg&name=small)

## 使用ハードウェア

- [M5Stack Core2](https://www.switch-science.com/catalog/6530/)
- [M5Stack用Servo2モジュール](https://www.switch-science.com/catalog/6737/)
- [M5GO Bottom2](https://www.switch-science.com/catalog/6785/)

## 開発環境

本開発環境は[PlatformIOのvocode拡張](https://platformio.org/install/ide?install=vscode)を用いて行っています。

## Wifiへの接続

以下のファイルを`include/wifi_setting.h`に作成してください。

```include/wifi_setting.h:c
#define WIFI_SSID "your ssid"
#define WIFI_PASS "your pass"
```

## OTAを使う

vscode上で`env:m5stack-core2_ota`を選択してください。ただし一度有線でコードを書き込む必要があります。

## 参考

- [fauxmoESP で スマートホームデバイス自作](https://homemadegarbage.com/espalexa04)
- [M5-ProductExampleCodes Servo2](https://github.com/m5stack/M5-ProductExampleCodes/tree/master/Module/SERVO2)
- [PlatformIOでESP32(M5Stick-C)のOTA機能を使う](https://wak-tech.com/archives/1906)
