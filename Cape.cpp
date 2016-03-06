/*  _____  _____   _____   _____
   |      |     | |     | |
   |      |_____| |_____| |_____
   |      |     | |       |
   |_____ |     | |       |_____  version 1.1

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

Cape::Cape(char *key, uint8_t iterations) {
  _key = key;
  _iterations = iterations;

  for(uint8_t i = 0; i < strlen(_key); i++)
    _reduced_key ^= key[i];
}

/* Private key, iteration tunable stream chipher algorithm with optional initialization vector */

void Cape::encrypt(char *data, uint8_t length) {
  this->crypt(data, length);
  for(int i = 0; _iterations && i < _iterations; i++)
    this->crypt(result, length, (i == _iterations - 1) ? true : false, 0);
}

void Cape::decrypt(char *data, uint8_t length) {
  this->crypt(data, length);
  for(int i = 0; _iterations && i < _iterations; i++)
    if(i == _iterations - 1)
      return this->crypt(result, length + 1, true, 1);
    else this->crypt(result, length);
}

void Cape::crypt(char *data, uint8_t length, boolean initialization_vector, boolean side) {
  uint8_t i = 0;
  uint8_t key_length = strlen(_key);

  if(initialization_vector && side) {
    data[length - 1] ^=  _reduced_key;

    for(i = 0; i < length; i++)
      data[i] ^= data[length - 1];
  }

  for (i = 0; i < length; i++)
    result[i] = data[i] ^ _key[i % key_length - 1];

  if(initialization_vector && !side) {
    result[length] = this->generate_IV();

    for(i = 0; i < length; i++)
      result[i] ^= result[length];

    result[length] ^= _reduced_key;
  }
}

uint8_t Cape::generate_IV() {
  return (micros() % 254) + 1;;
}
