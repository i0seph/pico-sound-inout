# pico-sound-inout
라즈베리 파이 피코로 구현하는 USB 마이크와 사운드 카드 장치

이 글은 chatgpt랑 수다를 떤 내용을 바탕으로 작성된 글이다. 

실전에서 아직 시도 조차 안했다. 

될지 안될지 전혀 모른다. 

![구성도](pico-usb-soundcard-and-mic_1743587234219.png)

## 하드웨어
+ Raspberry Pi RP2040-Zero Mini
+ MEMS Microphone Module -I2S, INMP441
+ CJMCU-1334 DAC Module CJMCU-1334

## 핀 연결
+ I2S BCK (Bit Clock)	GPIO10
+ I2S LRCK (Left/Right Clock)	GPIO11
+ I2S DIN (INMP441 마이크 데이터)	GPIO12
+ I2S DOUT (CJMCU-1334 DAC 데이터)	GPIO9

## 소프트웨어

### 라이브러리
+ Adafruit TinyUSB
+ I2S 라이브러리

### 코드
+ pico-soundcard.ino
