#include "vex.h"
#include "robot-config.h"

class Autonomous{
  private:
  drivetrain Drivetrain = drivetrain(leftMotor, rightMotor, 319.19, 295, 40, mm, 1);
  motor_group IntakeMotors = motor_group(leftIntake,rightIntake);
  public:
    void Turn(bool direction, float degreees);
    float GetHeading();
    void Drive(bool direction, float length);
    void OpenIntakes();
    void CloseIntakes();
    void runElevator(bool elevator, bool direction, float speed);
};

void Autonomous::Turn(bool direction, float degrees){
  
}
float Autonomous::GetHeading(){
  return 0.0f;
}
void Autonomous::Drive(bool direction, float length){
  if(direction){
    Drivetrain.driveFor(forward,length,distanceUnits::in);
  }
  else{
    Drivetrain.driveFor(reverse,length,distanceUnits::in);
  }
}
void Autonomous::OpenIntakes(){
  IntakeMotors.spinToPosition(180,degrees);
}
void Autonomous::CloseIntakes(){
  IntakeMotors.spinToPosition(0,degrees);
}
void Autonomous::runElevator(bool elevator, bool direction, float speed){

}

