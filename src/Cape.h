/*  _____  _____   _____   _____
   |      |_____| |_____| |_____
   |_____ |     | |       |_____  version 3.0

Cape Copyright © 2012-2018, Giovanni Blu Mitolo All rights reserved.
Cape implementation optimized by Pharap github user
Cape implementation ported in c and python by colinta github user

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

This algorithm should be used for research and educational purposes and should
not be applied in production. */

#pragma once

class Cape {
  public:
    unsigned char salt; // Salt used for encryption (exchange encrypted)
    /* Instantiate Cape passing a key, its length (max 65535) and salt: */
    Cape(unsigned char *key, uint16_t length, uint8_t s = 0) {
      salt = s;
      _key = key;
      _key_length = length;
      compute_reduced_key(key, length);
    };

    /* Compute a 1 byte version of the encryption key */
    void compute_reduced_key(unsigned char *key, uint16_t length) {
      _reduced_key = 0;
      // Reduced key computation
      for(uint16_t i = 0; i < length; i++)
        _reduced_key ^= (key[i] << (i % 8));
    };

    /* Decrypt data: (max length 65535 characters) */
    void decrypt(
      unsigned char *source,
      unsigned char *destination,
      uint16_t length
    ) {
      uint16_t index = length - 1;
      // 1 Compute salty reduced key or srk
      unsigned char srk = salt ^ _reduced_key;
      // 2 Decrypt initialization vector and salty reduced key or srk
      uint8_t iv = source[index] ^ index ^ _key[(index ^ srk) % _key_length];
      // 3 Decrypt data with key, initialization vector and salty reduced key
      for(uint16_t i = 0; i < index; ++i)
        destination[i] = source[i] ^ iv ^ i ^ _key[(srk ^ i) % _key_length];
    };

    /* Stream cipher, private key, initialization vector based encryption
       algorithm (max length 65534 characters):  */
    void encrypt(
      unsigned char *source,
      unsigned char *destination,
      uint16_t length,
      uint8_t iv
    ) {
      // 1 Compute salty reduced key or srk
      unsigned char srk = salt ^ _reduced_key;
      // 2 Encrypt initialization vector using key, salty reduced key or srk
      destination[length] = iv ^ length ^ _key[(length ^ srk) % _key_length];
      // 3 Encrypt data using key, initialization vector and salty reduced key
      for(uint16_t i = 0; i < length; ++i)
        destination[i] = source[i] ^ iv ^ i ^ _key[(srk ^ i) % _key_length];
    };

    /* Symmetric cipher using private key and salty reduced key:
       (max 65535 characters) */
    void hash(unsigned char *source, unsigned char *destination, uint16_t length) {
      // 1 Compute salty reduced key or srk
      unsigned char srk = salt ^ _reduced_key;
      // 2 Hash data
      for(uint16_t i = 0; i < length; i++) {
        uint8_t srki = srk ^ i;
        destination[i] = source[i] ^ srki ^ _key[srki % _key_length];
      }
    };

    /* Set or Change encryption key (max 65534 characters): */
    void set_key(unsigned char *key, uint16_t length) {
      _key = key;
      _key_length = length;
      compute_reduced_key(key, length);
    };

  private:
    unsigned char *   _key;     // Keep private and safe
    uint16_t _key_length;       // Keep private and safe
    unsigned char _reduced_key; // Keep private and safe
};
