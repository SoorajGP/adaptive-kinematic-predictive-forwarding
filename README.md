# Adaptive Kinematic Predictive Forwarding (AKPF)

An **in-progress research prototype** exploring kinematic link-lifetime prediction for Named Data Networking (NDN) in vehicular ad hoc networks (VANETs), using ns-3, ndnSIM, and SUMO.

## Current status

The repository currently contains selected source files from an ongoing integration inside an ndnSIM/NDN4IVC working tree. It is not a standalone buildable project yet.

Implemented prototype components include:

- A kinematic predictor based on relative position and relative velocity.
- Neighbor beacon-history data structures.
- Prediction calculation and runtime logging inside an NDN forwarding-strategy prototype.
- A SUMO/ns-3 demo that has previously run in the developer's local environment.

The standalone predictor was tested for stationary, approaching, separating, and out-of-range cases. These tests do not validate end-to-end forwarding performance.

## Important limitations

- The computed lifetime score does **not yet control next-hop selection or forwarding suppression**. Forwarding behavior remains based on the existing multicast-style strategy logic.
- A local-vehicle velocity integration attempt using TraCI's speed and angle caused a simulation crash for an unmapped/empty vehicle ID and was reverted. Treat the current predictor integration as a prototype requiring further validation.
- No controlled baseline comparison has been completed. There are no substantiated claims of improved packet delivery ratio, latency, retransmission rate, or forwarding overhead.
- This repository currently contains selected source files only. Build scripts, all dependencies, integration patches, test harnesses and full reproduction instructions are not yet included.

## Source layout

- `model/kinematic-predictor.{h,cc}` — kinematic lifetime estimator.
- `model/akpf-strategy.{h,cc}` — NDN forwarding-strategy prototype that logs prediction results.
- `integration/` — related beacon and neighbor-history source files from the local NDN4IVC integration.

## Integration environment

The source was developed in a working tree using ns-3, ndnSIM, NDN4IVC and SUMO. Exact compatible revisions and a clean-room setup procedure have not yet been documented. The source files should be treated as integration components, not copied into an arbitrary ndnSIM version without checking API compatibility.

## Planned work

1. Complete and validate the local vehicle velocity input.
2. Make predictions affect forwarding decisions in a defined, testable way.
3. Validate predicted link lifetimes against observed link disconnection times.
4. Compare against the baseline under the same mobility traces and configuration.
5. Add reproducible build instructions, test harnesses, simulation scripts and result artefacts.

## Licensing and attribution

This repository includes source derived from or designed to integrate with third-party projects. Before redistribution or reuse, review the original file headers and licenses for ns-3, ndnSIM, NDN4IVC and any related components. No new license is declared for the combined work in this initial snapshot; do not assume that all files share the same license.

## Research integrity

This is an implementation-in-progress snapshot. Please interpret logs and predictor unit tests as evidence of component execution only, not as evidence of protocol-level performance improvement.
