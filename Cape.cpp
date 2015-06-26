/*  _____  _____   _____   _____
   |      |     | |     | |
   |      |_____| |_____| |_____
   |      |     | |       |
   |_____ |     | |       |_____  version 0.1 */

#include "Cape.h"

/* Initiate Cape passing a random string (maximum 255 characters), a number from
   0 to 255 that determines the encryption strength (and computation time) and
   true if you want to compute the string with initialization vector */

Cape::Cape(char *encryption_key, uint8_t strength, boolean initialization_vector) {
  _initialization_vector = initialization_vector;
  _encryption_key = encryption_key;
  _encryption_strength = strength;
}

void Cape::encrypt(char *data) {
  this->crypt(data, _initialization_vector, 0);
}

void Cape::decrypt(char *data) {
  this->crypt(data, _initialization_vector, 1);
}

/* This crypting algorithm is inspired to the RC4 standard with the addition
   of a 1 byte initialization vector and tunable encryption_strength */

void Cape::crypt(char *data, boolean initialization_vector, boolean side) {
  uint8_t i, j = 0;
  uint8_t string_length = strlen(data);
  uint8_t key_length = strlen(_encryption_key);

  if(initialization_vector && side)
    for(i = 0; i < string_length; i++)
      data[i] ^= data[string_length - 1];

  for (i = 0; i < _encryption_strength; i++) {
    _s_box[i] = i;
    j = (j + _s_box[i] + _encryption_key[i % key_length]) % _encryption_strength;
    swap(_s_box[i], _s_box[j]);
  }

  i = j = 0;
  for (int k = 0; k < string_length; k++) {
    i = (i + 1) % _encryption_strength;
    j = (j + _s_box[i]) % _encryption_strength;
    swap(_s_box[i], _s_box[j]);
    result[k] = data[k] ^ _s_box[(_s_box[i] + _s_box[j]) % _encryption_strength];
  }

  if(initialization_vector && !side) {
    result[string_length] = this->generate_IV(string_length);
    for(i = 0; i < string_length; i++)
      result[i] ^= result[string_length];
  }

  result[string_length + 1] = '\0';
}

uint8_t Cape::generate_IV(uint8_t string_length) {
  uint8_t IV = (micros() % 254) + 1;
  for(uint8_t i = 0; i < string_length; i++)
    if(result[i] == IV)
      return this->generate_IV(string_length);
  return IV;
}
