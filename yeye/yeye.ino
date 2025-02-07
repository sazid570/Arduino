#include <WiFi.h>
#include <esp_now.h>
#include <driver/i2s.h>

// Wi-Fi credentials for ESP-NOW communication
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Replace with the MAC address of the receiving ESP32

// PDM microphone pins
const int PDM_MIC1_PIN = 32;
const int PDM_MIC2_PIN = 33;
const int PDM_MIC3_PIN = 34;

// I2S configuration
const i2s_port_t I2S_PORT = I2S_NUM_0;
const int SAMPLE_RATE = 36000; // 36 kHz
const int SAMPLE_BITS = 16;
const int CHANNEL_NUM = 3;

// Buffer for audio samples
const int SAMPLES_PER_CYCLE = 1024;
int16_t samples[SAMPLES_PER_CYCLE * CHANNEL_NUM];

// Data structure for sending samples
typedef struct struct_message {
  int16_t mic1[SAMPLES_PER_CYCLE];
  int16_t mic2[SAMPLES_PER_CYCLE];
  int16_t mic3[SAMPLES_PER_CYCLE];
} struct_message;

struct_message myData;

// Callback function for when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  
  // Disable watchdog timer
  disableCore0WDT();
  disableCore1WDT();
  disableLoopWDT();

  // Initialize Wi-Fi
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Configure I2S
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = SAMPLES_PER_CYCLE,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = 26,
    .ws_io_num = 25,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = PDM_MIC1_PIN
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

void loop() {
  size_t bytes_read;
  i2s_read(I2S_PORT, samples, sizeof(samples), &bytes_read, portMAX_DELAY);

  // Separate channels
  for (int i = 0; i < SAMPLES_PER_CYCLE; i++) {
    myData.mic1[i] = samples[i * CHANNEL_NUM];
    myData.mic2[i] = samples[i * CHANNEL_NUM + 1];
    myData.mic3[i] = samples[i * CHANNEL_NUM + 2];
  }

  // Send data via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  if (result != ESP_OK) {
    Serial.println("Error sending the data");
  }
}