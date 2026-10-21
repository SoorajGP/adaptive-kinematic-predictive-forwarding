#include "kinematic-predictor.h"

#include <cmath>
#include <algorithm>

namespace ndn4ivc {

namespace {

double squaredNorm(const Vector2D& v)
{
  return v.x * v.x + v.y * v.y;
}

double dot(const Vector2D& a, const Vector2D& b)
{
  return a.x * b.x + a.y * b.y;
}

Vector2D add(const Vector2D& a, const Vector2D& b)
{
  return Vector2D(a.x + b.x, a.y + b.y);
}

Vector2D scale(const Vector2D& v, double s)
{
  return Vector2D(v.x * s, v.y * s);
}

} // namespace

LinkPrediction
KinematicPredictor::predict(
  const Vector2D& r,
  const Vector2D& dv,
  double range,
  double horizon)
{
  LinkPrediction result = {0.0, false, false};

  if (range <= 0.0 || horizon <= 0.0)
    return result;

  const double c = squaredNorm(r) - range * range;
  result.initiallyInRange = (c <= 0.0);

  if (!result.initiallyInRange)
    return result;

  const double a = squaredNorm(dv);
  const double b = 2.0 * dot(r, dv);

  // Already in range and no relative movement: use the horizon cap.
  if (a < 1e-12) {
    result.lifetimeSeconds = horizon;
    return result;
  }

  const double discriminant = b * b - 4.0 * a * c;

  if (discriminant < 0.0) {
    result.lifetimeSeconds = horizon;
    return result;
  }

  const double root = std::sqrt(discriminant);
  const double t1 = (-b - root) / (2.0 * a);
  const double t2 = (-b + root) / (2.0 * a);

  // For an initially in-range link, t2 is the future exit time.
  const double exitTime = std::max(t1, t2);

  if (exitTime < 0.0 || exitTime > horizon) {
    result.lifetimeSeconds = horizon;
    return result;
  }

  result.lifetimeSeconds = exitTime;
  result.exitsWithinHorizon = true;
  return result;
}

LinkPrediction
KinematicPredictor::predictWithAcceleration(
  const Vector2D& r,
  const Vector2D& dv,
  const Vector2D& da,
  double range,
  double horizon,
  double tau,
  double step)
{
  LinkPrediction result = {0.0, false, false};

  if (range <= 0.0 || horizon <= 0.0 || tau <= 0.0 || step <= 0.0)
    return result;

  result.initiallyInRange =
    (squaredNorm(r) <= range * range);

  if (!result.initiallyInRange)
    return result;

  // For zero acceleration, use the exact constant-velocity solution.
  if (squaredNorm(da) < 1e-12)
    return predict(r, dv, range, horizon);

  const double rangeSquared = range * range;

  for (double t = step; t <= horizon + 1e-9; t += step) {
    // Fading acceleration:
    // g(t) = tau * [t - tau * (1 - exp(-t/tau))]
    const double g =
      tau * (t - tau * (1.0 - std::exp(-t / tau)));

    Vector2D separation = add(r, scale(dv, t));
    separation = add(separation, scale(da, g));

    if (squaredNorm(separation) > rangeSquared) {
      result.lifetimeSeconds = std::min(t, horizon);
      result.exitsWithinHorizon = true;
      return result;
    }
  }

  result.lifetimeSeconds = horizon;
  return result;
}

} // namespace ndn4ivc
