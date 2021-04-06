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
// Elevator1            motor         3               
// rightMotor           motor         20              
// leftMotor            motor         11              
// rightIntakeSwitch    limit         A               
// leftIntakeSwitch     limit         B               
// accelX               accelerometer C               
// accelY               accelerometer D               
// GyroOLD              gyro          E               
// GyroB                inertial      12              
// Gyro                 gyro          F               
// Elevator2            motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "math.h"
#include <sstream>

using std::stringstream;
using std::string;

using namespace vex;
competition Competition;//Competition object
motor_group IntakeMotors = motor_group(leftIntake,rightIntake);//Intake motor group
motor_group driveM = motor_group(leftMotor,rightMotor);//Drive Motor group
motor_group Elevator = motor_group(Elevator1,Elevator2);
#define screen Controller1.Screen//Define screen mapping
#define $PI 3.141592653589//Aproximate PI

void pre_auton(void){/*Pre_auton setup*/}

float globalIntakeAngle;

class autoMovement{//Define Autonomous Class Structure
  public:
  void Ready();//Readying function definition
};

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

//System Variables for operating robot. Tells Robot State
bool Breaks = false;
float speed_mod = 0.9; 
float elevator_mod = 1;
int Intakes = 0; //0 = closed | 1 = 90 degrees | 2 = 180 degrees

void ControllerScreenUpdater(){//Updates Screen
  double ElevatorTemp = Elevator1.temperature(percent);
  double LeftTemp = leftMotor.temperature(percent);
  double RightTemp = rightMotor.temperature(percent);
  Controller1.Screen.clearScreen();
  std::stringstream temp;
  temp << "E1:" << ElevatorTemp << " LM: " << LeftTemp << " RM: " << RightTemp;
  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print(temp.str().c_str());
  
}

void Drive(float stick_up, float stick_side, float speed_mod){//Using Arcade Control
  leftMotor.spin(forward, speed_mod*((stick_up + stick_side)/2), velocityUnits::pct);  //speed(up+over)/2
  rightMotor.spin(forward, speed_mod*((stick_up - stick_side)/2), velocityUnits::pct);  //speed(up-over)/2
}


bool LocationDisplay = false;
int ScreenUpdate = 0;
void DisplaySwitcher(){//Function to time the screen updates
  while(true){//Thread loop
    //Update Screen every 5th attempt
    if(ScreenUpdate == 5){
      ControllerScreenUpdater();//Update Screen
      ScreenUpdate = 0;
    }
    else{
      ScreenUpdate++;
    }
  }
}

bool Xpress = false;
void SystemControls(int S1,int S2,int S3, int S4,int X, int A, int Y, int B, int UP, int DN, int LF,int RT){//System Control Main Function
  
  Drive(S3,S4,speed_mod);
  Elevator.spin(forward,elevator_mod*S2,velocityUnits::pct);//Spin elevator with the second axis

  if(X && !Xpress){
    if(Intakes < 2){
      Intakes++;
    }
    else{
      Intakes = 0;
    }
    Xpress = true;
    if(Intakes == 0){
      IntakeMotors.spinToPosition(0,degrees,false);
    }
    if(Intakes == 1){
      IntakeMotors.spinToPosition(90,degrees,false);
    }
    if(Intakes == 2){
      IntakeMotors.spinToPosition(180,degrees,false);
    }
  }
  if(!X){
    Xpress = false;
    
    if(Intakes == 0 && rightIntake.position(degrees) == 0){
      IntakeMotors.stop(hold);
    }
    else if(Intakes == 1 && rightIntake.position(degrees) == 90){
      IntakeMotors.stop(hold);
    }
    else if(Intakes == 2 && rightIntake.position(degrees) == 180){
      IntakeMotors.stop(hold);
    }
  }

  //ofs << "Update\r\n";
  if(UP){//Drive forward with up arrow
    leftMotor.spin(forward,60,velocityUnits::pct);
    rightMotor.spin(forward,60,velocityUnits::pct);
  }
  if(DN){//Drive in reverse with down arrow
    leftMotor.spin(reverse,60,velocityUnits::pct);
    rightMotor.spin(reverse,60,velocityUnits::pct);
  }
  if(LF){//Turn left
    leftMotor.spin(reverse,25,velocityUnits::pct);
    rightMotor.spin(forward,25,velocityUnits::pct);
  }
  if(RT){//Turn right
    leftMotor.spin(forward,25,velocityUnits::pct);
    rightMotor.spin(reverse,25,velocityUnits::pct);
  }
  /*
  if(S3 == 0 && S4 == 0 && !UP && !DN && !LF && !RT){
    leftMotor.stop();
    rightMotor.stop();
  }*/
}

void usercontrol(void){//User control state
  ResetIntakes();//Zero the intakes
  thread Display = thread(DisplaySwitcher);
  move.Ready();
  int S1,S2,S3,S4,X,A,Y,B,UP,DN,LF,RT;
  while(true){//Start the system loop
	//Get controls
    S1 = Controller1.Axis1.value();
    S2 = Controller1.Axis2.value();
    S3 = Controller1.Axis3.value();
    S4 = Controller1.Axis4.value();
    X = Controller1.ButtonX.pressing();
    A = Controller1.ButtonA.pressing();
    B = Controller1.ButtonB.pressing();
    Y = Controller1.ButtonY.pressing();
    UP = Controller1.ButtonUp.pressing();
    DN = Controller1.ButtonDown.pressing();
    LF = Controller1.ButtonLeft.pressing();
    RT = Controller1.ButtonRight.pressing();
	//Run controls through drive function
    SystemControls(S1,S2,S3,S4,X,A,B,Y,UP,DN,LF,RT);
  }
}

void autonomous(){
  move.Ready();
  Elevator.spin(forward,100,pct);
  task::sleep(1000);
  Elevator.stop();
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
  Competition.autonomous(autonomous);
  //Competition.autonomous(autonomous);
  pre_auton();
  while (true) {
    wait(100, msec);
  }
}
