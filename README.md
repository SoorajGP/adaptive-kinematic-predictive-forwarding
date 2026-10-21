# Adaptive Kinematic Predictive Forwarding (AKPF)

**Research area:** Named Data Networking (NDN) · Vehicular ad hoc networks (VANETs) · Mobility-aware networking  
**Artifact status:** Early-stage research prototype  
**Public snapshot:** Kinematic link-lifetime predictor only

> **Scope note:** This repository is not yet a complete implementation of an adaptive forwarding protocol. It currently publishes the predictor component. The strategy and NDN4IVC integration used in the local simulation are not included in this snapshot while their provenance and redistribution terms are reviewed.

## 1. Overview

Vehicular networks experience rapidly changing connectivity as vehicles move through a road network. For data-centric networking architectures such as NDN, link persistence is one potentially useful signal when designing forwarding policies.

AKPF explores a kinematic approach to estimating the **remaining time a pair of vehicles stays within a specified communication range**. The predictor takes relative position, relative velocity, a communication-range assumption, and a finite prediction horizon. It returns a bounded lifetime estimate and flags indicating whether the pair starts in range and whether an exit is predicted within the horizon.

The longer-term research objective is to investigate how predicted link lifetime could inform NDN forwarding decisions. In the current local integration prototype, a strategy computes and logs a normalized lifetime score, but that score does **not** yet control next-hop selection or forwarding suppression. This repository snapshot publishes the predictor component only.

## 2. Research question

**Can a lightweight kinematic estimate of link persistence provide a useful input to mobility-aware forwarding in NDN-based vehicular networks?**

Answering this requires more than computing a plausible lifetime estimate. It requires validating predictions against observed disconnections, integrating the prediction into a defined forwarding rule, and comparing end-to-end outcomes against an appropriate baseline under reproducible mobility and network conditions. Those protocol-level experiments have not yet been completed.

## 3. Method

### 3.1 Constant-relative-velocity model

Let the relative position and relative velocity of two vehicles be

$$
\mathbf{r}_0 = \mathbf{p}_j - \mathbf{p}_i,
\qquad
\mathbf{v}_{rel} = \mathbf{v}_j - \mathbf{v}_i.
$$

Under a constant-relative-velocity assumption, their relative displacement at time $t$ is

$$
\mathbf{r}(t) = \mathbf{r}_0 + \mathbf{v}_{rel}t.
$$

Given a communication range $R$, the boundary of connectivity is reached when

$$
\|\mathbf{r}_0 + \mathbf{v}_{rel}t\|^2 = R^2.
$$

Expanding this expression yields a quadratic equation:

$$
at^2 + bt + c = 0,
$$

where

$$
a = \mathbf{v}_{rel}\cdot\mathbf{v}_{rel},\quad
b = 2\mathbf{r}_0\cdot\mathbf{v}_{rel},\quad
c = \mathbf{r}_0\cdot\mathbf{r}_0 - R^2.
$$

For a pair that starts within range, the implementation uses the future boundary-crossing root to estimate link lifetime. The result is bounded by a finite prediction horizon $H$. If the initial separation is outside range, the predictor reports `initiallyInRange = false` and a lifetime of zero. If no exit is predicted within the horizon, the returned lifetime is capped at $H$.

### 3.2 Acceleration-aware extension

The API also provides an experimental acceleration-aware estimate. It models an acceleration contribution with a fading displacement term:

$$
g(t) = \tau\left[t - \tau\left(1-e^{-t/\tau}\right)\right],
$$

and evaluates relative separation using

$$
\mathbf{r}(t) = \mathbf{r}_0 + \mathbf{v}_{rel}t
                 + \mathbf{a}_{rel}g(t).
$$

This branch checks separation over discrete time steps rather than solving a closed-form boundary-crossing equation. It is present in the predictor API but has not yet been independently validated for the study's intended mobility conditions. It should be treated as experimental, not as an established improvement over the constant-velocity model.

### 3.3 Output contract

The C++ API returns a `LinkPrediction` containing:

| Field | Meaning |
|---|---|
| `lifetimeSeconds` | Predicted lifetime, bounded by the requested horizon |
| `initiallyInRange` | Whether the initial relative separation is within the supplied range |
| `exitsWithinHorizon` | Whether an exit was detected or predicted within the horizon |

Positions, velocities, range, acceleration, and time parameters must use consistent units. The implementation does not infer or validate a radio propagation model from the supplied range.

## 4. Repository contents

