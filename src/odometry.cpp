#include "vex.h"
class Odometry{
  private:
    double deltaX = 0;
    double deltaY = 0;
    double deltaHeading = 0;
    double prevX = 0;
    double prevY = 0;
    double prevHeading = 0;
    double leftPrevAngle = 0;
    double rightPrevAngle = 0;
  public:
    double X = 0;
    double Y = 0;
    double heading = 0;
    void OdometryAlgorithm();
};
double $PI = 3.1415;
#define wheelRadius 3;

void Odometry::OdometryAlgorithm(){
  double LAngle = leftMotor.position(degrees);
  double RAngle = rightMotor.position(degrees);

  double deltaLeftDistance = ((LAngle - leftPrevAngle) * $PI / 180) * wheelRadius;
  double deltaRightDistance = ((RAngle - rightPrevAngle) * $PI / 180) * wheelRadius;

  leftPrevAngle = LAngle;
  rightPrevAngle = RAngle;
}
