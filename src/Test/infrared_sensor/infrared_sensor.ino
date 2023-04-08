#define irObstaclePin 8  // This is our input pin
int Obstacle = HIGH;  // HIGH MEANS NO OBSTACLE
void setup() 
{
 pinMode(irObstaclePin, INPUT);
 Serial.begin(9600);
}
void loop() 
{
 Obstacle = digitalRead(irObstaclePin);
 if (Obstacle == LOW)
 {
   Serial.println("OBSTACLE!!, OBSTACLE!!");  
 }
 else
 {
   Serial.println("clear");
 }
 delay(200);
}
