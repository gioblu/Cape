#include <Cape.h>

Cape cape("YOUR-PRIVATE-KEY", 2, true);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  cape.encrypt("Hello world!", 12);
  for(int i = 0; i < MAX_LENGTH; i++)
    Serial.print(cape.result[i]);
  Serial.println();
  
  cape.decrypt(cape.result, 12);
    for(int i = 0; i < MAX_LENGTH; i++)
    Serial.print(cape.result[i]);
  Serial.println();
  
  delay(100);
}
