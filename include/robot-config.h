using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor rightIntake;
extern motor leftIntake;
extern motor Elevator;
extern motor rightMotor;
extern motor leftMotor;
extern motor rampMotor;
extern limit rightIntakeSwitch;
extern limit leftIntakeSwitch;
extern drivetrain Drivetrain;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );