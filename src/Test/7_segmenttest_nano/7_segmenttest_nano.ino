
const int latchPin = 12;//Pin connected to ST_CP of 74HC595
const int clockPin = 10;//Pin connected to SH_CP of 74HC595
const int dataPin = 11; //Pin connected to DS of 74HC595

#define led_lift 8

#define knopl 4
#define knopr 3

#define knopl_led 7
#define knopr_led 6

int buttonstate_l = LOW;
int buttonstate_r = LOW;

#define irObstaclePin 9  // This is our input pin

int waar;

int Obstacle = HIGH;

/*=======================================================================================================
  //display 0,1,2,3,4,5,6,7,8
  Number 0 :  00000011     3
  Number 1 :  10101111     159
  Number 2 :  00100101     37
  Number 3 :  00001101     13
  Number 4 :  10011001     153
  Number 5 :  01001001     73
  Number 6 :  01000001     65
  Number 7 :  00011111     31
  Number 8 :  00000001     1
*/
/*In order to display zero your array has decimal number 3. However using the shift register
  it is actuall the binary number that is uploaded into the resitsre: zie the table above for zero: 00000011.
*/

int datArray[9] = {2, 158, 36, 12, 152, 72, 64, 30, 0}; // array without the decimal

void setup ()

{
  //set pins to output
  Serial.begin(9600);

  pinMode(irObstaclePin, INPUT);
  //Voor de konppen
  pinMode(knopr, INPUT_PULLUP);
  pinMode(knopl, INPUT_PULLUP);
  //voor de ledjes in de konop
  pinMode(knopl_led, OUTPUT);
  pinMode(knopr_led, OUTPUT);
  //voor de rode led
  pinMode(led_lift, OUTPUT);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}
void loop()


{
  Obstacle = digitalRead(irObstaclePin);
  buttonstate_l = digitalRead(knopl);
  buttonstate_r = digitalRead(knopr);

/*
  Serial.print(knopr);
  Serial.print(" r ");
  Serial.println(buttonstate_r);
  Serial.print(knopl);
  Serial.print(" l ");
  Serial.println(buttonstate_l);
  //delay(100);
*/

  //Serial.println(Obstacle);

  if ( buttonstate_l == 0) {
    digitalWrite(knopl_led, HIGH);
  }


  if ( buttonstate_r == 0) {
    digitalWrite(knopr_led, HIGH);
  }

  if (Obstacle == LOW) {
    waar = 1 ;
    digitalWrite(led_lift, HIGH);
    digitalWrite(knopl_led, LOW);
    digitalWrite(knopr_led, LOW);
    //Serial.println("OBSTACLE!!, OBSTACLE!!");

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 159);
    digitalWrite(latchPin, HIGH);

  } else {
    waar = 1 ;
    digitalWrite(led_lift, LOW);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 3);
    digitalWrite(latchPin, HIGH);
    //Serial.println("clear");
  }

}
