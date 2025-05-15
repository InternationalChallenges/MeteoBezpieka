#pragma once

#define IV_LENGTH 16
#define AES_KEY_SIZE 32
#define AES_BLOCK_SIZE 16

#define HMAC_KEY_SIZE 64
#define SHA_HASH_SIZE 32
#define SHA_BLOCK_SIZE 64

#define SHARED_DH_SIZE 32

// esp8266 osapi lib
#if defined(ESP8266)
#include "osapi.h"
inline void fill_random(void *buf, size_t len) {
  os_get_random((unsigned char *)buf, len);
}

#elif defined(ESP32)
#include "esp_random.h"
// #include <esp_system.h>

inline void fill_random(void *buf, size_t len) { esp_fill_random(buf, len); }
#endif
