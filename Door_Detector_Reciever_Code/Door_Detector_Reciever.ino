#include <esp_now.h>
#include "WiFi.h"
#include "esp_sleep.h"

// ===== Pins =====
const int buttonPin = 33;   // deep sleep button (active low)
const int statusLED = 27;   // on = awake, off = asleep
const int doorLED   = 32;   // on when transmitter senses nothing

// ===== Data structure (must match transmitter exactly) =====
typedef struct struct_message {
  bool objectDetected;
} struct_message;

struct_message myData;

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(myData)) {
    memcpy(&myData, incomingData, sizeof(myData));

    if (myData.objectDetected) {
      digitalWrite(doorLED, LOW);    // something sensed -> LED off
    } else {
      digitalWrite(doorLED, HIGH);   // nothing sensed -> LED on
    }
  }
}

void goToSleep() {
  digitalWrite(statusLED, LOW);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
  esp_deep_sleep_start();
}

void checkButton() {
  if (digitalRead(buttonPin) == LOW) {
    delay(50);
    if (digitalRead(buttonPin) == LOW) {
      Serial.println("Button pressed, going to sleep");
      while (digitalRead(buttonPin) == LOW) { }
      goToSleep();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(buttonPin, INPUT);
  pinMode(statusLED, OUTPUT);
  pinMode(doorLED, OUTPUT);

  digitalWrite(statusLED, HIGH);   // awake
  digitalWrite(doorLED, LOW);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receiver ready");
}

void loop() {
  checkButton();
}