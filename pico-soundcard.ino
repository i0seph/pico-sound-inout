/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 Copyright (c) 2024 Phl Schatzmann

 This example uses a Rasperry Pico to send the output to i2s. To prevent
 any underruns or overruns we take care of the feedback ourself. 
 
*********************************************************************/

/***************
    징하게 오래 걸렸네
    1. Adafruit_TinyUSB 필요함
    2. 그런데, 여긴 audio가 없음
    3. https://github.com/pschatzmann/Adafruit_TinyUSB_Arduino/tree/Audio 어르신이 만들었음 
    4. 소리가 끊긴다고 예제 코드에 큐를 사용했는데, 이 큐가 나의 환경에서는 정상 작동 안됨 
    5. 큐 다 빼고 시리얼 디버그 다 빼고 코드 정리해도 안됨
    6. i2s data pin 지정을 해야 했음
    7. 다 되었는데, 스테레오의 좌우가 바뀌어 들림 (dac 보드 문제인지, 코드 문제인지는 아직 확인 안했음)
***************/

#include "Adafruit_TinyUSB.h"
#include "AudioTools.h"
#include "AudioTools/Concurrency/RP2040.h"

AudioInfo info(44100, 2, 16);
Adafruit_USBD_Audio usb;
BufferRP2040 buffer(256, 20); 
//QueueStream queue(buffer);
I2SStream i2s;
//StreamCopy copier(i2s, queue);

size_t writeCB(const uint8_t* data, size_t len, Adafruit_USBD_Audio& ref) {
  usb.setFeedbackPercent(buffer.size()*100 / buffer.available());
  //Serial.println(len);

  // Serial.println(data);
  return i2s.write(data,len);
}

void setup() {  
  //Serial.begin(115200);
  //AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
  //while(!Serial);  // wait for serial
  //delay(1000);
  //Serial.println("starting...");
  // start queue
  //queue.begin(80);

  // Start USB device as Audio Sink w/o automatic feedback
  usb.setFeedbackMethod(0);
  usb.setWriteCallback(writeCB);
  if (!usb.begin(info.sample_rate, info.channels, info.bits_per_sample)){
    // Serial.println("USB error");
    int i = 0;
  }

  // If already enumerated, additional class driverr begin() e.g msc, hid, midi won't take effect until re-enumeration
  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }

  
}

void loop() {
  //usb.updateLED();
}

void setup1(){
  //start i2s
  auto cfg = i2s.defaultConfig(TX_MODE);
  // 이거 지정해야 소리남
  cfg.pin_data = 28;
  // 기본 핀 bck = 26, ws = 27
  cfg.copyFrom(info);
  cfg.buffer_size = 256;
  cfg.buffer_count = 3;
  if (!i2s.begin(cfg)){
    // Serial.print("i2s error");
    int i  = 0;
  }
}

void loop1() {
  //Serial.print('call');
  //Serial.println(queue.
  // copier.copy();
}
