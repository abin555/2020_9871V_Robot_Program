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
  float Heading;
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

void Position::Calibrate(){
  Gyro.calibrate();
  waitUntil(!Gyro.isCalibrating());
}