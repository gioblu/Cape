
  /* This sketch implements the known brute force full break exposed by
     rspencer01 that is able to decrypt a Cape ciphertext having some hints
     about its plaintext. See https://github.com/gioblu/Cape/issues/17 */

  #include <Cape.h>

  /* 3 characters encryption key unkown to the attacker
  (short to run in human times also on limited microcontrollers) */

  unsigned char key[] = {0, 155, 90};

  // Key brute force
  unsigned char b_key[] = {0, 0, 0};

  unsigned char source[27] = "";
  unsigned char destination[27];

  uint32_t combinations = 0;

  // Expected characters present in the plaintext guessed by the attacker
  // Simulating an attack to a GPS logger
  unsigned char expected[] = "0.123456789position:{}'";
  // Simulated plaintext version
  unsigned char plaintext[] = "{'position':0.449359147665}";
  // Insert secret key and its length
  Cape cape(key, 3);

  unsigned char result[27];

  int tested_length = 0;

  void setup() {
    Serial.begin(115200);
    Serial.println("Executing brute force key test...");
  };

  bool find_key(unsigned char *buf, int pos, int max_length, int &actual_length) {
    if(pos >= max_length) return false;
    for(uint8_t k = 0; k < 255; k++) {
      buf[pos] = k;
      if(match_string(buf)) {
        actual_length = pos + 1;
        return true; // Found solution, end recursion
      } else if(find_key(buf, pos + 1, max_length, actual_length)) {
        return true; // Found solution, end recursion
      }
    }
    return false;
  };

  bool match_string(unsigned char *buf) {
    combinations++;
    cape.set_key(buf, 3);
    cape.hash(destination, result, 27);
    uint8_t good = 0;
    for(uint8_t r = 0; r < 27; r++) {
      bool contains = false;
      for(uint8_t e = 0; e < 23; e++)
        if(result[r] == expected[e]) {
          contains = true;
          break;
        }
      if(!contains) return false;
      else good++;
    }
    if(good == 27) return true;
  };

  void loop() {
    cape.set_key(key, 3);

    Serial.print("Key: ");
    for(uint8_t y = 0; y < 3; y++) {
      Serial.print(key[y]);
      Serial.print(" ");
    }

    Serial.print(" Plain text: ");

    for(uint8_t i = 0; i < 27; i++) {
      source[i] = plaintext[i];
      Serial.print((char)plaintext[i]);
    }

    cape.hash(source, destination, 27);

    Serial.print(" Cipher text:  ");
    for(int i = 0; i < 27; i++)
      Serial.print((char)destination[i]);

    Serial.println();

    uint32_t time = millis();

    if(find_key(b_key, 0, 3, tested_length)) {
      Serial.println();
      Serial.print("Plain text candidate: ");
      for(uint8_t x = 0; x < 27; x++)
        Serial.print((char)result[x]);
      Serial.print(" found in ");
      Serial.print(millis() - time);
      Serial.print(" milliseconds");
      Serial.print(" - Key: ");
      for(uint8_t y = 0; y < 3; y++) {
        Serial.print(b_key[y]);
        Serial.print(" ");
      }
      Serial.print("- Keys tested: ");
      Serial.print(combinations);
      Serial.println();
      while(true);
    }
  };
