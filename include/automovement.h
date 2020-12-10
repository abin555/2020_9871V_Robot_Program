#include "vex.h"
#include "robot-config.h"
#include <position.h>

class autoMovement{
  public:

  void TurnRight(float degrees,float speed);
  void TurnLeft(float degrees,float speed);
  void DriveForward();
};