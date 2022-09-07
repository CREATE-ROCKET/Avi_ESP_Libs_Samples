# Avi_ESP_Libs

電装班のESP32のライブラリ開発・保管用リポジトリです。

# 開発の進め方

https://github.com/CREATE-ROCKET/Avi_ESP_Libs にアクセスし、`fork`ボタンからforkを作成してください。

forkしたリポジトリを手元環境にcloneし、開発を行い、stage、commit、pushを行ってください。

forkしたリポジトリにて、`pull request` ボタンからプルリクエストを行ってください。

# SPI Flashへの書き込みができない場合
現在、SPI Flashへの書き込みがEspressif32の最新のバージョン（@5.1.1）ではRebootしてしまい出来ないことを確認しています。（2022/09/06現在）

そのため、PlatformIOのEspressif32のバージョンを@4.2.0に固定してください。
platformio.iniを以下のように書き換えてください。

```
[env:esp32doit-devkit-v1]
platform = espressif32@4.2.0
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 115200
```