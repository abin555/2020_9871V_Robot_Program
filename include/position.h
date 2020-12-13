#include "vex.h"
#include "robot-config.h"
#pragma once
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