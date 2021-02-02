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
// rightIntakeSwitch    limit         A               
// leftIntakeSwitch     limit         B               
// accelX               accelerometer C               
// accelY               accelerometer D               
// GyroOLD              gyro          E               
// GyroB                inertial      12              
// Gyro                 gyro          F               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "math.h"

using namespace vex;
competition Competition;
motor_group IntakeMotors = motor_group(leftIntake,rightIntake);
motor_group driveM = motor_group(leftMotor,rightMotor);
#define screen Controller1.Screen
#define $PI 3.141592653589

void pre_auton(void){/*Pre_auton setup*/}

float globalIntakeAngle;
class autoMovement{
  float wheelDiameter = 3.0f;
  public:
  motor_group driveMotors = motor_group(leftMotor, rightMotor);
  motor_group intakeMotors = motor_group(leftIntake,rightIntake);
  void TurnRight(float degrees,float speed);
  void TurnLeft(float degrees,float speed);
  void DriveForward(float inches, float speed);
  void SetIntakes(float percentOpen, int timeout);
  void Precise90Turn(bool dir);
  void Ready();
  int x = 0;
  int y = 0;
};

void rightPreciseThreadTurn(){
  rightMotor.spinToPosition(290.60,degrees);
}
void leftPreciseThreadTurn(){
  leftMotor.spinToPosition(274.60,degrees);
}
void autoMovement::Precise90Turn(bool dir){
  if(!dir){//left
    leftMotor.setPosition(0,degrees);
    rightMotor.setPosition(0,degrees);
    thread rightTurn = thread(rightPreciseThreadTurn);
    leftMotor.spinToPosition(-340.40,degrees);
  }
  else{
    leftMotor.setPosition(0,degrees);
    rightMotor.setPosition(0,degrees);
    thread leftTurn = thread(leftPreciseThreadTurn);
    rightMotor.spinToPosition(-340.40,degrees);
  }
}

void autoMovement::TurnRight(float Degrees,float speed){
  //Gyro.calibrate();
  //waitUntil(!Gyro.isCalibrating());
  Gyro.setHeading(0,deg);
  leftMotor.spin(forward,speed,percent);
  rightMotor.spin(reverse,speed,percent);
  while(Gyro.heading(degrees) < Degrees-1 || Gyro.heading(degrees) > Degrees+1){
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(0,0);
    Controller1.Screen.print(Gyro.heading(degrees));
    Controller1.Screen.setCursor(1,0);
  } 
  leftMotor.stop();
  rightMotor.stop();
  task::sleep(500);
}

void autoMovement::TurnLeft(float Degrees, float speed){
  //Gyro.calibrate();
  //waitUntil(!Gyro.isCalibrating());
  Gyro.setHeading(0,deg);
  leftMotor.spin(reverse,speed,percent);
  rightMotor.spin(forward,speed,percent);
  while(Gyro.heading(degrees) < 360-Degrees-1 || Gyro.heading(degrees) > 360-Degrees+1){
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(0,0);
    Controller1.Screen.print(Gyro.heading(degrees));
    Controller1.Screen.setCursor(1,0);
  } 
  leftMotor.stop();
  rightMotor.stop();
  task::sleep(500);
}

void autoMovement::DriveForward(float inches, float speed){
  float targetAngle = inches*360/($PI*wheelDiameter);
  driveMotors.setPosition(0,deg);
  //driveMotors.setVelocity(speed*100,pct);
  //waitUntil(driveMotors.position(degrees)>=targetAngle);
  
  driveMotors.spinToPosition(rightMotor.position(degrees)+targetAngle,degrees);
  //autoMovement::UpdatePosition(inches);
  driveMotors.stop();
  task::sleep(500);
}

void autoMovement::SetIntakes(float percentOpen,int timeout){
  globalIntakeAngle = 180*(percentOpen/100);
  intakeMotors.spinToPosition(globalIntakeAngle,deg);
  //task::sleep(timeout);
  //autoIntakes.interrupt();
}
void autoMovement::Ready(){
  Elevator.spin(reverse,150,percent);
  task::sleep(2000);
  Elevator.stop();
} 
autoMovement move;


void ResetIntakes(){//Zero the intake positions
  while(!rightIntakeSwitch.pressing()){//Unless the right button is pressed, move the intake closer to zero
    rightIntake.spinFor(reverse,10,degrees);
  }
  while(!leftIntakeSwitch.pressing()){//Unless the left button is pressed, move the intake closer to zero
    leftIntake.spinFor(reverse,10,degrees);
  }
  //Set Intake positions to 0 degrees and turns
  rightIntake.setPosition(0,degrees);
  rightIntake.setPosition(0,turns);
  leftIntake.setPosition(0,degrees);
  leftIntake.setPosition(0,turns);
}
void AutonomousIntakes(){
  while(true){
    if(leftIntakeSwitch.pressing() || rightIntakeSwitch.pressing()){//push out
      IntakeMotors.spinToPosition(180,degrees); //180 degrees is target to spin to
    }
    else{//pull in
      IntakeMotors.spinToPosition(0,degrees);// 0 degrees is where the intakes are spinning to.
    }
  }
}

