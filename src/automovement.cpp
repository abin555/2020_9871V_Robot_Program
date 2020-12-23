#include "vex.h"
#include "robot-config.h"

float globalIntakeAngle;
class autoMovement{
  float wheelDiameter = 3.0f;
  float $PI = 3.141592653589;
  motor_group driveMotors = motor_group(leftMotor, rightMotor);
  motor_group intakeMotors = motor_group(leftIntake,rightIntake);
  public:
  void TurnRight(float degrees,float speed);
  void TurnLeft(float degrees,float speed);
  void DriveForward(float inches, float speed);
  void SetIntakes(float percentOpen, int timeout);
};

void autoMovement::TurnRight(float degrees,float speed){
  float originalHeading = Gyro.heading(deg);
  Gyro.setHeading(0,deg);
  leftMotor.spin(forward,speed*100,pct);
  rightMotor.spin(reverse,speed*100,pct);
  waitUntil(Gyro.heading()>=degrees);
  leftMotor.stop();
  rightMotor.stop();
  if(originalHeading+degrees>=360){
    Gyro.setHeading(originalHeading+degrees-360,deg);
  }
  else{
    Gyro.setHeading(originalHeading+degrees,deg);
  }
  task::sleep(500);
}

void autoMovement::TurnLeft(float degrees, float speed){
  float originalHeading = Gyro.heading(deg);
  Gyro.setHeading(0,deg);
  leftMotor.spin(reverse,speed*100,pct);
  rightMotor.spin(forward,speed*100,pct);
  waitUntil(fabs(Gyro.heading(deg)-360)>=degrees);
  leftMotor.stop();
  rightMotor.stop();
  if(originalHeading+degrees>=360){
    Gyro.setHeading(originalHeading+degrees-360,deg);
  }
  else{
    Gyro.setHeading(originalHeading+degrees,deg);
  }
  task::sleep(500);
}

void autoMovement::DriveForward(float inches, float speed){
  float targetAngle = inches*360/($PI*wheelDiameter);
  driveMotors.setPosition(0,deg);
  driveMotors.setVelocity(speed*100,pct);
  waitUntil(driveMotors.position(degrees)>=targetAngle);
  driveMotors.stop();
  task::sleep(500);
}

void autoMovement::SetIntakes(float percentOpen,int timeout){
  globalIntakeAngle = 180*(percentOpen/100);
  intakeMotors.spinToPosition(globalIntakeAngle,deg);
  //task::sleep(timeout);
  //autoIntakes.interrupt();
}