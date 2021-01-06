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
// Gyro                 gyro          E               
// OdometryEncoder      encoder       G, H            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "math.h"
//#include <automovement.h>

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
  motor_group driveMotors = motor_group(leftMotor, rightMotor);
  motor_group intakeMotors = motor_group(leftIntake,rightIntake);
  public:
  void TurnRight(float degrees,float speed);
  void TurnLeft(float degrees,float speed);
  void DriveForward(float inches, float speed);
  void SetIntakes(float percentOpen, int timeout);
  void UpdatePosition(float Distance);
  int x = 0;
  int y = 0;
};

void autoMovement::UpdatePosition(float Distance){
  float direction = Gyro.heading(degrees);
  float angle = 0;
  float deltaX = 0;
  float deltaY = 0;
  int Xdir = 1;//1 = positive X | -1 = negative X
  int Ydir =  1;//1 = positive Y | -1 = negative Y
  /*
                +Y
                │
                │
                │
                │
                │
                │
 -X ────────────┼──────────── +X
                │
                │
                │
                │
                │
                │
                -Y
  */
  if(direction != 0 && direction != 90 && direction != 180 && direction != 270){
    if(direction > 0 && direction < 90){
      angle = direction;
      Xdir = 1;
      Ydir = 1;
    }
    else if(direction > 90 &&  direction < 180){
      angle = direction - 90;
      Xdir = 1;
      Ydir = -1;
    }
    else if(direction > 180 && direction < 270){
      angle = direction - 180;
      Xdir = -1;
      Ydir = -1;
    }
    else if(direction > 270 && direction < 360){
      angle = direction - 270;
      Xdir = -1;
      Ydir = 1;
    }
    //TODO: Create Trigonometric System!!!
  }
  else{
    if(direction == 0){//+Y
      autoMovement::y = autoMovement::y + Distance;
    }
    else if(direction == 90){//+X
      autoMovement::x = autoMovement::x + Distance;
    }
    else if(direction == 180){//-Y
      autoMovement::y = autoMovement::y - Distance;
    }
    else if(direction == 270){//-X
      autoMovement::x = autoMovement::x - Distance;
    }
  }
}

void autoMovement::TurnRight(float degrees,float speed){
  float originalHeading = Gyro.heading(deg);
  Gyro.setHeading(0,deg);
  leftMotor.spin(forward,speed*100,pct);
  rightMotor.spin(reverse,speed*100,pct);
  waitUntil(Gyro.heading()>=degrees);
  leftMotor.stop();
  rightMotor.stop();
  if(originalHeading+degrees>=360){
    Gyro.setHeading(originalHeading+degrees-360,deg);
  }
  else{
    Gyro.setHeading(originalHeading+degrees,deg);
  }
  task::sleep(500);
}

void autoMovement::TurnLeft(float degrees, float speed){
  float originalHeading = Gyro.heading(deg);
  Gyro.setHeading(0,deg);
  leftMotor.spin(reverse,speed*100,pct);
  rightMotor.spin(forward,speed*100,pct);
  waitUntil(fabs(Gyro.heading(deg)-360)>=degrees);
  leftMotor.stop();
  rightMotor.stop();
  if(originalHeading+degrees>=360){
    Gyro.setHeading(originalHeading+degrees-360,deg);
  }
  else{
    Gyro.setHeading(originalHeading+degrees,deg);
  }
  task::sleep(500);
}

void autoMovement::DriveForward(float inches, float speed){
  float targetAngle = inches*360/($PI*wheelDiameter);
  driveMotors.setPosition(0,deg);
  driveMotors.setVelocity(speed*100,pct);
  waitUntil(driveMotors.position(degrees)>=targetAngle);
  driveMotors.stop();
  task::sleep(500);
}

void autoMovement::SetIntakes(float percentOpen,int timeout){
  globalIntakeAngle = 180*(percentOpen/100);
  intakeMotors.spinToPosition(globalIntakeAngle,deg);
  //task::sleep(timeout);
  //autoIntakes.interrupt();
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

void autonomous(void){
  ResetIntakes();
  move.SetIntakes(100,1000);
  task::sleep(1500);
  move.DriveForward(24,20);
  task::sleep(1500);
  move.TurnLeft(90,10);
  task::sleep(1500);
  move.SetIntakes(50,1000);
  move.TurnRight(180,10);
  move.DriveForward(18,20);
}

//System Variables for operating robot. Tells Robot State
bool Breaks = false;
float speed_mod = 0.75; 
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
