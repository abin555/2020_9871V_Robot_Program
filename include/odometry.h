class Odometry{
  private:
    double deltaX;
    double deltaY;
    double deltaHeading;
    double prevX;
    double prevY;
    double prevHeading;
  public:
    double X;
    double Y;
    double heading;
    void OdometryAlgorithm();
};