#include "vex.h"

class autoMovement{
  float wheelDiameter = 3.0f;
  float $PI = 3.141592653589;
  motor LeftMotor;
  motor RightMotor;
  motor LeftIntake;
  motor RightIntake;
  motor_group driveMotors;
  motor_group intakeMotors;
  gyro Gyro;
  public:
  void TurnRight(float degrees,float speed);
  void TurnLeft(float degrees,float speed);
  void DriveForward(float inches, float speed);
  void SetIntakes(float percentOpen, int timeout);

  int x;
  int y;
};