## 5. Experimental Results and Tuning Guidelines
Empirical verification shows that the system achieves stable line tracking up to a physical track translation limit of approximately $0.65\text{ m/s}$.

### 5.1 Tuning Protocol
1. Initialize $K_d = 0$ and increment $K_p$ linearly until steady-state limit-cycle oscillations (fishtailing) occur on a straight line.
2. Introduce and step up the derivative multiplier $K_d$ to act as a dampening factor, smoothing overshoot transitions on sharp radius boundaries.
3. If the chassis fails to negotiate a 90-degree intersection, step down $V_{base}$ or expand sensor memory persistence fallbacks.