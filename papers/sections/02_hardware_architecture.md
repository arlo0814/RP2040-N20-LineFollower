## 2. Hardware Design & Electrical Architecture
The electrical design isolates processing logic from inductive transients. The system utilizes a dual-rail power infrastructure mapped out below:

### 2.1 Power Distribution Mechanics
* **Logic Subsystem:** Supplied by the raw battery terminal ($3.7\text{V} - 4.2\text{V}$) connected directly to the `5V` header of the RP2040-Zero, relying on the onboard low-dropout (LDO) linear regulator to deliver a clean $3.3\text{V}$ rail.
* **Actuation Subsystem:** The raw battery line is stepped up via an MT3608 boost converter calibrated to $7.8\text{V} - 8.0\text{V}$, which directly drives Pin 8 ($V_{CC2}$) of the L293D. This counteracts the inherent $\sim1.8\text{V}$ internal Darlington saturation drop, exposing the 6V 300 RPM N20 micro gear motors to their absolute nominal rating.

### 2.2 Transient Noise Mitigation
A bulk electrolytic capacitor ($100\mu\text{F} - 470\mu\text{F}$) is positioned across the MT3608 output terminals to serve as a low-impedance reservoir during abrupt motor direction or velocity changes, preventing voltage sags from causing microcontroller resets.    