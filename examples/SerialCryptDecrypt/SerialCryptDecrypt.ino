
#include <Cape.h>

// Encryption key (keep private and safe)
unsigned char key[] = "9LKSUR&#5A";
// Source content to be crypted
unsigned char source[] = "HELLOWORLD";
// Result buffer needs an additional byte for the initialization vector
unsigned char destination[11];
// Insert secret key and its length
Cape cape(key, 10);


void setup() {
  Serial.begin(115200);
};

void loop() {
  Serial.print("Encryption key: ");

  // Generate a random key
  for(int i = 0; i < 10; i++) {
   key[i] = random(0, 255);
   Serial.print((char)key[i]);
  }

  Serial.println();

  // Set it as the encryption key
  cape.set_key(key, 10);
  // Set a new random salt (salt should be known on both sides)
  cape.salt = random(0, 255);

  unsigned long time = micros();
  cape.encrypt(source, destination, 10, random(0, 255));
  time = micros() - time;

  // Destroy original content
  for(int i = 0; i < 10; i++) source[i] = 0;

  Serial.print("CRIPTED:  ");
  for(int i = 0; i < 10; i++)
    Serial.print((char)destination[i]);

  Serial.print(" Masked IV: ");
  Serial.print((uint8_t)destination[11], DEC);

  Serial.print(" Computation time: ");
  Serial.print(time);
  Serial.println(" microseconds");

  time = micros();
  cape.decrypt(destination, source, 11);
  // Result is 1 byte longer to contain the added initialization vector
  time = micros() - time;

  Serial.print("ORIGINAL: ");
  for(int i = 0; i < 10; i++)
    Serial.print((char)source[i]);

  Serial.print("   Real IV: ");
  Serial.print((uint8_t)source[11], DEC);

  Serial.print(" Computation time: ");
  Serial.print(time);
  Serial.print(" microseconds");

  // Erase buffer
  for(int i = 0; i < 10; i++) destination[i] = 0;

  Serial.println();
  Serial.println();
  delay(250);
};
