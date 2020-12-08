#include "vex.h"
#include "robot-config.h"

class Autonomous{
  public:
    void Turn(bool direction, float degreees);
    float GetHeading();
    void Drive(bool direction, float length);
    void openIntakes();
    void closeIntakes();
    void runElevator(bool enable, bool direction, float speed);
};