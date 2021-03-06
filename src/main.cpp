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
competition Competition;//Competition object
motor_group IntakeMotors = motor_group(leftIntake,rightIntake);//Intake motor group
motor_group driveM = motor_group(leftMotor,rightMotor);//Drive Motor group
#define screen Controller1.Screen//Define screen mapping
#define $PI 3.141592653589//Aproximate PI

void pre_auton(void){/*Pre_auton setup*/}

float globalIntakeAngle;

class autoMovement{//Define Autonomous Class Structure
  float wheelDiameter = 3.0f;//Wheel Diameter constant
  public:
  motor_group driveMotors = motor_group(leftMotor, rightMotor);//drive motor group
  motor_group intakeMotors = motor_group(leftIntake,rightIntake);//intake motor group
  void TurnRight(float degrees,float speed);//Turn right function definition
  void TurnLeft(float degrees,float speed);//Turn left function definition
  void DriveForward(float inches, float speed);//Drive foward definition
  void SetIntakes(float percentOpen, int timeout);//Set Intakes definition
  void Precise90Turn(bool dir);//Precisely turn 90 degrees definition
  void Ready();//Readying function definition
  int x = 0;
  int y = 0;
};

void rightPreciseThreadTurn(){//Separate turn thread
  rightMotor.spinToPosition(290.60,degrees,100,velocityUnits::pct);
}
void leftPreciseThreadTurn(){//Separate turn thread
  leftMotor.spinToPosition(274.60,degrees,100,velocityUnits::pct);
}
void autoMovement::Precise90Turn(bool dir){//Use encoders to precisely turn left or right 90 degrees
  if(!dir){//left
    leftMotor.setPosition(0,degrees);
    rightMotor.setPosition(0,degrees);
    thread rightTurn = thread(rightPreciseThreadTurn);
    leftMotor.spinToPosition(-340.40,degrees,100,velocityUnits::pct);
  }
  else{
    leftMotor.setPosition(0,degrees);
    rightMotor.setPosition(0,degrees);
    thread leftTurn = thread(leftPreciseThreadTurn);
    rightMotor.spinToPosition(-340.40,degrees,100,velocityUnits::pct);
  }
}

void autoMovement::TurnRight(float Degrees,float speed){//Turn right using gyroscope
  Gyro.setHeading(0,deg);//reset heading
  leftMotor.spin(forward,speed,percent);//spin left motor
  rightMotor.spin(reverse,speed,percent);//spin right motor
  while(Gyro.heading(degrees) < Degrees-1 || Gyro.heading(degrees) > Degrees+1){//Wait until angle is correct
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(0,0);
    Controller1.Screen.print(Gyro.heading(degrees));
    Controller1.Screen.setCursor(1,0);
  } 
  //Stop motors
  leftMotor.stop();
  rightMotor.stop();
  task::sleep(500);
}

void autoMovement::TurnLeft(float Degrees, float speed){//Turn left using gyroscope
  Gyro.setHeading(0,deg);//reset heading
  leftMotor.spin(reverse,speed,percent);//spin left motor
  rightMotor.spin(forward,speed,percent);//spin right motor
  while(Gyro.heading(degrees) < 360-Degrees-1 || Gyro.heading(degrees) > 360-Degrees+1){//Wait until angle is correct
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(0,0);
    Controller1.Screen.print(Gyro.heading(degrees));
    Controller1.Screen.setCursor(1,0);
  } 
  //Stop motors
  leftMotor.stop();
  rightMotor.stop();
  task::sleep(500);
}

void autoMovement::DriveForward(float inches, float speed){//Drive distance function
  float targetAngle = inches*360/($PI*wheelDiameter);//Calculated turn amount in degrees
  driveMotors.setPosition(0,deg);
  
  driveMotors.spinToPosition(rightMotor.position(degrees)+targetAngle,degrees);//spin drive motors to calculated position
  driveMotors.stop();//Stop motors when position reached.
  task::sleep(500);
}

void autoMovement::SetIntakes(float percentOpen,int timeout){//Set intakes to percentage open
  globalIntakeAngle = 180*(percentOpen/100);//Calculated angle in degrees
  intakeMotors.spinToPosition(globalIntakeAngle,deg);//spin to calculated angle
}
void autoMovement::Ready(){//Deploy top
  Elevator.spin(reverse,150,percent);//spin elevator down
  task::sleep(2000);//Wait
  Elevator.stop();//stop
} 
autoMovement move;//Define autonomous class


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
void AutonomousIntakes(){//Intake Function for autonomous
  while(true){
    if(leftIntakeSwitch.pressing() || rightIntakeSwitch.pressing()){//push out
      IntakeMotors.spinToPosition(180,degrees); //180 degrees is target to spin to
    }
    else{//pull in
      IntakeMotors.spinToPosition(0,degrees);// 0 degrees is where the intakes are spinning to.
    }
  }
}

