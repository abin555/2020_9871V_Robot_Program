#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor rightIntake = motor(PORT1, ratio18_1, false);
motor leftIntake = motor(PORT2, ratio18_1, true);
motor Elevator = motor(PORT3, ratio18_1, true);
motor rightMotor = motor(PORT20, ratio18_1, true);
motor leftMotor = motor(PORT11, ratio18_1, false);
limit rightIntakeSwitch = limit(Brain.ThreeWirePort.A);
limit leftIntakeSwitch = limit(Brain.ThreeWirePort.B);
accelerometer accelX = accelerometer(Brain.ThreeWirePort.C);
accelerometer accelY = accelerometer(Brain.ThreeWirePort.D);
accelerometer accelZ = accelerometer(Brain.ThreeWirePort.E);
gyro Gyro = gyro(Brain.ThreeWirePort.F);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}