#pragma once

#include "constants.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

struct node_config {
  uint8_t aes_key[AES_KEY_SIZE];
  uint8_t hmac_key[HMAC_KEY_SIZE];
  node_config(uint8_t aes_k[AES_KEY_SIZE], uint8_t hmac_k[HMAC_KEY_SIZE]) {
    memcpy(aes_key, aes_k, AES_KEY_SIZE);
    memcpy(hmac_key, hmac_k, HMAC_KEY_SIZE);
  }
};

#pragma pack(push, 1)
struct key_exchange_send {
  uint32_t k;
};

struct key_exchange_accept {
  uint8_t accept; // 0 or 1
};

struct dht_config {
  uint32_t delay_s;
};

struct dht_measurement {
  int temperature;
  int humidity;
};

struct bmp_config {
  uint32_t delay_s;
};

struct bmp_measurement {
  float pressure;
};

struct subdevices_measurements {
  dht_measurement dht;
  bmp_measurement bmp;
};

struct subdevices_configs {
  dht_config dht;
  bmp_config bmp;
};

enum message_type : uint8_t {
  DHT_MEASUREMENT = 0,
  BMP_MEASUREMENT = 1,
  SUBDEVICES_MEASUREMENTS = 2,
  DHT_CONFIG = 3,
  BMP_CONFIG = 4,
  SUBDEVICES_CONFIGS = 5,
  KEY_EXCHANGE_SEND = 6,
  KEY_EXCHANGE_ACCEPT = 7
};

inline const char *get_enum_name_helpme(const message_type msg_type) {
  switch (msg_type) {
  case DHT_MEASUREMENT:
    return "DHT_MEASUREMENT";
  case BMP_MEASUREMENT:
    return "BMP_MEASUREMENT";
  case SUBDEVICES_MEASUREMENTS:
    return "SUBDEVICES_MEASUREMENTS";
  case DHT_CONFIG:
    return "DHT_CONFIG";
  case BMP_CONFIG:
    return "BMP_CONFIG";
  case SUBDEVICES_CONFIGS:
    return "SUBDEVICES_CONFIGS";
  case KEY_EXCHANGE_SEND:
    return "KEY_EXCHANGE_SEND";
  case KEY_EXCHANGE_ACCEPT:
    return "KEY_EXCHANGE_ACCEPT";
  default:
    return "UNKNOWN";
  }
}
struct send_message {
  // iv and sha256 need to be readable without the encryption
  uint8_t iv[IV_LENGTH];
  uint8_t hash[SHA_HASH_SIZE];
  // the encrypted data (of type message_data)
  uint8_t data[]; // type: message_data
};

struct message_data {
  message_type command;
  uint8_t data[]; // type: dht_measurement, bmp_meas, ...
};

#pragma pack(pop)

struct transmission_quality {
  int rssi;
  float snr;
};
