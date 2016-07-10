#include <Cape.h>

Cape cape("SECRET-KEY", 10);  // Insert your secret key and its length

char source[] = "HELLOWORLD"; // Source content to be crypted
char destination[11];         // Result buffer needs an additional byte for the initialization vector

void setup() {
  Serial.begin(115200);
};


void loop() {
  unsigned long time = micros();
  cape.encrypt(source, destination, 10);
  time = micros() - time;

  // Destroy original content
  for(int i = 0; i < 10; i++)
    source[i] = 0;

  Serial.println("------------------");
  Serial.print("CRIPTED:  ");
  for(int i = 0; i < 10; i++)
    Serial.print(destination[i]);
  Serial.print(" Masked IV: ");
  Serial.print((uint8_t)destination[11], DEC);
  Serial.print(" Computation time: ");
  Serial.print(time);
  Serial.println(" microseconds");

  time = micros();
  cape.decrypt(destination, source, 11);
  // Result String is 1 character longer because of the initialization vector

  time = micros() - time;

  Serial.print("ORIGINAL: ");
  for(int i = 0; i < 10; i++)
    Serial.print(source[i]);
  Serial.print("   Real IV: ");
  Serial.print((uint8_t)source[11], DEC);
  Serial.print(" Computation time: ");
  Serial.print(time);
  Serial.println(" microseconds");

  // Erase buffer
  for(int i = 0; i < 10; i++)
    destination[i] = 0;

  delay(250);
};