void autonomousOLD(void){//Autonomous OLD
  ResetIntakes();
  IntakeMotors.spinToPosition(180,degrees);
  driveM.spin(forward,75,pct);
  task::sleep(2000);
  driveM.stop();
  rightMotor.spin(forward,50,pct);
  leftMotor.spin(reverse,50,pct);
  task::sleep(450);
  rightMotor.stop();
  leftMotor.stop();
  driveM.spin(forward,75,pct);
  task::sleep(750);
  driveM.stop();
  thread AutoIntakes = thread(AutonomousIntakes);
  while(true){
    if(leftIntakeSwitch.pressing() != rightIntakeSwitch.pressing()){
      AutoIntakes.interrupt();
      IntakeMotors.stop();
      IntakeMotors.spinToPosition(180,degrees);
      AutoIntakes = thread(AutonomousIntakes);
    }
  }
}

void autonomousOLD2(void){
  ResetIntakes();
  Gyro.calibrate();
  waitUntil(!Gyro.isCalibrating());
  move.Ready();
  move.SetIntakes(100,1000);
  move.DriveForward(36,100);
  Elevator.spin(forward,50,percent);
  move.SetIntakes(0,1000);
  task::sleep(1000);
  Elevator.stop();
  move.DriveForward(10,100);
  move.TurnLeft(100,10);
  move.DriveForward(6,100);
}
void autonomousTest(void){
  move.Precise90Turn(false);
  rightIntake.spinToPosition(90,degrees);
  move.Precise90Turn(true);
}
void finalLeftMotorThread(){
  leftMotor.spinToPosition(225.60,degrees);
}

void autonomous(void){
  ResetIntakes();
  Gyro.calibrate();
  waitUntil(!Gyro.isCalibrating());
  //SECTION 1
  //Point 1
  move.Ready();
  move.SetIntakes(100,100);
  Elevator.spin(forward,50,percent);
  //Point 2
  task::sleep(500);
  move.DriveForward(12,100);
  move.SetIntakes(0,100);
  Elevator.stop();
  //Point 3
  task::sleep(500);
  move.DriveForward(33,100);
  task::sleep(500);
  move.Precise90Turn(false);
  //Point 4
  task::sleep(500);
  //move.DriveForward(20,100);
  rightIntake.spinToPosition(90,degrees);
  move.driveMotors.spin(forward,100,percent);
  task::sleep(2500);
  move.driveMotors.stop();
  //Point 5
  task::sleep(500);
  move.DriveForward(-10,100);
  rightMotor.spinFor(reverse,100,degrees);
  leftMotor.spinFor(forward,100,degrees);
  task::sleep(500);
  move.driveMotors.spin(forward,100,percent);
  //move.DriveForward(2,100);
  task::sleep(150);
  move.driveMotors.stop();
  move.SetIntakes(100,100);
  task::sleep(500);
  rightMotor.setPosition(0,degrees);
  leftMotor.setPosition(0,degrees);
  thread finalLeft = thread(finalLeftMotorThread);
  rightMotor.spinToPosition(448.0,degrees);
  task::sleep(500);
  finalLeft.interrupt();
  rightMotor.spin(forward,50,percent);
  task::sleep(500);
  rightMotor.stop();
  Elevator.spin(forward,100,percent);
  //Point 6
  task::sleep(2500);
  Elevator.stop();

  //SECTION 2
  move.DriveForward(-15,100);
  move.Precise90Turn(true);
  move.SetIntakes(100,100);
  move.DriveForward(15,100);
  Elevator.spin(forward,50,percent);
  move.DriveForward(10,100);
}

//System Variables for operating robot. Tells Robot State
bool Breaks = false;
float speed_mod = 1; 
float elevator_mod = 1;
bool Intakes = false;
bool elevator = false;
bool elevatormode = true;
bool elevatordir = false;
bool IntakeCall = false;

void ControllerScreenUpdater(bool Breaks, float Speed, float elevator,  bool IntakeSet){//Updates Screen
  screen.clearScreen();
  screen.setCursor(1,0);
  if(Intakes){screen.print("Intakes Rolling | ");}
  else{screen.print("Intakes Stopped | ");}
  screen.setCursor(2,0);
  screen.print("Drive Speed: ");
  screen.print(Speed*100);
  screen.setCursor(3,0);
  if(Breaks){screen.print("Breaks ON");}
  else{screen.print("Breaks OFF");}
}

void Drive(float stick_up, float stick_side, float speed_mod){//Using Arcade Control
  leftMotor.spin(forward, speed_mod*((stick_up + stick_side)/2), velocityUnits::pct);  //speed(up+over)/2
  rightMotor.spin(forward, speed_mod*((stick_up - stick_side)/2), velocityUnits::pct);  //speed(up-over)/2
}

