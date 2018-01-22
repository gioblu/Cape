/*
    _____  _____   _____   _____       _____
   |      |_____| |_____| |_____      |
   |_____ |     | |       |_____ ____ |_____  version 3.0

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

typedef struct {
  unsigned char salt;          // Salt used for encryption
  unsigned char *key;          // Key
  uint16_t length;    // Keep these private and safe
  unsigned char reduced_key;   // Computed reduced key
} cape_t;

void cape_init(
  cape_t *cape,
  unsigned char *key,
  uint16_t length,
  uint8_t salt
);

void cape_hash(
  cape_t *cape,
  unsigned char *source,
  unsigned char *destination,
  uint16_t length
);

void cape_encrypt(
  cape_t *cape,
  unsigned char *source,
  unsigned char *destination,
  uint16_t length,
  uint8_t iv
);

void cape_decrypt(
  cape_t *cape,
  unsigned char *source,
  unsigned char *destination,
  uint16_t length
);

/* Compute a 1 byte version of the encryption key */
char cape_compute_reduced_key(unsigned char *key, uint16_t length) {
  unsigned char reduced_key = 0;
  for(uint16_t i = 0; i < length; i++) // Reduced key computation
    reduced_key ^= (key[i] << (i % 8));
  return reduced_key;
};

void cape_init(
  cape_t *cape,
  unsigned char *key,
  uint16_t length,
  uint8_t salt = 0
) {
  cape->salt = salt;
  cape->key = key;
  cape->length = length;
  cape->reduced_key = cape_compute_reduced_key(key, length);
};

/* Decrypt data:
   (max 65534 characters) */
void cape_decrypt(
  cape_t *cape,
  unsigned char *source,
  unsigned char *destination,
  uint16_t length
) {
  uint16_t index = length - 1;
  // 1 Compute salty reduced key
  uint8_t srk = cape->salt ^ cape->reduced_key;
  // 2 Decrypt initialization vector using key and salty reduced key
  uint8_t iv = source[index] ^ index ^
    cape->key[(index ^ srk) % cape->length];
  // 3 Decrypt source using key, initialization vector and salty reduced key
  for(uint16_t i = 0; i < index; ++i)
    destination[i] = source[i] ^ iv ^ i ^
      cape->key[(srk ^ i) % cape->length];
};

/* Stream cipher, private key, initialization vector based encryption
   algorithm (max 65534 characters):  */
void cape_encrypt(
  cape_t *cape,
  unsigned char *source,
  unsigned char *destination,
  uint16_t length,
  uint8_t iv
) {
  // 1 Compute salty reduced key or srk
  uint8_t srk = cape->salt ^ cape->reduced_key;
  // 2 Encrypt initialization vector using key and salty reduced key or srk
  destination[length] = iv ^ length ^
    cape->key[(length ^ srk) % cape->length];
  // 3 Encrypt source using key, initialization vector and salty reduced key
  for(uint16_t i = 0; i < length; ++i)
    destination[i] = source[i] ^ iv ^ i ^
      cape->key[(srk ^ i) % cape->length];
};

/* Symmetric cipher using private key, reduced key and optionally salt:
   (max 65534 characters) */
void cape_hash(
  cape_t *cape,
  unsigned char *source,
  unsigned char *destination,
  uint16_t length
) {
  uint8_t srk = cape->salt ^ cape->reduced_key;
  for(uint16_t i = 0; i < length; i++) {
    uint8_t isrk = srk ^ i;
    destination[i] = source[i] ^ isrk ^ cape->key[isrk % cape->length];
  }
};
