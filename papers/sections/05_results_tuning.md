## 5. Experimental Results & Tuning Guidelines

Calibrating a high-speed line follower with low mechanical inertia requires a structured tuning protocol. This section outlines the empirical methodology used to determine the optimal Proportional ($K_p$) and Derivative ($K_d$) gain multipliers and details the operational troubleshooting matrix.

### 5.1 Empirical PID Tuning Protocol (Ziegler-Nichols Derivative Adaptation)
To minimize overshoot and eliminate steady-state oscillations on high-radius turns, a manual heuristic tuning procedure was executed under fixed battery voltage states ($V_{\text{motor}} = 7.8\text{V}$):

1. **Zero Calibration Baseline:** Initialize both parameters to zero ($K_p = 0.0$; $K_d = 0.0$) and establish a conservative base cruise velocity ($V_{\text{base}} = 500$);
2. **Proportional Gain Scaling:** Increment $K_p$ slowly until the robot can follow a straight trajectory and negotiate gentle arcs; Continue increasing $K_p$ until the chassis begins a continuous, uniform oscillation pattern across the track (commonly referred to as "fishtailing" or limit-cycle behavior); This boundary represents the critical gain ($K_{cr}$);
3. **Proportional Setpoint Derivation:** Reduce the $K_p$ value to approximately $60\% - 70\%$ of the critical oscillation threshold to stabilize the primary restoration torque;
4. **Derivative Dampening Injection:** Introduce the derivative multiplier $K_d$ in small increments; The derivative term introduces predictive dampening by monitoring the error velocity slope ($\Delta e$); Increase $K_d$ until the hunting behavior disappears and the chassis stabilizes immediately when returning to a straight section after a sharp radius transition.

---

### 5.2 System Calibration & Diagnostics Matrix
When implementing a bare-metal control system on the RP2040 micro-architecture, tracking failures typically correlate to specific electrical or mechanical thresholds. The matrix below defines standard failure modes alongside their control-loop remedies:

| Observed Behavior Field | Probable Root Cause Category | Corrective System Action |
| :--- | :--- | :--- |
| **Chassis oscillates violently on straight tracks** | Over-corrected proportional gain setpoint | Step down $K_p$ by $15\%$ or scale up $K_d$ to increase loop dampening; |
| **Robot loses tracking at sharp 90-degree curves** | Base cruise velocity exceeds motor saturation limits | Lower $V_{\text{base}}$ parameter or implement an automated speed reduction scaling rule when $\lvert error \rvert \ge 1.5$; |
| **Microcontroller resets when motors reverse direction** | High inductive current surges pulling down shared logic rails | Verify the placement of the $470\mu\text{F}$ bulk capacitor across the MT3608 output lines; |
| **Robot fails to turn and drives straight off curves** | Insufficient restoration torque or slow derivative reaction | Increase $K_p$ or verify that sensor logic matching is not inverted in the parsing loop; |
| **High-pitched whistling sound coming from motor windings** | Audible pulse-width modulation carrier frequency resonance | Ensure `analogWriteFreq(20000)` is executing inside setup to force ultrasonic switching; |

Using this structured approach, the final stable operational parameters for the N20 micro gear powertrain configuration were derived as:

$$K_p = 150.0; \quad K_d = 80.0; \quad V_{\text{base}} = 600;$$

This configuration maintains a deterministic cycle update latency under $12\mu\text{s}$, achieving continuous track alignment at velocities up to $0.65\text{ m/s}$.