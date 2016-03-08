/*  _____  _____   _____   _____
   |      |     | |     | |
   |      |_____| |_____| |_____
   |      |     | |       |
   |_____ |     | |       |_____  version 1.2

Cape Copyright (c) 2012-2016, Giovanni Blu Mitolo All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
- Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

-  Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

-  All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   "Secured by Cape encryption, Giovanni Blu Mitolo 2012-2016"

-  Neither the name of Cape nor the names of its contributors
   may be used to endorse or promote products derived from
   this software without specific prior written permission.

This software is provided by the copyright holders and contributors "as is"
and any express or implied warranties, including, but not limited to, the
implied warranties of merchantability and fitness for a particular purpose
are disclaimed. In no event shall the copyright holder or contributors be liable
for any direct, indirect, incidental, special, exemplary, or consequential
damages (including, but not limited to, procurement of substitute goods or
services; loss of use, data, or profits; or business interruption) however
caused and on any theory of liability, whether in contract, strict liability,
or tort (including negligence or otherwise) arising in any way out of the use
of this software, even if advised of the possibility of such damage. */

#include "Cape.h"

/* Initiate Cape passing a random string (maximum 255 characters), the number
   of hashing iterations you want to perform (max 32767) */

Cape::Cape(char *key, uint8_t key_length) {
  _key = key;
  _key_length = key_length;

  // Generate a 1 byte version of the private key
  for(uint8_t i = 0; i < strlen(_key); i++)
    _reduced_key ^= key[i];
}

/* Private key, stream chipher algorithm with masked initialization vector */

void Cape::encrypt(char *data, uint8_t length) {
  // Hash data with masked initialization vector at the end
  this->crypt(data, length, true, 0);
  // Further hash result and initialization vector (without adding a new one)
  this->crypt(result, length + 1);
}

void Cape::decrypt(char *data, uint8_t length) {
  // Hash data without triyng to decode initialization vector at the end
  this->crypt(data, length);
  // Now decrypt decoding initialization vector
  this->crypt(result, length - 1, true, 1);
}

void Cape::crypt(char *data, uint8_t length, boolean initialization_vector, boolean side) {
  uint8_t i = 0;
  if(initialization_vector && side) {
    // 1 - Hash data with private key and reduced key
    for(i = 0; i < length; i++)
      data[i] ^= (i ^ _key[(i ^ _reduced_key) % _key_length]);
    // 2 - Hash last character to get back real initialization vector
    data[length] ^=  _reduced_key;
    // 3 - Hash all content with the real initialization vector
    for(i = 0; i < length; i++)
      data[i] ^= data[length];
  }

  // Hash data with key
  // Static, reversible hashing
  for (i = 0; i < length; i++)
    result[i] = data[i] ^ _key[i % _key_length - 1];

  if(initialization_vector && !side) {
    // 1 - Generate pseudo-random initialization vector
    result[length] = this->generate_IV();
    // 2 - Hash result using initialization vector
    // 255 different versions of the same original string 
    for(i = 0; i < length; i++)
      result[i] ^= result[length];
    // 3 - Hash initialization vector with reduced private key
    // Hide initialization vector
    result[length] ^= _reduced_key;
    // 4 - further hash result with private key and reduced key
    // i. e. original "Hello world" Hashing result at this state: "$eUUr)DjdUa"
    // Hide possible pattern / repeated charater i.e. result: "a5D#)W#<!{s"
    for(i = 0; i < length; i++)
      result[i] ^= (i ^ _key[(i ^ _reduced_key) % _key_length]);
  }
}

uint8_t Cape::generate_IV() {
  return (random(0, 256) * millis()) % 255 + 1;
}
