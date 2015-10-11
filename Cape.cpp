/*  _____  _____   _____   _____
   |      |     | |     | |
   |      |_____| |_____| |_____
   |      |     | |       |
   |_____ |     | |       |_____  version 1.0 */

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
    _s_box[i] = i;
    j = (j + _s_box[i] + _encryption_key[i % key_length]) % _encryption_strength;
    swap(_s_box[i], _s_box[j]);
  }

  i = j = 0;
  for (int k = 0; k < length; k++) {
    i = (i + 1) % _encryption_strength;
    j = (j + _s_box[i]) % _encryption_strength;
    swap(_s_box[i], _s_box[j]);
    result[k] = data[k] ^ _s_box[(_s_box[i] + _s_box[j]) % _encryption_strength];
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