//TODO: Create Backup Autonomous!!!
void autonomous(void){
  ResetIntakes();
  Gyro.calibrate();
  waitUntil(!Gyro.isCalibrating());
  move.Ready();
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
  screen.clearScreen();//clear screen
  screen.setCursor(1,0);//set cursor
  //Intake Status indicator
  if(Intakes){screen.print("Intakes Open");}
  else{screen.print("Intakes Closed");}
  screen.setCursor(2,0);//Set cursor
  //Drive speed indicator
  screen.print("Drive Speed: ");
  screen.print(Speed*100);
  screen.setCursor(3,0);//Set cursor
  //Breaks indicator
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
bool LocationDisplay = false;
int ScreenUpdate = 0;
void DisplaySwitcher(){//Function to time the screen updates
  while(true){//Thread loop
    //Update Screen every 5th attempt
    if(ScreenUpdate == 5){
      ControllerScreenUpdater(Breaks,speed_mod, elevator_mod, Intakes);//Update Screen
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
  thread Display = thread(DisplaySwitcher);
  move.Ready();
  while(true){//Start the system loop
    leftMotor.velocity(rpm);
    if(!Breaks){//If not breaked, drive
      Drive(Controller1.Axis3.value(),Controller1.Axis4.value(),speed_mod);//Drive system
    }

    if(Controller1.ButtonA.pressing()){//Toggle breaks 
      Breaks = !Breaks;
      while(Controller1.ButtonA.pressing()){//Do nothing until A is released
        task::sleep(1);
      }
    }
    Elevator.spin(forward,elevator_mod*Controller1.Axis2.value(),velocityUnits::pct);//Spin elevator

    if(Controller1.ButtonX.pressing()){//Toggle the intakes
      Intakes = !Intakes;
      while(Controller1.ButtonX.pressing()){//Wait until the x button is released
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

    if(Controller1.ButtonY.pressing()){//Put Claw in punch position
      IntakeMotors.spinToPosition(90,degrees,false);
      while(Controller1.ButtonY.pressing()){}//Wait until Y button is released
    }
    else{
      if(IntakeMotors.position(degrees) == 90){//Stop motors when desired position is reached.
        IntakeMotors.stop(hold);
      }
    }

    if(Controller1.ButtonUp.pressing()){//Drive forward at 60 percent speed
      leftMotor.spin(forward,60,velocityUnits::pct);
      rightMotor.spin(forward,60,velocityUnits::pct);
    }
    if(Controller1.ButtonDown.pressing()){//Drive backwards at 60 percent speed
      leftMotor.spin(reverse,60,velocityUnits::pct);
      rightMotor.spin(reverse,60,velocityUnits::pct);
    }
    if(Controller1.ButtonLeft.pressing()){//Turn left at 25 percent speed
      leftMotor.spin(reverse,25,velocityUnits::pct);
      rightMotor.spin(forward,25,velocityUnits::pct);
    }
    if(Controller1.ButtonRight.pressing()){//Turn right at 25 percent speed
      leftMotor.spin(forward,25,velocityUnits::pct);
      rightMotor.spin(reverse,25,velocityUnits::pct);
    }
    //When the controller is not telling it to drive, stop the drive motors
    if(Controller1.Axis3.value() == 0 && Controller1.Axis4.value() == 0 
    && !Controller1.ButtonUp.pressing() && !Controller1.ButtonDown.pressing() && !Controller1.ButtonLeft.pressing() && !Controller1.ButtonRight.pressing()){
      leftMotor.stop();
      rightMotor.stop();
    }
  }
}

//Starting function
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  //Set intakes to maximum torque
  leftIntake.setMaxTorque(100,pct);
  rightIntake.setMaxTorque(100,pct);
  Gyro.calibrate(1);//Calibrate Gyroscopes
  Competition.drivercontrol(usercontrol);//Driver control main loop
  //Competition.autonomous(autonomous);
  Competition.autonomous(autonomous);//Autonomous control system
  pre_auton();
  while (true) {
    wait(100, msec);
  }
}
