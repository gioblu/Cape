/*  _____  _____   _____   _____
   |      |     | |     | |
   |      |_____| |_____| |_____
   |      |     | |       |
   |_____ |     | |       |_____  version 1.0

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
   This product includes Cape developed by Giovanni Blu Mitolo.

-  Neither the name of the <organization> nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

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

/* Initiate Cape passing a random string (maximum 255 characters), a number from
   0 to 255 that determines the encryption strength (and computation time) and
   true if you want to compute the string with initialization vector */

Cape::Cape(char *encryption_key, uint8_t strength, boolean initialization_vector) {
  _iv = initialization_vector;
  _encryption_key = encryption_key;
  _encryption_strength = strength;
}

void Cape::encrypt(char *data, uint8_t length) {
  this->crypt(data, length, _iv, 0);
}

void Cape::decrypt(char *data, uint8_t length) {
  if(_iv) length++;
  this->crypt(data, length, _iv, 1);
}

/* This crypting algorithm is inspired to the RC4 standard with the addition
   of a 1 byte initialization vector and tunable encryption_strength */

void Cape::crypt(char *data, uint8_t length, boolean initialization_vector, boolean side) {
  uint8_t i, j = 0;
  uint8_t key_length = strlen(_encryption_key);

  if(initialization_vector && side)
    for(i = 0; i < length; i++)
      data[i] ^= data[length - 1];

  for (i = 0; i < _encryption_strength; i++) {
    _s_box[i] = 0;
    j = (j + _s_box[i] + _encryption_key[i % key_length]) % MAX_LENGTH;
    swap(_s_box[i], _s_box[j]);
  }

  i = j = 0;
  for (uint8_t k = 0; k < length; k++) {
    i++;
    j = (j + _s_box[i]);
    swap(_s_box[i], _s_box[j]);
    result[k] = data[k] ^ _s_box[(_s_box[i] + _s_box[j])];
  }

  if(initialization_vector && !side) {
    result[length] = this->generate_IV();
    for(i = 0; i < length; i++)
      result[i] ^= result[length];
  }

}

uint8_t Cape::generate_IV() {
  return (micros() % 254) + 1;;
}
