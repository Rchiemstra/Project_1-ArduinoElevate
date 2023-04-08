#include <Keypad.h>


const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
                   // r1  r2  r3  r4
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
                   // C4  c3  c2  c1

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int liftkooi_afstand_key,  bestemming_key ;



void setup() {
  Serial.begin(9600);

}

void loop() {
  int key = customKeypad.getKey() - '0';

    if (key == -48) {
    
    
    }

  if (key != -48) {
    if (key == 1) {
      bestemming_key  = 1;
    } else if (key == 2) {
      bestemming_key  = 2;
    } else if (key == 3) {
      bestemming_key  = 3;
    } else if (key == 4) {
      bestemming_key  = 4;
    } else if (key  == 5) {
      bestemming_key  = 5;
    } else {
      bestemming_key  = positie;
    }
  
  Serial.print("k ");
  Serial.println(key);
  delay(100);
}
