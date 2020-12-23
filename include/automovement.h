#include "vex.h"
#include "robot-config.h"
#include <position.h>

class autoMovement{
  float wheelDiameter;
  float $PI;
  motor_group driveMotors;
  motor_group intakeMotors;
  public:
  void TurnRight(float degrees,float speed);
  void TurnLeft(float degrees,float speed);
  void DriveForward(float inches, float speed);
  void SetIntakes(float percentOpen, int timeout);
};