/*
    _____  _____   _____   _____       _____
   |      |_____| |_____| |_____      |
   |_____ |     | |       |_____ ____ |_____  version 2.0

Cape Copyright (c) 2012-2017, Giovanni Blu Mitolo All rights reserved.

Cape_c ported by colinta github user is designed for use in projects
lacking C++ support.

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

typedef struct {
  char salt;          // Salt used for encryption
  char *key;          // Key
  uint16_t length;    // Keep these private and safe
  char reduced_key;   // Computed reduced key
} cape_t;

void cape_init(cape_t *cape, char *key, uint16_t length, uint8_t salt);

void cape_hash(
  cape_t *cape,
  char *source,
  char *destination,
  uint16_t length
);

void cape_encrypt(
  cape_t *cape,
  char *source,
  char *destination,
  uint16_t length,
  uint8_t iv
);

void cape_decrypt(
  cape_t *cape,
  char *source,
  char *destination,
  uint16_t length
);

/* Compute a 1 byte version of the encryption key */
char cape_compute_reduced_key(char *key, uint16_t length) {
  char reduced_key = 0;
  for(uint16_t i = 0; i < length; i++) // Reduced key computation
    reduced_key ^= (key[i] << (i % 8));
  return reduced_key;
};

void cape_init(cape_t *cape, char *key, uint16_t length, uint8_t salt = 0) {
  cape->salt = salt;
  cape->key = key;
  cape->length = length;
  cape->reduced_key = cape_compute_reduced_key(key, length);
};

/* Symmetric cipher using private key, reduced key and optionally salt:
   (max 65534 characters) */
void cape_hash(
  cape_t *cape,
  char *source,
  char *destination,
  uint16_t length
) {
  uint8_t saltKey = cape->salt ^ cape->reduced_key;
  for(uint16_t i = 0; i < length; i++) {
    uint8_t iSaltKey = saltKey ^ i;
    destination[i] = source[i] ^ iSaltKey ^
      cape->key[iSaltKey % cape->length];
    }
};

/* Decrypt data:
   (max 65534 characters) */
void cape_decrypt(
  cape_t *cape,
  char *source,
  char *destination,
  uint16_t length
) {
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
};

/* Stream cipher, private key, initialization vector based encryption
   algorithm (max 65534 characters):  */
void cape_encrypt(
  cape_t *cape,
  char *source,
  char *destination,
  uint16_t length,
  uint8_t iv
) {
  // 1. Pre-hash salt and reduced key
  uint8_t saltKey = salt ^ _reduced_key;

  // 2. Encrypt initialisation vector using key, reduced key and salt
  destination[length] = iv ^ length ^
    _key[(lastIndex ^ saltKey) % _key_length];

  // 3. Encrypt source data using key, initialisation vector, reduced key and salt
  for (uint16_t i = 0; i < length; ++i)
    destination[i] = source[i] ^ iv ^ i ^
      _key[(saltKey ^ i) % _key_length];
};
