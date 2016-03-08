#include <Cape.h>

Cape cape("SECRET-KEY", 10);
// Insert your secret key and its length

void setup() {
  Serial.begin(115200);
}

void loop() {
  unsigned long time = micros();
  cape.encrypt("Original data!", 14);
  time = micros() - time;

  Serial.println("------------------");
  Serial.print("CRIPTED:  ");
  for(int i = 0; i < 14; i++)
    Serial.print(cape.result[i]);
  Serial.print(" Masked IV: ");
  Serial.print((uint8_t)cape.result[14], DEC);
  Serial.print(" Computation time: ");
  Serial.print(time);
  Serial.println(" microseconds");

  time = micros();
  cape.decrypt(cape.result, 15);
  // Result String is 1 character longer because of the initialization vector
  time = micros() - time;

  Serial.print("ORIGINAL: ");
  for(int i = 0; i < 14; i++)
    Serial.print(cape.result[i]);
  Serial.print("   Real IV: ");
  Serial.print((uint8_t)cape.result[14], DEC);
  Serial.print(" Computation time: ");
  Serial.print(time);
  Serial.println(" microseconds");

  delay(500);
}
