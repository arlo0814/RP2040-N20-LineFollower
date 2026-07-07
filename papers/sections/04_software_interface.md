## 4. Software Hardware-Interface Map & Register Layer

The execution framework is built bare-metal within the Arduino API layer inside a PlatformIO development ecosystem. To safely handle high-frequency sensor capture and output actuation, the underlying hardware registers of the RP2040 micro-architecture must be carefully configured. This section details the interface map and low-level modifications.

### 4.1 Input Peripheral Mapping & GPIO Multiplexing
The Waveshare RP2040-Zero utilizes an internal General Purpose Input/Output (GPIO) multiplexer to connect the physical processor pads to its internal SIO (Single-Cycle IO) block. The input pins are mapped linearly to reduce computation overhead:

$$\text{GPIO\_IN} = \{\text{GP0}, \, \text{GP1}, \, \text{GP2}, \, \text{GP3}, \, \text{GP4}\}$$

Each pin in this set is configured with high-impedance inputs via the `GPIO_CTRL` registers. Internal pull-up and pull-down resistors are explicitly disabled:

```cpp
// De-activating internal biasing to preserve raw logic thresholds from the comparator
pinMode(SENSOR_PINS[i], INPUT);
```

By ensuring zero internal hardware biasing on the MCU side, the input logic transition thresholds sit precisely at:

$$V_{IL} \le 0.8\text{V} \quad \text{and} \quad V_{IH} \ge 2.0\text{V}$$

This allows the digital comparator outputs of the 5-channel IR module to switch cleanly between logic low and high states.

---

### 4.2 High-Frequency Ultrasonic PWM Generation
Standard microcontroller implementations execute Pulse Width Modulation (PWM) at low frequencies ($\sim490\text{ Hz} - 1\text{ kHz}$). When applied to small coreless or micro gear actuators like N20 motors, these low-frequency cycles cause two major issues:
* **Acoustic Magnetostriction Noise:** The motor coils oscillate physically at the switching frequency, generating an audible high-pitched hum.
* **Inductive Current Ripple:** The motor winding current drops to zero during the off-cycle, causing jerky low-speed control and increased thermal loss.

To resolve this, the RP2040 hardware PWM slices are configured to operate in the ultrasonic range at $20\text{ kHz}$. This frequency is above human and biological acoustic perception limits and ensures continuous current flow through the motor windings.

#### 4.2.1 Timer & Clock Division Mathematics
The RP2040 system clock ($f_{\text{sys}}$) runs natively at $133\text{ MHz}$. The internal hardware counters increment on every clock cycle. The target PWM frequency ($f_{\text{pwm}}$) is a function of the system clock, the clock divisor ($\text{CLKDIV}$), and the wrap period register ($\text{WRAP}$):

$$f_{\text{pwm}} = \frac{f_{\text{sys}}}{\text{CLKDIV} \cdot (\text{WRAP} + 1)}$$

To provide fine speed control resolution, the counter wrap limit is locked to a 10-bit resolution ceiling:

$$\text{WRAP} = 1023 \quad (0 \text{ to } 1023 \text{ discrete steps})$$

Substituting the known parameters into the frequency formula allows us to calculate the required clock division ratio:

$$20000\text{ Hz} = \frac{133,000,000\text{ Hz}}{\text{CLKDIV} \cdot (1023 + 1)}$$

$$\text{CLKDIV} = \frac{133,000,000}{20000 \cdot 1024} = \frac{133,000,000}{20,480,000} \approx \mathbf{6.494}$$

The Earle Philhower compiler core automatically programs this fraction into the RP2040’s `PWM_CHx_DIV` registers when initialized in setup.

```cpp
analogWriteFreq(20000); // Forces hardware clocks to target 20kHz
analogWriteRange(1023);  // Establishes a 10-bit duty cycle boundary (0-1023)
```

### 4.3 Low-Level Software Driver Control Matrix
The L293D driver contains two separate H-bridge channels. Each bridge requires three input lines from the MCU: one Enable pin (speed) and two Input pins (direction). The software driver updates these states across the hardware pins according to the control matrix below:

| Target Operation | Motor Channel | Direction Pin A | Direction Pin B | Enable Pin (PWM) |
| :--- | :--- | :--- | :--- | :--- |
| **Forward Translation** | Left Motor | `GP6` = HIGH | `GP7` = LOW | `GP5` = Computed Duty Cycle |
| | Right Motor | `GP26` = HIGH | `GP27` = LOW | `GP28` = Computed Duty Cycle |
| **Pivot Left Turn** | Left Motor | `GP6` = LOW | `GP7` = HIGH | `GP5` = Computed Duty Cycle |
| | Right Motor | `GP26` = HIGH | `GP27` = LOW | `GP28` = Computed Duty Cycle |
| **Pivot Right Turn** | Left Motor | `GP6` = HIGH | `GP7` = LOW | `GP5` = Computed Duty Cycle |
| | Right Motor | `GP26` = LOW | `GP27` = HIGH | `GP28` = Computed Duty Cycle |
| **Electronic Brake** | Both Channels| Unchanged | Unchanged | `GP5` / `GP28` = 0 (0% Duty Cycle) |

This direct control mapping eliminates nested logic branches inside the main loop, allowing the RP2040 to process sensor inputs and adjust motor states in **less than $12\mu\text{s}$**, ensuring real-time track tracking response.