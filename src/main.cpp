#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(9600);
  Serial.print("test");
}

void loop() {
  Serial.print("test");
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
