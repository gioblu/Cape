Cape 1.4 Stable
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
If you need to change the encryption key after instantiation call ```set_key``` passing the new key and its length:
```cpp  
  cape.set_key("YOUR-ENCRYPTION-KEY", 19);
```

###License

```cpp  
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
```
