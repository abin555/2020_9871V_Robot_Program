#include "vex.h"
#include "robot-config.h"
#include <position.h>

class autoMovement{
  public:

  void TurnRight(float degrees,float speed);
  void TurnLeft(float degrees,float speed);
  void DriveForward();
};

void autoMovement::TurnRight(float degrees,float speed){
  float heading1 = Gyro.heading();
  float target = heading1+degrees;
  bool special = false;
  if(target > 360){
    target=target-360;
    special = true;
  }
  leftMotor.spin(forward,speed*100,pct);
  rightMotor.spin(reverse,speed*100,pct);
  if(!special){
    waitUntil(Gyro.heading()>target);
  }
  else{
    waitUntil(Gyro.heading()<target);
    waitUntil(Gyro.heading()>target);
  }
  leftMotor.stop();
  rightMotor.stop();
}