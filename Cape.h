/*  _____  _____   _____   _____
   |      |     | |     | |
   |      |_____| |_____| |_____
   |      |     | |       |
   |_____ |     | |       |_____  version 1.3

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

#ifndef Cape_h
  #define Cape_h
  #include "Arduino.h"
#endif

class Cape {
  public:
    Cape(char *key, uint8_t key_length = 0);
    void crypt(char *source, char *destination, uint8_t length, boolean initialization_vector = false, boolean side = false);
    void encrypt(char *source, char *destination, uint8_t length);
    void decrypt(char *source, char *destination, uint8_t length);
    uint8_t generate_IV();
  private:
    char * _key;
    char   _key_length;
    char   _reduced_key;
};
