Cape 3.0
====
Cape implements private key, public salt, xor based symmetric stream cipher developed to offer encryption on limited microcontrollers.

Cape is an experimental project, should be used for research and educational purposes and should not be applied in production. Cape 3.0 has been posted on reddit/r/crypto to obtain feedback about its algorithm and great minds broke it in a matter of hours:
- rspencer01 exposed a full break of the algorithm using brute force and some hints about its plaintext
- silkeh exposed a known plaintext attack and poor performance of salt

See the extremely educational related [issue](https://github.com/gioblu/Cape/issues/17), [KnownPlainTextVulnerability](https://github.com/gioblu/Cape/blob/master/examples/KnownPlainTextVulnerability/KnownPlainTextVulnerability.ino) and [ArduinoBruteForceKey](https://github.com/gioblu/Cape/blob/master/examples/ArduinoBruteForceKey/ArduinoBruteForceKey.ino) examples.

Cape algorithm is weak because has been engineered not taking in consideration enough the [Kerckhoff's principle](https://en.wikipedia.org/wiki/Kerckhoffs%27s_principle) or "one ought to design systems under the assumption that the enemy will immediately gain full familiarity with them" in contrast to the "security through obscurity" principle that is obviously not applicable to open-source software. Furthermore, has not been considered the possibility that an attacker could constrain the necessary time to brute force the key used having partial hints about its plain text content (known plain text vulnerability). Further study will be done to determine the feasibility of a new, more secure encryption algorithm to be applied in this library.

### How to use Cape
Instantiate Cape passing the encryption key, its length and optionally the salt. The longer is the key the higher is the encryption strength. Encryption key must be kept secret and never transmitted, generated salt instead can be exchanged only if encrypted.
```cpp  
  // Instance name - Private key - Length
  Cape cape("YOUR-ENCRYPTION-KEY", 19);
  // Optional Public salt (to be shared encrypted)
  cape.salt = "S";                      
```
Adding salt higher data security, supporting the same private key usage for a longer time, exchanging a new salt once in a while if necessary.
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
### Encryption strength
After the support of expert cryptologists from [reddit/r/crypto](reddit/r/crypto), has been practically demonstrated that Cape v3.0 algorithm is weak and a full break is already publicly available.

### Ports developed by contributors
- [cape-js](https://github.com/eldisniper/cape-js) javascript port by eldisniper
- [Cape.py](https://github.com/colinta/Cape.py) python port by by colinta
- [CapeDotNet](https://github.com/Pharap/CapeDotNet) C# port by Pharap
- [CapeHaskell](https://github.com/Pharap/CapeHaskell) Haskell port by Pharap

### Contribute
Feel free to open an issue or to send a pull request, changes affecting both algorithm and implementation can be proposed and are evaluated. If you have detected a vulnerability, act ethically and share **immediately** your discovery with the community. Thank you for your support.

### License

```cpp  
/*  _____  _____   _____   _____
   |      |_____| |_____| |_____
   |_____ |     | |       |_____  version 3.0

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
