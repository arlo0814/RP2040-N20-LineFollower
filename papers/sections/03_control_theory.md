## 3. Mathematical Modeling & Control Loop Theory

To achieve high-speed tracking and stability on changing track geometry without introducing jerky, non-linear adjustments, the robot uses a discrete closed-loop system. This section details the mathematical modeling used to calculate positional error and compute the resulting control corrections.

### 3.1 Spatial Error Normalization & Center-of-Mass Tracking
The sensor sub-system consists of $5$ infrared phototransistor channels arranged linearly. Rather than relying on rigid conditional logic, the tracking error $e(t)$ is calculated using an objective spatial-weighting function. 

Each discrete sensor is assigned a static geometric weight $w_i$, which corresponds to its physical distance and orientation relative to the chassis's longitudinal centerline:

$$w_i \in \{-2, \, -1, \, 0, \, 1, \, 2\}$$

Let $S_i(t) \in \{0, 1\}$ be the digitized state of the $i$-th sensor channel at a given time step $t$, where $S_i = 1$ denotes a black tracking line detection and $S_i = 0$ represents a highly reflective white substrate. The normalized positional error $e(t)$ is calculated as the weighted center of mass of the active sensors:

$$e(t) = \frac{\sum_{i=1}^{5} w_i \cdot S_i(t)}{\sum_{i=1}^{5} S_i(t)}$$

#### 3.1.1 Boundary Conditions and Lost-Line State Persistence
* **Symmetric Centering:** When the line is centered directly under the mid-array channel ($S_3$), the active state matrix returns $[0, 0, 1, 0, 0]$, resulting in a perfect steady-state value of $e(t) = 0$.
* **Wide-Line Integration:** If the robot straddles an intersection such that multiple sensors trigger simultaneously (e.g., $[0, 1, 1, 0, 0]$), the division by $\sum S_i$ normalizes the calculation to return an interpolated mid-point error of $e(t) = -0.5$.
* **Null-State Memory Allocation:** If the track profile is lost entirely ($\sum_{i=1}^{5} S_i(t) = 0$), the denominator becomes zero. To prevent undefined calculations, the system checks a conditional persistence state:

$$e(t) = \begin{cases} -2 & \text{if } e(t-1) < 0 \\ +2 & \text{if } e(t-1) > 0 \\ 0 & \text{otherwise} \end{cases}$$

This forces the robot to execute an aggressive pivot in the direction it last saw the line until tracking is re-established.

---

### 3.2 Discrete Parallel Proportional-Derivative (PD) Control
The error parameter $e(t)$ is passed directly to a parallel-form Proportional-Derivative control loop running at a sampling rate of $\Delta t \approx 1\text{ ms}$. Because a micro line follower has minimal physical inertia, an Integral term ($I$) is intentionally omitted to avoid windup oscillations near high-frequency path transitions.

The continuous control function is converted into a discrete-time difference equation:

$$u(t) = K_p \, e(t) + K_d \, \left[ \frac{e(t) - e(t-1)}{\Delta t} \right]$$

Since the cycle frequency is bound tightly by hardware timers to a fixed constant ($\Delta t = 1$), the time division factor is rolled into the derivative gain constant ($K_d$), simplifying the execution math into:

$$u(t) = K_p \, e(t) + K_d \, \Delta e(t)$$

Where:
* $e(t)$ is the current calculated tracking offset.
* $\Delta e(t) = e(t) - e(t-1)$ represents the instantaneous derivative slope (velocity of error change).
* $K_p$ represents the proportional gain constant responsible for generating restoration torque proportional to the displacement.
* $K_d$ represents the derivative gain constant responsible for predicting error trajectory and dampening mechanical overshoot.

---

### 3.3 Differential Actuation Transformation
The computed control output $u(t)$ defines an abstract correction value that must be translated into physical wheel velocities. This is achieved through an inverse differential steering transformation matrix applied to the base cruise velocity ($V_{\text{base}}$):

$$\begin{aligned}
V_{\text{left}}(t)  &= V_{\text{base}} + u(t) \\
V_{\text{right}}(t) &= V_{\text{base}} - u(t)
\end{aligned}$$

#### 3.3.1 Boundary Constraints and Anti-Saturation Rules
Because the RP2040 configuration scales its high-frequency PWM timers to a 10-bit resolution ceiling ($0 \le \text{PWM} \le 1023$), the actual voltage commands are restricted by strict boundary limits to prevent integer overflow errors or driver saturation:

$$V_{\text{out}} = \min(\max(V, \, 0), \, 1023)$$

When navigating sharp turns, this clamping mechanism automatically handles motor braking. If a severe right-hand error ($e(t) = +2$) forces $u(t) > V_{\text{base}}$, $V_{\text{right}}$ is clamped hard at $0$, locking the right wheel to act as a pivot point, while $V_{\text{left}}$ receives maximum drive power to snap the robot back on track.