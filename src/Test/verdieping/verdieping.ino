#include <Wire.h>

const int latchPin = 12;//Pin connected to ST_CP of 74HC595
const int clockPin = 10;//Pin connected to SH_CP of 74HC595
const int dataPin = 11; //Pin connected to DS of 74HC595

#define led_lift 8

#define knopl 4
#define knopr 3

#define knopl_led 7 //omhoog 
#define knopr_led 6 //omlaag

int buttonstate_l = LOW;
int buttonstate_r = LOW;

#define irObstaclePin 9  // This is our input pin

int waar;

int Obstacle = HIGH;

bool omhoog = false;
bool omlaag = false;

int seven;

/*
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
//-----------------0---1----2----3---4----5---6---7---8
int datArray[9] = {3, 159, 37, 13, 153, 73, 65, 31, 1}; // array without the decimal



void setup ()

{
  //set pins to output
  Wire.begin(2);
  Wire.onRequest(gegevens_naar_de_master); // register event
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);

  pinMode(irObstaclePin, INPUT);
  //Voor de konppen. INPUT_PULLUP is het omgekeer de van een input 1 = 0 dus je kijkt of de knop LOW is
  pinMode(knopr, INPUT_PULLUP);
  pinMode(knopl, INPUT_PULLUP);
  //voor de ledjes in de konop
  pinMode(knopl_led, OUTPUT);
  pinMode(knopr_led, OUTPUT);
  //voor de rode led
  pinMode(led_lift, OUTPUT);
  //voor 7_segment display
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

}
void loop()


{

  Obstacle = digitalRead(irObstaclePin);
  buttonstate_l = digitalRead(knopl);//omhoog
  buttonstate_r = digitalRead(knopr);//omlaag

  Serial.println(seven);

  /*
    Serial.print(knopr);
    Serial.print(" r ");
    Serial.println(buttonstate_r);
    Serial.print(knopl);
    Serial.print(" l ");
    Serial.println(buttonstate_l);
  */

  delay(100);



  if (omhoog == true) {
    Serial.print(" l ");
    Serial.println(" omhoog");
  }

  if (omlaag == true) {
    Serial.print(" r ");
    Serial.println(" omlaag");

  }


  if ( buttonstate_l == 0) {
    omhoog = true;
    digitalWrite(knopl_led, HIGH);//omhoog
  }

  if ( buttonstate_r == 0) {
    omlaag = true;
    digitalWrite(knopr_led, HIGH);//omlaag

  }

  if (Obstacle == LOW) {
    waar = 1 ;
    omhoog = false;
    omlaag = false;
    digitalWrite(led_lift, HIGH);
    digitalWrite(knopl_led, LOW);
    digitalWrite(knopr_led, LOW);
    //Serial.println("OBSTACLE!!, OBSTACLE!!");

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, datArray[seven]);
    digitalWrite(latchPin, HIGH);



  } else {
    waar = 1 ;
    digitalWrite(led_lift, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, datArray[seven]);//datArray[seven]
    digitalWrite(latchPin, HIGH);
    //Serial.println("clear");

  }

}
void gegevens_naar_de_master() {
  Wire.write(Obstacle);
  Wire.write(omhoog);
  Wire.write(omlaag);

}
void receiveEvent() {
  seven = Wire.read(); // receive byte as a character

}
