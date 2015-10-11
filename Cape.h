/*  _____  _____   _____   _____
   |      |     | |     | |
   |      |_____| |_____| |_____
   |      |     | |       |
   |_____ |     | |       |_____  version 0.1 */

#ifndef Cape_h
  #define Cape_h

  #if defined(ARDUINO) && (ARDUINO >= 100)
    #include "Arduino.h"
  #else
    #include "WProgram.h"
    #include "WConstants.h"
  #endif
#endif

#define swap(a,b) do { int t = _s_box[a]; _s_box[a] = _s_box[b]; _s_box[b] = t; } while(0)
#define MAX_LENGTH 100

class Cape {

  public:

    Cape(char *encryption_key, uint8_t strength, boolean initialization_vector = false);
    void crypt(char *data, uint8_t length, boolean initialization_vector = false, boolean side = false);
    void encrypt(char *data, uint8_t length);
    void decrypt(char *data, uint8_t length);

    uint8_t generate_IV();
    char result[MAX_LENGTH];

  private:
    char * _encryption_key;
    uint8_t _encryption_strength;
    boolean _iv;
    unsigned char _s_box[255];
};
