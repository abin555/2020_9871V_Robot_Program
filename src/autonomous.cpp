#include "vex.h"
#include "robot-config.h"

class Autonomous{
  private:
  motor_group IntakeMotors = motor_group(leftIntake,rightIntake);
  public:
    void Turn(bool direction, float degreees);
    float GetHeading();
    void Drive(bool direction, float length);
    void openIntakes();
    void closeIntakes();
    void runElevator(bool elevator, bool direction, float speed);
};

void Autonomous::Turn(bool direction, float degrees){
}
float Autonomous::GetHeading(){
  return Gyro.heading(degrees);
}
void Autonomous::Drive(bool direction, float length){
}
void Autonomous::openIntakes(){
  IntakeMotors.spinToPosition(180,degrees);
}
void Autonomous::closeIntakes(){
  IntakeMotors.spinToPosition(0,degrees);
}
void Autonomous::runElevator(bool enable, bool direction, float speed){
}

