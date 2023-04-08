#include <Wire.h>
const int latchPin = 10;//Pin connected to ST_CP of 74HC595
int Buttonled1 = 2;  
int Buttonpress1 = 3;//button 1 = up
int Buttonled2 = 4;
int Buttonpress2 = 5;//button 2 = down
const int clockPin = 8;//Pin connected to SH_CP of 74HC595 
const int dataPin = 11; //Pin connected to DS of 74HC595 
int LED = 7; // Use the onboard Uno LED
int obstaclePin = 6;  // This is our input pin
int hasObstacle = HIGH;  // HIGH MEANS NO OBSTACLE

int seven;


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

To display the dot, simply subtract 1.
example : 1 - 1 = 0  or b0000000 0x0 to display 8
*/
/*In order to display zero your array has decimal number 3. However using the shift register
it is actuall the binary number that is uploaded into the resitsre: zie the table above for zero: 00000011. 
*/
int datArray[9] = {3, 159, 37, 13, 153, 73, 65, 31, 1}; // array without the decimal

void setup ()
{
  Wire.begin(2);
  Wire.onRequest(gegevens_naar_de_master);
  Wire.onReceive(receiveEvent);
  //set pins to output
  pinMode(Buttonled1, OUTPUT);       //creating button and led in- and outputs 
  pinMode(Buttonpress1, INPUT);
  pinMode(Buttonled2, OUTPUT);
  pinMode(Buttonpress2, INPUT);
  pinMode(LED, OUTPUT);             //creating IR sensor pin in and outputs
  pinMode(obstaclePin, INPUT);      //IR sensor pin detects objects
  Serial.begin(9600);  
  pinMode(latchPin,OUTPUT);         //creating outputs for shift register to connect to the arduino
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
}
void loop()
{
  if (digitalRead(obstaclePin) == LOW) {      //if IR sensor detects capsule then:
    digitalWrite(latchPin,HIGH);              //
    digitalWrite(latchPin,LOW);               //
    shiftOut(dataPin,clockPin,MSBFIRST,datArray[seven]);   //7 segment display displays number 3 with dot
    digitalWrite(LED, HIGH);                  //elevator led turns on
    digitalWrite(Buttonled1, LOW);            //button led's get turned off
    digitalWrite(Buttonled2, LOW);            //
  }
  else if (digitalRead(Buttonpress1) == HIGH){ //if button 1 get pressed then:
      digitalWrite(Buttonled1, HIGH);         //button led turns on and stays on (both 1 and 2 can be pressed simultaniously)
      digitalWrite(LED, LOW);                 //Elevator led will be turned off since it can only be on when capsule is detected
  }
  else if (digitalRead(Buttonpress2) == HIGH){//if button 2 get pressed then:
      digitalWrite(Buttonled2, HIGH);         //button led turns on and stays on
      digitalWrite(LED, LOW);                 //Elevator led will be turned off since it can only be on when capsule is detected
  }
  else if (digitalRead(obstaclePin) == HIGH){ //if IR sensor doesnt detect elevator capsule or any of the buttons get pressed then:
    digitalWrite(latchPin,LOW);               //7 segment display displays 0
    shiftOut(dataPin,clockPin,MSBFIRST,datArray[seven]);    //
    digitalWrite(latchPin,HIGH);              //
    digitalWrite(LED, LOW);                   //elevator led will be turned off
  }
  } 

  void gegevens_naar_de_master()
  {
    Wire.write(digitalRead(obstaclePin));
    Wire.write(digitalRead(Buttonled2));
    Wire.write(digitalRead(Buttonled1));
  }
  void receiveEvent(){
    seven=Wire.read();
  }
  

  
