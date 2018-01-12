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
  for(uint16_t i = 0; i < length; i++)
    destination[i] = (
      (cape->reduced_key ^ source[i] ^ cape->salt ^ i) ^
      cape->key[(cape->reduced_key ^ cape->salt ^ i) % cape->length]
    );
};

/* Decrypt data:
   (max 65534 characters) */
void cape_decrypt(
  cape_t *cape,
  char *source,
  char *destination,
  uint16_t length
) {
  // 1 - Hash data without triyng to decode initialization vector
  cape_hash(cape, source, destination, length);
  // 2 - Decrypt initialization vector
  length = length - 1;
  destination[length] ^= (cape->reduced_key ^ cape->salt);
  // 3 - Decrypt data with private key, reduced key and salt
  for(uint16_t i = 0; i < length; i++)
    destination[i] ^= (
      (destination[length] ^ i) ^
      cape->key[(cape->salt ^ i ^ cape->reduced_key) % cape->length]
    );
  // 4 - Hash data with key (static symmetric hashing)
  cape_hash(cape, destination, destination, length);
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
  destination[length] = iv;
  // 1 - Hash data with key (static symmetric hashing)
  cape_hash(cape, source, destination, length);
  // 2 - Encrypt data with private key, reduced key and salt
  for(uint16_t i = 0; i < length; i++)
    destination[i] ^= (
      (destination[length] ^ i) ^
      cape->key[(cape->salt ^ i ^ cape->reduced_key) % cape->length]
    );
  // 3 - Encrypt initialization vector using reduced key and salt
  destination[length] ^= (cape->reduced_key ^ cape->salt);
  // 4 - Further encrypt result and initialization vector
  cape_hash(cape, destination, destination, length + 1);
};
