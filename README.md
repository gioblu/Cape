Cape 2.0
====
Cape is a string encryption library developed to offer efficient encryption on small microcontrollers. Cape implements a new, private key, XOR based, stream chipher algorithm on top of which is applied further XOR based encryption using a private key and pseudo-random initialization vector, that is also encrypted using the private key and then appended to information. I started working on this to add encryption to [PJON](https://github.com/gioblu/PJON) communications bus system. It is implemented in c++ and engineered to avoid any external dependency.

### How to use Cape

Instantiate Cape passing the encryption key and its length.
```cpp  
  Cape cape("YOUR-ENCRYPTION-KEY", 19);
```
To encrypt a string:
```cpp  
  char source[] ="CRYPTMEPLEASE";
  char destination[14];
  cape.encrypt(source, destination, 13);                 // Light stream cypher   
  cape.encrypt(source, destination, 13, random(0, 255)); // Strong encryption
```
In `destination` it is saved the encrypted version of the string, with an additional byte at the end, used to encrypt data, called initialization vector, so be sure to define your destination buffer, always 1 byte longer:
```cpp  
  for(uint8_t i = 0; i < 14; i++)
    Serial.print(destination[i]);
```
If you want to come back from encrypted data to the original string:
```cpp  
    cape.decrypt(destination, source, 14);
```
Print the original string as before to check that all is working and to get back "CRYPTMEPLEASE" string:
```cpp  
  for(uint8_t i = 0; i < 13; i++)
    Serial.print(source[i]);
```
If you need to change the encryption key after instantiation call `set_key` passing the new key and its length:
```cpp  
  cape.set_key("YOUR-ENCRYPTION-KEY", 19);
```

### License

```cpp  
/*  _____  _____   _____   _____
   |      |_____| |_____| |_____
   |_____ |     | |       |_____  version 2.0

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
