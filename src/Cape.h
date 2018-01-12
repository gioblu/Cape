/*
    _____  _____   _____   _____
   |      |_____| |_____| |_____
   |_____ |     | |       |_____  version 2.0

Cape Copyright (c) 2012-2018, Giovanni Blu Mitolo All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once

class Cape {
  public:
    char salt; // Salt used for encryption, can be exchanged if encrypted

    /* Instantiate Cape pass a pointer to the encryption key and its length:
       (max 65534 characters) */
    Cape(char *key, uint16_t length, uint8_t s = 0) {
      salt = s;
      _key = key;
      _key_length = length;
      compute_reduced_key(key, length);
    }

    /* Compute a 1 byte version of the encryption key */
    void compute_reduced_key(char *key, uint16_t length) {
      _reduced_key = 0;
      // Reduced key computation
      for(uint16_t i = 0; i < length; i++)
        _reduced_key ^= (key[i] << (i % 8));
    }

    /* Decrypt data:
       (max 65534 characters) */
    void decrypt(char *source, char *destination, uint16_t length) {
      uint8_t lastIndex = length - 1;

      // 1. Pre-hash salt and reduced key
      uint8_t saltKey = salt ^ _reduced_key;

      // 2. Decrypt initialisation vector using key, reduced key and salt
      uint8_t iv = source[lastIndex] ^ lastIndex ^
        _key[(lastIndex ^ saltKey) % _key_length];

      // 3. Decrypt source data using key, initialisation vector, reduced key and salt
      for (uint16_t i = 0; i < lastIndex; ++i)
        destination[i] = source[i] ^ iv ^ i ^
          _key[(saltKey ^ i) % _key_length];
    }

    /* Stream cipher, private key, initialization vector based encryption
       algorithm (max 65534 characters):  */
    void encrypt(
      char *source,
      char *destination,
      uint16_t length,
      uint8_t iv
    ) {
      // 1. Pre-hash salt and reduced key
      uint8_t saltKey = salt ^ _reduced_key;

      // 2. Encrypt initialisation vector using key, reduced key and salt
      destination[length] = iv ^ length ^
        _key[(length ^ saltKey) % _key_length];

      // 3. Encrypt source data using key, initialisation vector, reduced key and salt
      for (uint16_t i = 0; i < length; ++i)
        destination[i] = source[i] ^ iv ^ i ^
          _key[(saltKey ^ i) % _key_length];
    }

    /* Symmetric cipher using private key, reduced key and optionally salt:
       (max 65534 characters) */
    void hash(char *source, char *destination, uint16_t length) {
      uint8_t saltKey = salt ^ _reduced_key;
      for(uint16_t i = 0; i < length; i++) {
        uint8_t iSaltKey = saltKey ^ i;
        destination[i] = source[i] ^ iSaltKey ^
          _key[iSaltKey % _key_length];
        }
    }

    /* Set or Change encryption key (max 65534 characters): */
    void set_key(char *key, uint16_t length) {
      _key = key;
      _key_length = length;
      compute_reduced_key(key, length);
    }

  private:
    char *   _key;           // Keep private and safe
    uint16_t _key_length;    // Keep private and safe
    char     _reduced_key;   // Keep private and safe
};
