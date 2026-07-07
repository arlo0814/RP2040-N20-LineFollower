## 3. Mathematical Modeling & Control Loop Theory
Steering correction is computed using a discrete-time parallel Proportional-Derivative (PD) controller. 

### 3.1 Error Normalization
The spatial deviation $e(t)$ relative to the track center line is extracted from a 5-channel array using localized spatial weights $w_i \in \{-2, -1, 0, 1, 2\}$:

$$e(t) = \frac{\sum_{i=1}^{5} w_i \cdot S_i}{\sum_{i=1}^{5} S_i}$$

Where $S_i \in \{0, 1\}$ represents the digitized state of the $i$-th reflectance channel.

### 3.2 Manipulated Variable Computation
The plant input command $u(t)$ dictates differential wheel speeds through the controller transfer function:

$$u(t) = K_p \, e(t) + K_d \, \frac{e(t) - e(t-1)}{\Delta t}$$

The resulting correction parameter alters the base cruise duty cycle ($V_{base}$) split across left and right actuation channels:

$$V_{left} = V_{base} + u(t), \quad V_{right} = V_{base} - u(t)$$