| Path | Purpose |
|---|---|
| `model/kinematic-predictor.h` | Data structures and predictor API |
| `model/kinematic-predictor.cc` | Constant-velocity and experimental acceleration-aware implementations |
| `README.md` | Scope, mathematical model, reproduction notes, and current limitations |

The strategy and beacon-history integration files are intentionally absent from this public snapshot pending review of provenance and redistribution terms. The standalone test harness and full SUMO/ns-3 scenario are also not yet included.

## 5. Quick compile check

The predictor source uses C++ standard-library facilities and declares a C++11-compatible API. From the repository root, check that the source compiles with a local C++ compiler:

~~~bash
g++ --version
g++ -std=c++11 -Wall -Wextra -Werror -Imodel \
  -c model/kinematic-predictor.cc \
  -o /tmp/kinematic-predictor.o
~~~

This is a **compile check only**; it does not execute test cases or validate the numerical results. A test executable, documented test inputs, and automated test command are planned but are not included in this snapshot yet.

A local smoke test was previously reported to exercise stationary, separating, approaching, and initially out-of-range cases. Because its harness and exact inputs are not part of this repository, those earlier results are not independently reproducible from the public snapshot.

## 6. Implementation and evaluation status

| Component or claim | Current status |
|---|---|
| Constant-relative-velocity predictor | Implemented in the public source |
| Acceleration-aware predictor API | Implemented; validation remains outstanding |
| Standalone automated tests in this repository | Not included |
| SUMO/ns-3 mobility integration | Previously exercised in a local development tree; not reproducible from this snapshot |
| Prediction-driven next-hop selection | Not implemented |
| Link-lifetime prediction accuracy against observed disconnections | Not evaluated systematically |
| Baseline comparison for packet delivery, delay, retransmissions, or overhead | Not completed |

No performance improvement is claimed in this repository.

## 7. Limitations

- **Model assumptions:** The constant-velocity branch assumes relative velocity remains unchanged over the prediction interval.
- **Finite horizon:** Long link lifetimes are censored at the supplied horizon; the default API horizon is 15 seconds.
- **Range abstraction:** A supplied range is a geometric threshold, not a radio/channel model. Its suitability depends on the selected propagation, interference, and receiver assumptions.
- **Acceleration extension:** It uses discrete time sampling and still needs numerical and mobility-trace validation.
- **Integration gap:** The predictor is not yet connected to a verified prediction-driven forwarding decision in this public repository.
- **Experimental evidence:** There are no reproducible PDR, delay, overhead, or baseline-comparison results in this snapshot.

## 8. Research roadmap

1. Add a standalone test harness with explicit inputs, expected results, edge cases, and automated execution.
2. Validate predicted lifetime against observed link-disconnection times across representative mobility traces.
3. Document and pin compatible ns-3, ndnSIM, NDN4IVC, and SUMO revisions.
4. Complete source provenance and licensing review before publishing the strategy and integration components.
5. Define a forwarding rule that actually consumes the prediction and specify its fallback behavior when a prediction is unavailable.
6. Evaluate against an unchanged baseline under identical traces and settings, reporting packet delivery ratio, end-to-end delay, retransmissions, and forwarding overhead.
7. Publish scripts, raw outputs, configuration files, and summary tables so results can be independently reproduced.

## 9. Related software

- [ndnSIM](https://github.com/named-data-ndnSIM/ndnSIM) — NS-3-based NDN simulator.
- [ndnSIM ns-3 development tree](https://github.com/named-data-ndnSIM/ns-3-dev) — custom ns-3 tree with ndnSIM-related patches.
- [SUMO documentation](https://sumo.dlr.de/docs/) — traffic simulation and TraCI documentation.

The local development environment used a custom ns-3 tree based on ns-3.30.1, ndnSIM 2.8, and SUMO 1.1.0. Exact source commit hashes and a clean-room setup procedure have not yet been pinned, so this should not be interpreted as a complete reproducibility specification.

## 10. Licensing and reuse

This snapshot does not currently include a project-level licence. The code is part of an ongoing research artifact, and the provenance and redistribution terms of the wider integration are still being reviewed. **Do not assume that this repository grants permission to reuse or redistribute every component.** A suitable licensing statement will be added once ownership and applicable upstream terms are confirmed.

---

*Research transparency note: AKPF is an implementation-in-progress. The published predictor is a component-level artifact, not evidence that an adaptive forwarding protocol improves VANET performance.*
