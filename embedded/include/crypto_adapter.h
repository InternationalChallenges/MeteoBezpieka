#pragma once

// custom headers
#include "constants.h"
#include "data_types.h"

// arduino libs
#include <AES.h>
#include <CTR.h>
#include <SHA256.h>

// std libs
#include <algorithm>

enum class encrypt_error : uint8_t { OK, BAD_DATA_LENGTH };
enum class decrypt_error : uint8_t { OK, BAD_DATA_LENGTH, WRONG_HASH };

// maybe use ed25519 for key exchange
// https://rweather.github.io/arduinolibs/classCurve25519.html
class CryptoAdapter {
private:
  // https://rweather.github.io/arduinolibs/classCBC.html
  // https://rweather.github.io/arduinolibs/classAESCommon.html
  CTR<AES256> _ctr;
  // https://rweather.github.io/arduinolibs/classSHA256.html
  SHA256 _sha;
  uint8_t _hmac_key[HMAC_KEY_SIZE];

public:
  CryptoAdapter(node_config *config);

  // only run this function after the other peer confirms the secret
  void generateNodeConfig(const uint8_t shared_secret_in[SHARED_DH_SIZE],
                          node_config *new_config) {
    // _sha.resetHMAC(_hmac_key, HMAC_KEY_SIZE);
    // _sha.update(shared_secret_in, SHARED_DH_SIZE);
    // _sha.finalizeHMAC(_hmac_key, HMAC_KEY_SIZE, new_config,
    //                   sizeof(node_config));
    uint8_t hash_output[SHA_HASH_SIZE];
    // generate the AES_KEY
    _sha.resetHMAC(_hmac_key, HMAC_KEY_SIZE);
    _sha.update(shared_secret_in, SHARED_DH_SIZE);
    _sha.update("AES_KEY", 7); // context string
    _sha.finalizeHMAC(_hmac_key, HMAC_KEY_SIZE, hash_output, SHA_HASH_SIZE);
    memcpy(new_config->aes_key, hash_output, AES_KEY_SIZE);

    // generate the HMAC_KEY
    _sha.resetHMAC(_hmac_key, HMAC_KEY_SIZE);
    _sha.update(shared_secret_in, SHARED_DH_SIZE);
    _sha.update("HMAC_KEY", 8); // context string
    _sha.finalizeHMAC(_hmac_key, HMAC_KEY_SIZE, hash_output, SHA_HASH_SIZE);
    memcpy(new_config->hmac_key, hash_output, HMAC_KEY_SIZE);
  }

  bool isCorrectDataLength(uint32_t length);

  void updateConfig(const node_config *config);

  // every message needs to be divisible by the block size
  static constexpr uint32_t getBlockSize() {
    return std::max(AES_BLOCK_SIZE, SHA_BLOCK_SIZE);
  }

  static uint32_t toValidSize(uint32_t size) {
    uint32_t blockSize = CryptoAdapter::getBlockSize();
    return size + (blockSize - size % blockSize) % blockSize;
  }

  encrypt_error encryptData(uint8_t *output_data, const uint8_t *input_data,
                            uint32_t data_length, uint8_t iv[IV_LENGTH]);

  decrypt_error decryptData(uint8_t *output_data, const uint8_t *input_data,
                            uint32_t data_length, const uint8_t iv[IV_LENGTH]);

  encrypt_error computeHash(const uint8_t *data, uint32_t data_length,
                            uint8_t output_hash[SHA_HASH_SIZE]);

  encrypt_error encryptMessage(send_message *output_msg,
                               const send_message *input_msg,
                               uint32_t data_length);

  decrypt_error decryptMessage(send_message *output_msg,
                               const send_message *input_msg,
                               uint32_t data_length);
};
template <typename T> constexpr T kwargs_max(T a) { return a; }

template <typename T, typename... Ts>
constexpr T kwargs_max(T a, T b, Ts... rest) {
  return kwargs_max((a > b ? a : b), rest...);
}
inline size_t MAX_MESSAGE_SIZE_helpme = CryptoAdapter::toValidSize(
    sizeof(send_message) + // Base size of send_message (without data[])
    sizeof(message_data) + // Base size of message_data (without data[])
    kwargs_max(sizeof(dht_measurement),         //
               sizeof(bmp_measurement),         //
               sizeof(subdevices_measurements), //
               sizeof(dht_config),              //
               sizeof(bmp_config),              //
               sizeof(subdevices_configs),      //
               sizeof(key_exchange_send),       //
               sizeof(key_exchange_accept)));   //
// Max size of any payload (from the union)
