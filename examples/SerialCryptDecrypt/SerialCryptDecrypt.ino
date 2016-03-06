#include <Cape.h>

Cape cape("YOUR-ENCRYPTION-KEY", 2);

void setup() {
  Serial.begin(115200);
}

void loop() {
  unsigned long time = micros();
  cape.encrypt("Hello world!", 12);
  time = micros() - time;

  Serial.println("------------------");
  Serial.print("CRIPTED:  ");
  for(int i = 0; i < 12; i++)
    Serial.print(cape.result[i]);

  Serial.print(" Computation time: ");
  Serial.print(time);
  Serial.println(" microseconds");

  time = micros();
  cape.decrypt(cape.result, 12);
  time = micros() - time;

  Serial.print("ORIGINAL: ");
  for(int i = 0; i < 12; i++)
    Serial.print(cape.result[i]);

  Serial.print(" Computation time: ");
  Serial.print(time);
  Serial.println(" microseconds");

  delay(150);
}
