// from chatgpt
#include <Arduino.h>
#include "tusb.h"  // TinyUSB 라이브러리
#include "hardware/i2s.h"
#include "pico/stdlib.h"
#include "pico/audio_i2s.h"

#define I2S_DATA_OUT 9   // I2S 출력 데이터 핀 (DAC)
#define I2S_DATA_IN 12   // I2S 입력 데이터 핀 (MIC)
#define I2S_BCK 10       // I2S 비트 클럭
#define I2S_LRCK 11      // I2S 좌우 클럭

#define AUDIO_SAMPLE_RATE 44100  // 44.1kHz 샘플링 레이트
#define AUDIO_BUFFER_SIZE 512    // 버퍼 크기

// I2S 오디오 출력 설정
audio_buffer_pool_t *init_audio_i2s() {
    static audio_format_t audio_format = {
        .sample_freq = AUDIO_SAMPLE_RATE,
        .format = AUDIO_BUFFER_FORMAT_PCM_S16,
        .channel_count = 2
    };
    
    static i2s_config_t i2s_config = {
        .data_pin = I2S_DATA_OUT,
        .clock_pin_base = I2S_BCK,
        .dma_channel = 0,
        .pio_sm = 0
    };
    
    audio_buffer_pool_t *pool = audio_i2s_setup(&audio_format, &i2s_config);
    return pool;
}

// I2S 마이크 입력 설정
void init_i2s_mic() {
    i2s_inst_t *i2s = i2s0;
    i2s_configure(i2s, I2S_BCK, I2S_LRCK, I2S_DATA_IN);
    i2s_start(i2s);
}

// USB 오디오 출력 콜백
void tud_audio_tx_done_cb(uint8_t rhport, uint8_t ep_out, uint8_t ep_in) {
    static int16_t audio_buffer[AUDIO_BUFFER_SIZE];
    size_t received_bytes = tud_audio_read(audio_buffer, sizeof(audio_buffer));
    
    if (received_bytes > 0) {
        struct audio_buffer *buffer = take_audio_buffer(init_audio_i2s(), true);
        if (buffer) {
            memcpy(buffer->buffer, audio_buffer, received_bytes);
            buffer->sample_count = received_bytes / 2;
            give_audio_buffer(init_audio_i2s(), buffer);
        }
    }
}

// USB 마이크 입력 콜백
void tud_audio_rx_done_cb(uint8_t rhport, uint8_t ep_out, uint8_t ep_in) {
    static int16_t mic_buffer[AUDIO_BUFFER_SIZE];
    i2s_read_blocking(i2s0, mic_buffer, sizeof(mic_buffer));
    tud_audio_write(mic_buffer, sizeof(mic_buffer));
}

void setup() {
    Serial.begin(115200);
    tud_init(BOARD_TUD_RHPORT);
    init_audio_i2s();
    init_i2s_mic();
}

void loop() {
    tud_task(); // TinyUSB 이벤트 처리
}
