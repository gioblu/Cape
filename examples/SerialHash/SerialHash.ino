
#include <Cape.h>

// Encryption key (keep private and safe)
unsigned char key[] = "9LKSUR&#5A";
// Source content to be crypted
unsigned char source[] = "HELLOWORLD";
// Result buffer needs an additional byte for the initialization vector
unsigned char destination[10];
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

  // Hash and measure duration
  unsigned long time = micros();
  cape.hash(source, destination, 10);
  time = micros() - time;

  // Destroy original content
  for(int i = 0; i < 10; i++) source[i] = 0;

  // Output result
  Serial.print("ENCRYPTED:  ");
  for(int i = 0; i < 10; i++)
    Serial.print((char)destination[i]);
  Serial.print(" Computation time: ");
  Serial.print(time);
  Serial.println(" microseconds");

  // Hash to get back original string
  time = micros();
  cape.hash(destination, source, 10);
  time = micros() - time;

  // Output result
  Serial.print("ORIGINAL:   ");
  for(int i = 0; i < 10; i++)
    Serial.print((char)source[i]);
  Serial.print(" Computation time: ");
  Serial.print(time);
  Serial.print(" microseconds");

  // Erase buffer
  for(int i = 0; i < 10; i++) destination[i] = 0;

  // Delay
  Serial.println();
  Serial.println();
  delay(250);
};
