Cape 
====
This is an arduino compatible string encryption library. I wrote this to add encryption feature to [PJON](https://github.com/gioblu/PJON) communications bus system. It is really easy to use but limited by the computational power of the ATmega chip.

Cape use a private key, an iteration tunable stream chipher algorithm with the addition of 1 byte initialization vector; all this is reversible, so two entities with the same key can share encrypted data and come back to the original content.

Instantiate Cape passing as first parameter the encryption key, and as second, the encryption strength. If set to high values will encrease the computation time needed:
```cpp  
  Cape cape("YOUR-ENCRYPTION-KEY", 2); 
```
If you want an additional layer of security using `initialization_vector` pass true as third parameter:
```cpp  
  Cape cape("YOUR-ENCRYPTION-KEY", 2, true); 
```
To encrypt a string:
```cpp  
  cape.encrypt("CRYPTMEPLEASE", 13); 
```
Inside cape.result you find the crypted version of your string
```cpp  
  for(uint8_t i = 0; i < 13; i++)
    Serial.print(cape.result[i]);
```
If you want to come back from the encrypted data to the original string:
```cpp  
    cape.decrypt(cape.result, 13);
```
and print the original string as before to check all is working and to get back "CRYPTMEPLEASE" string:
```cpp  
  for(uint8_t i = 0; i < 13; i++)
    Serial.print(cape.result[i]);
```

---

###License

```cpp  
/* Copyright (c) 2012-2016, Giovanni Blu Mitolo All rights reserved.

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
```
