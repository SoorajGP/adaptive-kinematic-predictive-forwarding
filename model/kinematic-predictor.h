#ifndef NDN4IVC_KINEMATIC_PREDICTOR_H
#define NDN4IVC_KINEMATIC_PREDICTOR_H

namespace ndn4ivc {

struct Vector2D
{
  double x;
  double y;

  Vector2D(double xValue = 0.0, double yValue = 0.0)
    : x(xValue), y(yValue)
  {
  }
};

struct LinkPrediction
{
  double lifetimeSeconds;
  bool initiallyInRange;
  bool exitsWithinHorizon;
};

class KinematicPredictor
{
public:
  static LinkPrediction predict(
    const Vector2D& relativePosition,
    const Vector2D& relativeVelocity,
    double communicationRange,
    double maxHorizon = 15.0);

  static LinkPrediction predictWithAcceleration(
    const Vector2D& relativePosition,
    const Vector2D& relativeVelocity,
    const Vector2D& relativeAcceleration,
    double communicationRange,
    double maxHorizon = 15.0,
    double accelerationTimeConstant = 2.0,
    double timeStep = 0.1);
};

} // namespace ndn4ivc

#endif
