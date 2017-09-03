/*
    _____  _____   _____   _____
   |      |_____| |_____| |_____
   |_____ |     | |       |_____  version 2.0

Cape Copyright (c) 2012-2017, Giovanni Blu Mitolo All rights reserved.

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
       (max 65535 characters) */
    Cape(char *key, uint16_t length, uint8_t s = 0) {
      salt = s;
      _key = key;
      _key_length = length;
      compute_reduced_key(key, length);
    };

    /* Compute a 1 byte version of the encryption key */
    void compute_reduced_key(char *key, uint16_t length) {
      _reduced_key = 0;
      // Reduced key computation
      for(uint16_t i = 0; i < length; i++)
        _reduced_key ^= (key[i] << (i % 8));
    };

    /* Decrypt data:
       (max 65535 characters) */
    void decrypt(char *source, char *destination, uint16_t length) {
      // 1 - Hash data without triyng to decode initialization vector
      hash(source, destination, length);
      // 2 - Decrypt initialization vector
      length = length - 1;
      destination[length] ^= (_reduced_key ^ salt);
      // 3 - Decrypt data with private key, reduced key and salt
      for(uint16_t i = 0; i < length; i++)
        destination[i] ^= (
          (destination[length] ^ i) ^
          _key[(salt ^ i ^ _reduced_key) % _key_length]
        );
      // 4 - Hash data with key (static symmetric hashing)
      hash(destination, destination, length);
    };

    /* Stream chipher, private key, initialization vector based encryption
       algorithm (max 65535 characters):  */
    void encrypt(
      char *source,
      char *destination,
      uint16_t length,
      uint8_t iv
    ) {
      destination[length] = iv;
      // 1 - Hash data with key (static symmetric hashing)
      hash(source, destination, length);
      // 2 - Encrypt data with private key, reduced key and salt
      for(uint16_t i = 0; i < length; i++)
        destination[i] ^= (
          (destination[length] ^ i) ^
          _key[(salt ^ i ^ _reduced_key) % _key_length]
        );
      // 3 - Encrypt initialization vector using reduced key and salt
      destination[length] ^= (_reduced_key ^ salt);
      // 4 - Further encrypt result and initialization vector
      hash(destination, destination, length + 1);
    };

    /* Symmetric chiper using private key, reduced key and optionally salt:
       (max 65535 characters) */
    void hash(char *source, char *destination, uint16_t length) {
      for(uint16_t i = 0; i < length; i++)
        destination[i] = (
          (_reduced_key ^ source[i] ^ salt ^ i) ^
          _key[(_reduced_key ^ salt ^ i) % _key_length]
        );
    };

    /* Set or Change encryption key (max 65535 characters): */
    void set_key(char *key, uint16_t length) {
      _key = key;
      _key_length = length;
      compute_reduced_key(key, length);
    };
  private:
    char *   _key;           // Keep private and safe
    uint16_t _key_length;    // Keep private and safe
    char     _reduced_key;   // Keep private and safe
};
