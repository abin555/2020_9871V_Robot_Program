#include "vex.h"
#include "robot-config.h"

class Position{
  public:
  struct Location{
    float x = 0;
    float y = 0;
    float z = 0;
    public:
    float getX();
    float getY();
    float getZ();
    void Update(float x, float y, float z);
  };
  struct Velocity{
    float x = 0;
    float y = 0;
    float z = 0;
    public:
    float getX();
    float getY();
    float getZ();
    void Update(float x, float y, float z);
  };
  struct Acceleration{
    float x = 0;
    float y = 0;
    float z = 0;
    public:
    float getX();
    float getY();
    float getZ();
    void Update(float x, float y, float z);
  };
  Location location;
  Velocity velocity;
  Acceleration acceleration;
  void ZeroInertialSystem();
  float Heading;
  void UpdateSystem();
  void Calibrate();
};

float Position::Location::getX(){
  return Position::Location::x;
}
float Position::Location::getY(){
  return Position::Location::y;
}
float Position::Location::getZ(){
  return Position::Location::z;
}
void Position::Location::Update(float x, float y, float z){
  Position::Location::x = x;
  Position::Location::y = y;
  Position::Location::z = z;
}

float Position::Velocity::getX(){
  return Position::Velocity::x;
}
float Position::Velocity::getY(){
  return Position::Velocity::y;
}
float Position::Velocity::getZ(){
  return Position::Velocity::z;
}
void Position::Velocity::Update(float x, float y, float z){
  Position::Velocity::x = x;
  Position::Velocity::y = y;
  Position::Velocity::z = z;
}

float Position::Acceleration::getX(){
  return Position::Acceleration::x;
}
float Position::Acceleration::getY(){
  return Position::Acceleration::y;
}
float Position::Acceleration::getZ(){
  return Position::Acceleration::z;
}
void Position::Acceleration::Update(float x, float y, float z){
  Position::Acceleration::x = x;
  Position::Acceleration::y = y;
  Position::Acceleration::z = z;
}

void Position::ZeroInertialSystem(){
  Heading = 0.0f;
  Gyro.setHeading(0,degrees);
}
void Position::UpdateSystem(){
  Position::acceleration.x = accelX.acceleration();
  Position::acceleration.y = accelY.acceleration();
  Position::acceleration.z = accelZ.acceleration();
  Position::velocity.x = Position::velocity.x + Position::acceleration.x;
  Position::velocity.y = Position::velocity.y + Position::acceleration.y;
  Position::velocity.z = Position::velocity.z + Position::acceleration.z;
  Position::location.x = Position::location.x + Position::velocity.x;
  Position::location.y = Position::location.y + Position::velocity.y;
  Position::location.z = Position::location.z + Position::velocity.z;
}
void Position::Calibrate(){
  Gyro.calibrate();
  waitUntil(!Gyro.isCalibrating());
}