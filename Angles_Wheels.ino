#include <Servo.h>
Servo myservo;      //Servo is "myservo" hi
//int pos = 91;
 int Turns = 0;     //The number of rotations
 float thetaP = 0;  //The previous theta
 float theta = 0;   // The theta of this measurement
 int count = 0;     // Count for the number of measurements; the number of times Angle is called
 int kp = 1;
void setup() {
  Serial.begin(9600);   
  myservo.attach(9);  // Attach to pin 9
  
}

void servo_speed(pinControl, set_speed){
  //CREATE
  
}
int readPos(int pwmPin, int targetAngle)
{
 int tHigh;           // time "on"
 int tLow;            // time "off"
 int tCycle;          // length of duty cycle
 int q2min = 90;      // lower limit of q2
 int q3max = 270;     // Upper limit of q3
 float dc = 0;        // duty cycle
 int unitsFC = 360;   // 360 degrees in a circle
 float dcMin = 0.029; // Minimum dc (data sheet)
 float dcMax = 0.971; // maximum dc (data sheet)
 float dutyScale = 1;
 float angle = 0;     // the angle of rotation (infinite)

 // While loop calculates the time "on" and time "off" until tCycle is between 1050 and 1150 (average is 1100)
 while(1) {
   tHigh = pulseIn(pwmPin, HIGH); // receive signal on
   tLow =  pulseIn(pwmPin, LOW);  // receive signal off
   tCycle = tHigh + tLow;
   /*Serial.print(tHigh);
   Serial.print("\t");
   Serial.print(tLow);
   Serial.print("\t");
   Serial.print(tCycle);
   Serial.println();*/
   if ((tCycle > 1050) && ( tCycle < 1150)) break;
 }
 // Equation used to calculate duty cycle and theta (where theta is between 0 and 360)
 dc = (dutyScale * tHigh) / tCycle;
 theta = ((dc - dcMin) * unitsFC) / (dcMax - dcMin);
 
   //Serial.print(theta);
   //Serial.println();

  // Set boundaries so theta is in the range of 0 and 360  
  if(theta < 0) theta = 0;
  else if(theta > (unitsFC - 1)) theta = unitsFC - 1;

  // Incrementing or decrementing number of turns
  if((theta < q2min) && (thetaP > q3max)) // If 4th to 1st quadrant
  Turns++; // Increment turns count
  else if((thetaP < q2min) && (theta > q3max)) // If in 1st to 4th quadrant
  Turns--; // Decrement turns count

  // Change the angle by theta based on turns (add if positive turns, subtract if negative turns)
  if(Turns >= 0)
    angle = (Turns * unitsFC) + theta;
  else if(Turns < 0)
    angle = ((Turns + 1) * unitsFC) - (unitsFC - theta);
    
 Serial.print("Turns = ");
 Serial.print(Turns);
 Serial.print("\t");
 Serial.print(" Angle = ");
 Serial.print(angle);

 // Set the current theta, as thetaP of the next round
 thetaP = theta;

// Control Function

  // Find the variance in target angle and angle
  float errorAngle = targetAngle - angle;
  
  // Multiply the error by Kp (for PID controller)
  float output = errorAngle * Kp;

  // Set boundaries for speed
  if(output > 200) output = 200;
  if(output < -200) output = -200;

  // If errorAngle is positive, speed up
  // If errorAngle is negative, slow down
  // Else speed is correct
  
  if(errorAngle > 0)
    offset = 30;
  else if(errorAngle < 0)
    offset = -30;
  else
    offset = 0;
    
  // Set the speed
  servo_speed(pinControl, output + offset);
}



void loop() {
  int target = 720;
  myservo.write(75);  // Run servo
  readPos(13);        // Feedback at pin 13
  delay(1.1);         
  count++;            // Increment the number of count by 1 (for testing)
  Serial.print("\t");
  Serial.print(count);
  Serial.println();

}
/*void loop() {
  for (pos = 0; pos <= 360; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.print(pos);
    Serial.println();
    delay(100);                       // waits 15ms for the servo to reach the position
  }
    for (pos = 360; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.print(pos);
    Serial.println();
    delay(100);                       // waits 15ms for the servo to reach the position
  }
}*/
