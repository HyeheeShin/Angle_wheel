#include <Servo.h>
Servo dlt;
Servo drt;
Servo dlb;
Servo drb;
 // Define wheel control pins
 int dlt_control = 9;
 int dlb_control = 10;
 int drt_control = 11;
 int drb_control = 12;      
 int Turns[4] = {0, 0, 0, 0};     //The number of rotations
 int i = 0;
 float incrementSpeed = 0.01;
 float speedlt = 80;
 float speedlb = 80;
 float theta[4] = {0, 0, 0, 0};   // The theta[i] of this measurement
 float thetaP[4] = {0, 0, 0, 0};  //The previous theta[i]
 int count = 0;     // Count for the number of measurements; the number of times Angle is called
 int kp = 1;
void setup() {
  Serial.begin(9600);   
  dlt.attach(dlt_control);
  dlb.attach(dlb_control);
  drt.attach(drt_control);
  drb.attach(drb_control);
     dlt.write(90);

}

int readPos(int pwmPin)
{
 i = pwmPin - 3;
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
 float angle[4]={0, 0, 0, 0};     // the angle of rotation (infinite)

 // While loop calculates the time "on" and time "off" until tCycle is between 1050 and 1150 (average is 1100)
 while(1) {
   tHigh = pulseIn(pwmPin, HIGH); // receive signal on
   tLow =  pulseIn(pwmPin, LOW);  // receive signal off
   tCycle = tHigh + tLow;
   if ((tCycle > 1050) && ( tCycle < 1150)) break;
 }
 // Equation used to calculate duty cycle and theta[i] (where theta[i] is between 0 and 360)
 dc = (dutyScale * tHigh) / tCycle;
 theta[i] = ((dc - dcMin) * unitsFC) / (dcMax - dcMin);
   Serial.print(i);
    Serial.print("\t");

   //Serial.print(theta[i]);

  // Set boundaries so theta[i] is in the range of 0 and 360  
  if(theta[i] < 0) theta[i] = 0;
  else if(theta[i] > (unitsFC - 1)) theta[i] = unitsFC - 1;

  // Incrementing or decrementing number of Turns[i]
  if((theta[i] < q2min) && (thetaP[i] > q3max)){ // If 4th to 1st quadrant
    Turns[i]++; 
  }// Increment Turns[i] count
  else if((thetaP[i] < q2min) && (theta[i] > q3max)) // If in 1st to 4th quadrant
  Turns[i]--; // Decrement Turns[i] count

  // Change the angle by theta[i] based on Turns[i] (add if positive Turns[i], subtract if negative Turns[i])
  if(Turns[i] >= 0)
    angle[i] = (Turns[i] * unitsFC) + theta[i];
  else if(Turns[i] < 0)
    angle[i] = ((Turns[i] + 1) * unitsFC) - (unitsFC - theta[i]);

     Serial.print(Turns[i]);
// Serial.print(theta[i]-thetaP[i]);
  Serial.print("\t");
   Serial.print(angle[i]);

 Serial.print("\t");


/*Serial.print(pwmPin);   
 Serial.print("   Turns[i] = ");
 Serial.print(Turns[i]);
 Serial.print("\t");
 Serial.print(" Angle = ");
 Serial.print(angle);
 Serial.println();*/
 // Set the current theta[i], as thetaP[i] of the next round
 thetaP[i] = theta[i];
//  Serial.print(theta[i]-thetaP[i]);
 // Serial.print("\t");
 return angle[i];

}
void loop() {
  dlt.write(speedlt);
  dlb.write(speedlb);
  int angledlt_current = readPos(3);
   Serial.print("\t");

 int angledlb_current = readPos(4);

while(angledlt_current!=angledlb_current) {
  if (angledlt_current - angledlb_current > 20) {
    speedlt = speedlt + incrementSpeed;
    speedlb = speedlb - incrementSpeed;

    Serial.print(" lt+ ");
    //Serial.print(speedlt);
    //Serial.println();
    break;
  }
  else if (angledlt_current - angledlb_current < -20) {
    speedlb = speedlb + incrementSpeed;
    speedlt = speedlt -incrementSpeed;

    Serial.print(" lb+ ");
    //Serial.print(speedlb);
      //  Serial.println();
        break;

  }
  else {
    Serial.print(" == ");    
    break;
  }
  

}

  Serial.print("lt = ");
    Serial.print(speedlt);
     Serial.print("\t");
     Serial.print("lb = ");
    Serial.print(speedlb);
         Serial.print("\t");
  Serial.print(angledlt_current - angledlb_current);
    Serial.println();
    
  /*Serial.print(angledlt_current);
  Serial.println();
  Serial.print(angledlb_current);
  Serial.println();*/
}
