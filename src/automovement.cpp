#include "vex.h"

float globalIntakeAngle;
class autoMovement{
  float wheelDiameter = 3.0f;
  float $PI = 3.141592653589;
  motor LeftMotor = motor(PORT11, ratio18_1, false);
  motor RightMotor = motor(PORT20, ratio18_1, true);
  motor LeftIntake = motor(PORT2, ratio18_1, true);
  motor RightIntake = motor(PORT1, ratio18_1, false);
  motor_group driveMotors = motor_group(LeftMotor, RightMotor);
  motor_group intakeMotors = motor_group(LeftIntake,RightIntake);
  gyro Gyro = gyro(Brain.ThreeWirePort.E);
  public:
  void TurnRight(float degrees,float speed);
  void TurnLeft(float degrees,float speed);
  void DriveForward(float inches, float speed);
  void SetIntakes(float percentOpen, int timeout);
  void UpdatePosition(float Distance);

  int x = 0;
  int y = 0;
};

void autoMovement::TurnRight(float degrees,float speed){
  float originalHeading = Gyro.heading(deg);
  Gyro.setHeading(0,deg);
  LeftMotor.spin(forward,speed*100,pct);
  RightMotor.spin(reverse,speed*100,pct);
  waitUntil(Gyro.heading()>=degrees);
  LeftMotor.stop();
  RightMotor.stop();
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
  LeftMotor.spin(reverse,speed*100,pct);
  RightMotor.spin(forward,speed*100,pct);
  waitUntil(fabs(Gyro.heading(deg)-360)>=degrees);
  LeftMotor.stop();
  RightMotor.stop();
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