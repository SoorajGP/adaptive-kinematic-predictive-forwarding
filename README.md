# Adaptive Kinematic Predictive Forwarding (AKPF)

An in-progress research project exploring kinematic link-lifetime prediction for Named Data Networking (NDN) in vehicular networks.

## Initial snapshot

This initial snapshot contains the kinematic predictor component. The complete forwarding strategy and NDN4IVC integration are not included while their licensing and redistribution terms are being reviewed.

## Kinematic predictor

The predictor estimates how long two vehicles remain within communication range based on relative position and velocity.

Local standalone tests previously passed for stationary, approaching, separating, and out-of-range cases. These tests do not establish end-to-end forwarding performance.

## Limitations

- This is not yet a standalone buildable project.
- Prediction-driven next-hop selection is not complete.
- No controlled performance comparison against baseline forwarding has been completed.
- Reproducible build instructions and the test harness still need to be added.

## Licensing

Reuse and redistribution terms are still being reviewed. No blanket licence is granted by this initial snapshot.