void OperateIntakes(void){//Run and operate the intakes
  while(true){
    if(Intakes){//If intakes active, spin forward and back
      if(leftIntakeSwitch.pressing() || rightIntakeSwitch.pressing()){//push out
        IntakeMotors.spinToPosition(180,degrees); //180 degrees is target to spin to
      }
      else{//pull in
        IntakeMotors.spinToPosition(0,degrees);// 0 degrees is where the intakes are spinning to.
      }
      Intakes = false;
    }
    else{
      IntakeMotors.stop(hold);//Stopping intakes when not active.
    }
  }
}
void UpdatePosition(){
}

void DisplayPosition(){
  
}
bool LocationDisplay = false;
int ScreenUpdate = 0;
void DisplaySwitcher(){
  while(true){
    if(ScreenUpdate == 5){
      if(LocationDisplay){
        DisplayPosition();
      }
      else{
        ControllerScreenUpdater(Breaks,speed_mod, elevator_mod, Intakes);
      }
      ScreenUpdate = 0;
    }
    else{
      ScreenUpdate++;
    }
  }
}

void usercontrol(void){//User control state
  ResetIntakes();//Zero the intakes
  thread IntakeThread = thread(OperateIntakes);//Open the intake operation thread to run simultaneously. (maybe)
  //thread Odometry = thread(OdometrySystem);
  //DisplaySwitcher();//Update the screen
  thread Display = thread(DisplaySwitcher);
  move.Ready();
  while(true){//Start the system loop
    UpdatePosition();
    leftMotor.velocity(rpm);
    if(!Breaks){//If not breaked, drive
      Drive(Controller1.Axis3.value(),Controller1.Axis4.value(),speed_mod);//Drive system
    }
    //OperateIntakes();
    if(Controller1.ButtonA.pressing()){//Toggle breaks 
      Breaks = !Breaks;
      while(Controller1.ButtonA.pressing()){//Do nothing until A is released
        task::sleep(1);
      }
    }
    if(elevatormode == true){//If not true, use joystick control
      Elevator.spin(forward,elevator_mod*Controller1.Axis2.value(),velocityUnits::pct);
    }
    else{
      if(elevator == true){//If elevator is enabled
        if(elevatordir == false){//Spin forward
          Elevator.spin(forward,elevator_mod*100,velocityUnits::pct);
        }
        else{//Spin backwards
          Elevator.spin(reverse,elevator_mod*100,velocityUnits::pct);
        }
      }
      else{
        Elevator.stop();
      }
    }
    if(Controller1.ButtonB.pressing()){//Switch elevator direction
      elevatordir = !elevatordir;
      while(Controller1.ButtonB.pressing()){
        task::sleep(1);
      }
    }
    if(Controller1.ButtonY.pressing()){//Toggle the elevator
      elevator = !elevator;
      while(Controller1.ButtonY.pressing()){
        task::sleep(1);
      }
    }
    if(Controller1.ButtonRight.pressing()){//Switch between joystick and button control.
      elevatormode = !elevatormode;
      while(Controller1.ButtonRight.pressing()){
        task::sleep(1);
      }
    }

    if(Controller1.ButtonX.pressing()){//Toggle the intakes
      Intakes = !Intakes;
      while(Controller1.ButtonX.pressing()){
       task::sleep(1);
      }
    }
    if(Controller1.ButtonL1.pressing()){//Increase the drive speed
      speed_mod += 0.05;
      task::sleep(10);
    }
    if(Controller1.ButtonL2.pressing()){//Decrease the drive speed
      speed_mod -= 0.05;
      task::sleep(10);
    }
    if(Controller1.ButtonR1.pressing()){//Increase the elevator speed
      elevator_mod += 0.05;
      task::sleep(10);
    }
    if(Controller1.ButtonR2.pressing()){//Decrease the elevator speed
      elevator_mod -= 0.05;
      task::sleep(10);
    }
    if(Controller1.ButtonLeft.pressing()){//Rezero the Intakes
      ResetIntakes();
      while(Controller1.ButtonLeft.pressing()){
        task::sleep(1); 
      }
    }
    if(Controller1.ButtonUp.pressing()){
      IntakeThread.interrupt();
      IntakeMotors.spinToPosition(180,degrees);
      IntakeThread = thread(OperateIntakes);
      while(Controller1.ButtonUp.pressing()){
        task::sleep(1);
      }
    }
    //wait(50, msec);
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  leftIntake.setMaxTorque(100,pct);
  rightIntake.setMaxTorque(100,pct);
  Gyro.calibrate(1);
  Competition.drivercontrol(usercontrol);
  //Competition.autonomous(autonomous);
  Competition.autonomous(autonomous);
  pre_auton();
  while (true) {
    wait(100, msec);
  }
}
