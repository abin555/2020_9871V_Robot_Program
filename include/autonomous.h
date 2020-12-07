#include "vex.h"
#include "robot-config.h"

class Autonomous{
  public:
    void Turn(bool direction, float degreees);
    float GetHeading();
    void Drive(bool direction, float length);
    void OpenIntakes();
    void CloseIntakes();
    void runElevator(bool elevator, bool direction, float speed);
};