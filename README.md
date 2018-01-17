Cape 2.0
====
Cape implements a new, private key, public salt, xor based, symmetric stream cipher along with a pseudo-random initialization vector asymmetric encryption algorithm, both originally developed to offer data security on limited microcontrollers.

If you agree with the statement "Don't roll your own crypto" you should not use Cape for the following 2 reasons:
1. Cape is developed by passionate experimenters rolling their own crypto
2. Better not to try to understand crypto and use alternatives developed by intelligence agencies

### How to use Cape
Instantiate Cape passing the encryption key, its length and optionally the salt. The longer is the key the higher is the coverage `Cape` can offer. To have an acceptable level of security the encryption key should always be at least as long as the maximum data length transmitted. Adding salt higher data security, enabling to keep the same private key for a longer time, exchanging a new common salt once in a while if necessary. Salt must be exchanged encrypted (never transmit salt data in plain text).
```cpp  
  // Instance name - Private key - Length
  Cape cape("YOUR-ENCRYPTION-KEY", 19);
  // Optional Public salt (to be shared encrypted)
  cape.salt = "S";                      
```
To hash data call `hash` passing the data source buffer, the destination buffer and the data length:
```cpp  
  char source[] = "CRYPTMEPLEASE";
  char destination[13];
  // Light symmetric stream cipher -> "I1C_8K)*8G}dB"
  cape.hash(source, destination, 13);    
  // Hash again to get back        -> "CRYPTMEPLEASE"
  cape.hash(destination, source, 13);   
```
To encrypt a string call `encrypt` passing the data source buffer, the destination buffer, the data length and a pseudo-random 8-bit integer used as initialization vector:
```cpp  
  char source[] ="CRYPTMEPLEASE";
  char destination[14];
  // Strong asymmetric encryption  -> "09)*&{!@#*)I)"
  cape.encrypt(source, destination, 13, random(0, 255));
  // Call decrypt to get back      -> "CRYPTMEPLEASE"
  cape.decrypt(destination, source, 14);
```
In the destination buffer it is saved the encrypted data along with a one byte initialization vector at the end, be sure to define your destination buffer always 1 byte longer and not to exceed 65534 data length.

If you need to change the encryption key after instantiation call `set_key` passing the new key and its length:
```cpp  
  cape.set_key("YOUR-ENCRYPTION-KEY", 19);
```
### Compatible implementations
- [cape-js](https://github.com/eldisniper/cape-js) Javascript port developed by eldisniper
- [Cape.py](https://github.com/colinta/Cape.py) Python port developed by colinta

### Contribute
Feel free to open an issue or to send a pull request, changes affecting both algorithm and implementation can be proposed and are evaluated. If you have detected a vulnerability, act ethically, share your discovery with the community **immediately**.  Thank you for your support.

### License

```cpp  
/*  _____  _____   _____   _____
   |      |_____| |_____| |_____
   |_____ |     | |       |_____  version 2.0

Cape Copyright © 2012-2018, Giovanni Blu Mitolo All rights reserved.

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
