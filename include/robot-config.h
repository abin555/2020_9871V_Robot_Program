using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor rightIntake;
extern motor leftIntake;
extern motor Elevator;
extern motor rightMotor;
extern motor leftMotor;
extern limit rightIntakeSwitch;
extern limit leftIntakeSwitch;
extern accelerometer accelX;
extern accelerometer accelY;
extern gyro Gyro;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );