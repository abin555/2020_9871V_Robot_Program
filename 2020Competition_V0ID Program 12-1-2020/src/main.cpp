/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Aidan Boisvert                                   */
/*    Created:      Tue Oct 13 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// rightIntake          motor         1               
// leftIntake           motor         2               
// Elevator             motor         3               
// rightMotor           motor         20              
// leftMotor            motor         11              
// rampMotor            motor         10              
// rightIntakeSwitch    limit         A               
// leftIntakeSwitch     limit         B               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;
competition Competition;
motor_group IntakeMotors = motor_group(leftIntake,rightIntake);
controller::lcd screen = Controller1.Screen;

void pre_auton(void){

}
void autonomous(void){

}

bool Breaks = false;
float speed_mod = 0.75; 
float elevator_mod = 1;
bool Intakes = false;
bool elevator = false;
bool elevatormode = false;
bool elevatordir = false;

void ControllerScreenUpdater(bool Breaks, float Speed, float elevator,  bool IntakeSet){
  screen.clearScreen();
  screen.setCursor(0,0);
  //Drive System Status
  if(Breaks){
    screen.print("Held ");
  }
  else{
    screen.print("Free ");
  }
  screen.setCursor(5,0);
  screen.print(Speed*100);
  //Intake System Status
  screen.setCursor(0,1);
  if(IntakeSet){
    screen.print("Intakes running");
  }
  else{
    screen.print("Intakes stopped");
  }
  //Elevator System
  screen.setCursor(0,2);
  screen.print(elevator*100);
}

void Drive(float stick_up, float stick_side, float speed_mod){//Using Arcade Control
  leftMotor.spin(forward, speed_mod*((stick_up + stick_side)/2), velocityUnits::pct);  //speed(up+over)/2
  rightMotor.spin(forward, speed_mod*((stick_up - stick_side)/2), velocityUnits::pct);  //speed(up-over)/2
}

void ResetIntakes(){
  while(!rightIntakeSwitch.pressing()){
    rightIntake.spinFor(reverse,10,degrees);
  }
  while(!leftIntakeSwitch.pressing()){
    leftIntake.spinFor(reverse,10,degrees);
  }
  rightIntake.setPosition(0,degrees);
  rightIntake.setPosition(0,turns);
  leftIntake.setPosition(0,degrees);
  leftIntake.setPosition(0,turns);
}
void OperateIntakes(void){
  if(Intakes){
    if(leftIntakeSwitch.pressing() || rightIntakeSwitch.pressing()){//push out
      IntakeMotors.spinToPosition(180,degrees); 
    }
    else{//pull in
      IntakeMotors.spinToPosition(0,degrees);
    }
  }
}
void usercontrol(void){
  ResetIntakes();
  thread IntakeThread = thread(OperateIntakes);
  ControllerScreenUpdater(Breaks,speed_mod, elevator_mod, Intakes);
  while(true){
    if(!Breaks){
      Drive(Controller1.Axis3.value(),Controller1.Axis4.value(),speed_mod);
    }
    if(Controller1.ButtonA.pressing()){
      Breaks = !Breaks;
      ControllerScreenUpdater(Breaks,speed_mod, elevator_mod, Intakes);
      while(Controller1.ButtonA.pressing()){
        task::sleep(1);
      }
    }
    if(elevatormode == false){
      Elevator.spin(forward,elevator_mod*Controller1.Axis2.value(),velocityUnits::pct);
    }
    else{
      if(elevator == true){
        if(elevatordir == false){
          Elevator.spin(forward,elevator_mod,velocityUnits::pct);
        }
        else{
          Elevator.spin(reverse,elevator_mod,velocityUnits::pct);
        }
      }
    }
    if(Controller1.ButtonB.pressing()){
      elevatordir = !elevatordir;
      while(Controller1.ButtonB.pressing()){
        task::sleep(1);
      }
    }
    if(Controller1.ButtonY.pressing()){
      elevator = !elevator;
      while(Controller1.ButtonY.pressing()){
        task::sleep(1);
      }
    }
    if(Controller1.ButtonRight.pressing()){
      elevatormode = !elevatormode;
      while(Controller1.ButtonRight.pressing()){
        task::sleep(1);
      }
    }

    if(Controller1.ButtonX.pressing()){
      Intakes = !Intakes;
      ControllerScreenUpdater(Breaks,speed_mod, elevator_mod, Intakes);
      while(Controller1.ButtonX.pressing()){
       task::sleep(1);
      }
    }
    if(Controller1.ButtonL1.pressing()){
      speed_mod += 0.5;
      ControllerScreenUpdater(Breaks,speed_mod, elevator_mod, Intakes);
      task::sleep(10);
    }
    if(Controller1.ButtonL2.pressing()){
      speed_mod -= 0.5;
      ControllerScreenUpdater(Breaks,speed_mod, elevator_mod, Intakes);
      task::sleep(10);
    }
    if(Controller1.ButtonR1.pressing()){
      elevator_mod += 0.5;
      ControllerScreenUpdater(Breaks,speed_mod, elevator_mod, Intakes);
      task::sleep(10);
    }
    if(Controller1.ButtonR2.pressing()){
      elevator_mod -= 0.5;
      ControllerScreenUpdater(Breaks,speed_mod, elevator_mod, Intakes);
      task::sleep(10);
    }
    if(Controller1.ButtonDown.pressing()){
      rampMotor.spinToPosition(0,degrees);
    }
    if(Controller1.ButtonUp.pressing()){
      rampMotor.spinToPosition(880,degrees);
    }
    wait(20, msec);
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  Competition.drivercontrol(usercontrol);
  Competition.autonomous(autonomous);
  pre_auton();
  while (true) {
    wait(100, msec);
  }
}
