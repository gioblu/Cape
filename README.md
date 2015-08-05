Cape 
====
This is an arduino compatible string encryption library. I wrote this to add encryption feature to [PJON](https://github.com/gioblu/PJON) communications bus system. It is really easy to use but it is limited by the computational power of the ATmega chip. To have decent computation time and to save memory the encryption_strength is hardcoded inside Cape.h file to 2 and the max_length of the string is hardcoded there to 100 characters, if you need more feel free to change. 

Cape use an algorithm inspired by the RC4 standard with the addition of a 1 byte initialization vector and tunable encryption_strength; all this is reversible, so two entities with the same key can share encrypted data and come back to original content without any other procedure. Without initialization_vector, crypting the string multiple times will give you always the same encrypted result, that is generally considered as really bad.

For many application can fit without initialization_vector:
```cpp  
  Cape cape("YOUR-ENCRYPTION-KEY"); 
```
but if you want an additional layer of security:
```cpp  
  Cape cape("YOUR-ENCRYPTION-KEY", true); 
```
To use the library simply call cape.encrypt passing the string you want to crypt:
```cpp  
  cape.crypt("CRYPTMEPLEASE"); 
```
inside cape.result you find the crypted version of your string
```cpp  
  for(uint8_t i = 0; i < max_length; i++)
    Serial.print(cape.result[i]);
```
if you want to try to come back from the encrypted data to the original string you can:
```cpp  
    cape.decrypt((*char)cape.result);
```
and print the original string as before to check all is working and to get back "CRYPTMEPLEASE" string:
```cpp  
  for(uint8_t i = 0; i < max_length; i++)
    Serial.print(cape.result[i]);
```

Is a big pleasure to see my code being used by people to solve problems, and I would be happy to have feedback and good advices to bring it more efficient and easy to use, so feel free to say your opinion!!
