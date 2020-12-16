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
// accelZ               accelerometer E               
// Gyro                 gyro          F               
// OdometryEncoder      encoder       G, H            
// ---- END VEXCODE CONFIGURED DEVICES ----
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
// accelZ               accelerometer E               
// Gyro                 gyro          F               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "math.h"
#include <position.h>

using namespace vex;
competition Competition;
motor_group IntakeMotors = motor_group(leftIntake,rightIntake);
motor_group driveM = motor_group(leftMotor,rightMotor);
#define screen Controller1.Screen
#define $PI 3.141592653589
Position pos;

void pre_auton(void){/*Pre_auton setup*/}

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


double wheelDiameter = 2.75;
float prevDistance = 0;
void OdometrySystem(){
  while(true){
    float Encoder = OdometryEncoder.position(rev);
    float Distance = Encoder*(wheelDiameter * $PI);
    float ΔDistance = Distance - prevDistance;
    float heading = Gyro.heading(degrees);
    float tempheading = 0;
    float ΔX = 0;
    float ΔY = 0;
    
    if(heading < 90 && heading > 0){
      ΔX = sin(heading)*ΔDistance;
      ΔY = cos(heading)*ΔDistance;
    }
    else if(heading > 90 && heading < 180){
      tempheading = heading - 90;
      ΔX = cos(tempheading)*ΔDistance;
      ΔY = -sin(tempheading)*ΔDistance;
    }
    else if(heading > 180 && heading < 270){
      tempheading = heading -180;
      ΔX = -sin(tempheading)*ΔDistance;
      ΔY = -cos(tempheading)*ΔDistance;
    }
    else if(heading > 270 && heading < 360){
      tempheading = heading - 270;
      ΔX = -cos(tempheading)*ΔDistance;
      ΔY = sin(tempheading)*ΔDistance;
    }
    else if(heading == 0){// Y
      ΔX = 0;
      ΔY = ΔDistance;
    }
    else if(heading == 90){// X
      ΔX = ΔDistance;
      ΔY = 0;
    }
    else if(heading == 180){// -Y
      ΔX = 0;
      ΔY = -ΔDistance;
    }
    else if(heading == 270){// -X
      ΔX = -ΔDistance;
      ΔY = 0;
    }
    pos.location.Update(pos.location.getX() + ΔX, pos.location.getY() + ΔY, pos.location.getZ());
    prevDistance = Distance;
  }
}

void autonomous(void){//Autonomous
  thread Odometry = thread(OdometrySystem);
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
  pos.Heading = Gyro.heading(degrees);
}

void DisplayPosition(){
  float x = pos.location.getX();
  float y = pos.location.getY();
  float z = pos.location.getZ();

  screen.clearScreen();
  screen.setCursor(0,0);
  screen.print(x);
  screen.print(" | ");
  screen.print(y);
  screen.print(" | ");
  screen.print(z);
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

void SystemsTest(){
  Gyro.calibrate();
  waitUntil(!Gyro.isCalibrating());
  Gyro.setHeading(0,degrees);
  thread Odometry = thread(OdometrySystem);
  //task::sleep(2000);

  //leftMotor.spin(forward,25,pct);
  //rightMotor.spin(reverse,25,pct);
  //waitUntil(Gyro.heading(degrees) > 90);

  //leftMotor.stop();
  //rightMotor.stop();


  while(true){
    task::sleep(250);
    //pos.UpdateSystem();
    screen.clearScreen();
    screen.setCursor(0,0);
    screen.print(Gyro.heading(degrees));
    screen.newLine();
    screen.print(pos.location.getX());
    screen.print("|");
    screen.print(pos.location.getY());
    screen.print("|");
    screen.print(pos.location.getZ());
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
  Competition.autonomous(SystemsTest);
  pre_auton();
  while (true) {
    wait(100, msec);
  }
}
