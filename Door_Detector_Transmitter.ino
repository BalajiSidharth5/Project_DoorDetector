#include <esp_now.h>
#include "WiFi.h"
#include "esp_sleep.h"

// ===== Pins =====
const int buttonPin = 33;   // deep sleep button (active low)
const int irSensor  = 32;    // IR sensor OUT
const int statusLED = 27;   // on = awake, off = asleep

// ===== Receiver's MAC address - FILL THIS IN =====
uint8_t receiverMAC[] = {0x38, 0x18, 0x2B, 0x2D, 0x6C, 0x98};

// ===== Data structure (must match receiver exactly) =====
typedef struct struct_message {
  bool objectDetected;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

unsigned long lastSend = 0;
const unsigned long sendInterval = 500;   // send every 500ms

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  // not used, but required by the 3.x core signature
}

void goToSleep() {
  digitalWrite(statusLED, LOW);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);  // wake on button press (LOW)
  esp_deep_sleep_start();
}

void checkButton() {
  if (digitalRead(buttonPin) == LOW) {
    delay(50);                                   // debounce
    if (digitalRead(buttonPin) == LOW) {
      Serial.println("Button pressed, going to sleep");  // runs BEFORE sleep
      while (digitalRead(buttonPin) == LOW) { }  // wait for release
      goToSleep();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(buttonPin, INPUT);
  pinMode(irSensor, INPUT);
  pinMode(statusLED, OUTPUT);

  digitalWrite(statusLED, HIGH);   // awake

  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  checkButton();

  if (millis() - lastSend >= sendInterval) {
    lastSend = millis();
    myData.objectDetected = (digitalRead(irSensor) == LOW);  // LOW = detected
    esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData));
  }
}