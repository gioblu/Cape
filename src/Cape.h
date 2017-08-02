/*  _____  _____   _____   _____
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
    /* Instantiate Cape pass a pointer to the encryption key and its length:
       (max 65535 characters) */
    Cape(char *key, uint16_t length = 0) {
      _key = key;
      _key_length = length;
      compute_reduced_key(key, length);
    };

    /* Compute a 1 byte version of the encryption key */
    void compute_reduced_key(char *key, uint16_t length) {
      _reduced_key = 0;
      for(uint16_t i = 0; i < length; i++)
         _reduced_key ^= key[i];
    };

    /* Decrypt data:
       (max 65535 characters) */
    void decrypt(char *source, char *destination, uint16_t length) {
      // Hash data without triyng to decode initialization vector at the end
      hash(source, destination, length);
      // Now decrypt decoding initialization vector
      length = length - 1;
      // 1 - Hash data with private key and reduced key
      for(uint16_t i = 0; i < length; i++)
        destination[i] =
          destination[i] ^ (i ^ _key[(i ^ _reduced_key) % _key_length]);
      // 2 - Hash last character to get back real initialization vector
      destination[length] = destination[length] ^ _reduced_key;
      // 3 - Hash all content with the real initialization vector
      for(uint16_t i = 0; i < length; i++)
        destination[i] ^= destination[length];
      // Hash data with key (static reversible hashing)
      hash(destination, destination, length);
    };

    /* Static, reversible hashing using the private key key:
       (max 65535 characters) */
    void hash(char *source, char *destination, uint16_t length) {
      for(uint16_t i = 0; i < length; i++)
        destination[i] = source[i] ^ _key[i % _key_length];
    };

    /* Stream chipher, private key, initialization vector based encryption
       algorithm (max 65535 characters):  */
    void encrypt(
      char *source,
      char *destination,
      uint16_t length,
      uint16_t iv
    ) {
      // Hash data with key (static reversible hashing)
      hash(source, destination, length);
      // 1 - Generate pseudo-random initialization vector
      destination[length] = iv;
      // 2 - Hash result using initialization vector
      // 255 different versions of the same original string
      for(uint16_t i = 0; i < length; i++)
        destination[i] ^= destination[length];
      // 3 - Hash initialization vector with reduced private key
      // Hide initialization vector
      destination[length] ^= _reduced_key;
      // 4 - further hash result with private key and reduced key
      // Original:                             "Hello world" or "Hehehehe"
      // Hashing result at this state:         "$eUUr)DjdUa" or "#o{o{o{o"
      // Avoid exposing characters recurrence: "a5D#)W#<!{s" or "T&^:GQ?D"
      for(uint16_t i = 0; i < length; i++)
        destination[i] ^= (i ^ _key[(i ^ _reduced_key) % _key_length]);
      // Further hash result and initialization vector (without adding a new one)
      this->hash(destination, destination, length + 1);
    };

    /* Set or Change encryption key (max 65535 characters): */
    void set_key(char *key, uint16_t length) {
      _key = key;
      _key_length = length;
      compute_reduced_key(key, length);
    };
  private:
    char * _key;
    char   _key_length;
    char   _reduced_key;
};
