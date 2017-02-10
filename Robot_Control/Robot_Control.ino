#include <Servo.h>

#include <Wire.h>

byte bytes[3];

//Servo wires
//Brown - Ground
//Red - Power
//Orange - Signal


Servo cam2;
Servo forL;
Servo forR;

int forLPin = 4; //left stick combo
int forRPin = 5; //left stick combo
int cam2Pin = 9; //right stick y

int cam2Angle = 90;

void setup() {
  Serial.begin(9600);

  forL.attach(forLPin);
  forR.attach(forRPin);

  cam2.attach(cam2Pin);
  cam2.write(cam2Angle);
  
  delay(2000);//in milliseconds
}

void loop() {
  if(Serial.available() >= 3) {
     while(Serial.read()!=250){} //only start taking data if it begins with this flag byte
     for(int i = 0; i< 3; i++) {
        bytes[i] = Serial.read();
        if(bytes[i]==250) { //throw the array away if there's a 250 in it (as 250 is invalid)
          i=-1;
          for(int j = 0; j< 3; j++)bytes[j]=0;
        }
        //0: Left Motor Y, 0-100
        //1: Right Motor X, 0-100
        //2: Turn Y, 0-180
     }
     //for(int i=0; i<16; i++) { //send data back to java so we know what we're doing
       //Serial.print(bytes[i]);
       //Serial.print("\t");
     //}
     //Serial.println();
     
     differentialDrive(bytes[1], bytes[0]);
     
     cam2Angle = updateAngle(bytes[2], cam2Angle, 2);
     cam2Angle = constrain(cam2Angle, 60, 90);
     cam2.write(cam2Angle);
     //Serial.print("Camera Angles: ");
     //Serial.print(cam2Angle);
     //Serial.print(" ");
  }
  delay(5); //just to make sure we don't update TOO fast
}

/*this will change an angle. It's just here so I
 *don't have to write the same three lines over and over
 */
int updateAngle(int input, int angle, int increment) {
  if(input > 60 && angle <= 150) angle+=increment;
  else if(input < 40 && angle >= 30) angle -=increment;
  return angle;
}


/*this will change angles on the servos of the claw's
 *arm and hand. It's just here so I
 *don't have clutter the body of loop()
 */
int servoIncrement(int b1, int b2, int angle) {
  if((b1 == 1 && b2 == 0) && (angle <= 160)) angle+=5;
  else if((b1 == 0 && b2 == 1) && (angle >= 20)) angle-=5;
  return angle;
}

/*this will take a combo of x and y vals from a joystick
 *and turn it into motion. Pin params aren't necessary, as
 *this will only ever control forL and forR
 */
void differentialDrive(int turn, int thrust) {
  turn -= 50;
  int rMotor = 100-constrain(thrust + turn,0,100);
  int lMotor = 100-constrain(thrust - turn,0,100);
  forR.write(map(rMotor,0,100,75,105)+3);
  forL.write(map(lMotor,0,100,75,105)+3);//TRUST THE MATH, IT'S TOO COMPLEX FOR MY BRAIN
//  DEBUGGING
//  Serial.print("R: ");
//  Serial.print(map(rMotor,0,100,75,105)); 
//  Serial.print(" L: ");
//  Serial.println(map(lMotor,0,100,75,105));
}
