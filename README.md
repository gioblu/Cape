Cape 1.3 Stable
====
This is an arduino compatible string encryption library. I started working on this to add encryption to [PJON](https://github.com/gioblu/PJON) communications bus system. It is really easy to use but limited by the computational power of the ATmega chip. I am now working to bring it compatible with ESP8266 and other Arduino compatible boards.

Cape uses a private key, stream chipher algorithm and a masked initialization vector.

Instantiate Cape passing as first parameter the encryption key, and as second, its length.
```cpp  
  Cape cape("YOUR-ENCRYPTION-KEY", 19);
```
To encrypt a string:
```cpp  
  char source[] ="CRYPTMEPLEASE";
  char destination[14];
  cape.encrypt(source, destination, 13);
```
Inside `destination` you find the crypted version of your string, with an additional byte at the end, use to encrypt data, called initialization vector, so be sure to define your destination buffer, always 1 byte longer:
```cpp  
  for(uint8_t i = 0; i < 14; i++)
    Serial.print(destination[i]);
```
If you want to come back from the encrypted data to the original string:
```cpp  
    cape.decrypt(destination, source, 14);
```
Print the original string as before to check all is working and to get back "CRYPTMEPLEASE" string:
```cpp  
  for(uint8_t i = 0; i < 13; i++)
    Serial.print(source[i]);
```

###License

```cpp  
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
```
