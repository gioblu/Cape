/*  _____  _____   _____   _____
   |      |     | |     | |
   |      |_____| |_____| |_____
   |      |     | |       |
   |_____ |     | |       |_____  version 1.4

Cape Copyright (c) 2012-2016, Giovanni Blu Mitolo All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include "Cape.h"

/* Initiate Cape passing a random string (maximum 255 characters), the number
   of hashing iterations you want to perform (max 32767) */

Cape::Cape(char *key, uint8_t length) {
  _key = key;
  _key_length = length;
  compute_reduced_key(key, length);
};


/* Compute a 1 byte version of the private key: */

void Cape::compute_reduced_key(char *key, uint8_t length) {
  for(uint8_t i = 0; i < length; i++)
     _reduced_key ^= key[i];
};


/* Set or Change encryption key: */

void Cape::set_key(char *key, uint8_t length) {
  _key = key;
  _key_length = length;
  compute_reduced_key(key, length);
};


/* Private key, stream chipher algorithm with masked initialization vector */

void Cape::encrypt(char *source, char *destination, uint8_t length) {
  // Hash data with masked initialization vector at the end
  this->crypt(source, destination, length, true, false);
  // Further hash result and initialization vector (without adding a new one)
  this->crypt(destination, destination, length + 1);
};


void Cape::decrypt(char *source, char *destination, uint8_t length) {
  // Hash data without triyng to decode initialization vector at the end
  this->crypt(source, destination, length);
  // Now decrypt decoding initialization vector
  this->crypt(destination, destination, length - 1, true, true);
};


void Cape::crypt(char *source, char *destination, uint8_t length, boolean initialization_vector, boolean side) {
  uint8_t i = 0;

  if(initialization_vector && side) {
    // 1 - Hash data with private key and reduced key
    for(i = 0; i < length; i++)
      destination[i] = source[i] ^ (i ^ _key[(i ^ _reduced_key) % _key_length]);
    // 2 - Hash last character to get back real initialization vector
    destination[length] = source[length] ^ _reduced_key;
    // 3 - Hash all content with the real initialization vector
    for(i = 0; i < length; i++)
      destination[i] ^= source[length];
  }

  // Hash data with key (static reversible hashing)
  for (i = 0; i < length; i++)
    destination[i] = source[i] ^ _key[i % _key_length];

  if(initialization_vector && !side) {
    // 1 - Generate pseudo-random initialization vector
    destination[length] = this->generate_IV();
    // 2 - Hash result using initialization vector
    // 255 different versions of the same original string
    for(i = 0; i < length; i++)
      destination[i] ^= destination[length];
    // 3 - Hash initialization vector with reduced private key
    // Hide initialization vector
    destination[length] ^= _reduced_key;
    // 4 - further hash result with private key and reduced key
    // i. e. original "Hello world" Hashing result at this state: "$eUUr)DjdUa"
    // Hide possible pattern / repeated charater i.e. result: "a5D#)W#<!{s"
    for(i = 0; i < length; i++)
      destination[i] ^= (i ^ _key[(i ^ _reduced_key) % _key_length]);
  }
};


uint8_t Cape::generate_IV() {
  return (random(0, 256) * millis()) % 255 + 1;
};
