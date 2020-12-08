#include "vex.h"
#include "robot-config.h"

class Autonomous{
  private:
  smartdrive Drivetrain = smartdrive(leftMotor, rightMotor, Gyro, 319.19, 320, 40, mm, 1);
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
  if(direction){
    Drivetrain.turnFor(degrees, rotationUnits::deg,true);
  }
  else{
    Drivetrain.turnFor(-degrees, rotationUnits::deg,true);
  }
}
float Autonomous::GetHeading(){
  return Gyro.heading(degrees);
}
void Autonomous::Drive(bool direction, float length){
  if(direction){
    Drivetrain.driveFor(forward,length,distanceUnits::in);
  }
  else{
    Drivetrain.driveFor(reverse,length,distanceUnits::in);
  }
}
void Autonomous::openIntakes(){
  IntakeMotors.spinToPosition(180,degrees);
}
void Autonomous::closeIntakes(){
  IntakeMotors.spinToPosition(0,degrees);
}
void Autonomous::runElevator(bool enable, bool direction, float speed){
  if(enable){
    if(direction){
      Elevator.spin(forward,speed,pct);
    }
    else{
      Elevator.spin(reverse,speed,pct);
    }
  }
  else{
    Elevator.stop();
  }
}

