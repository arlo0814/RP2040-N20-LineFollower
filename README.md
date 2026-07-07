# 🤖 Mini N20 Line Follower Robot (RP2040-Zero)

An ultra-lightweight, high-speed mini line follower robot powered by a Waveshare RP2040-Zero microcontroller, driving N20 micro gear motors over an L293D motor driver, powered by a single 3.7V Lithium-ion cell.

---

## 🛠 Hardware Specifications & Bill of Materials (BOM)
* **Microcontroller:** Waveshare RP2040-Zero (Dual ARM Cortex-M0+ @ 133MHz)
* **Actuators:** 2x N20 Micro Gear Motors (6V, 300 RPM)
* **Motor Driver:** L293D Push-Pull Four-Channel Driver IC
* **Sensor Array:** 5-Channel Infrared (IR) Reflectance Sensor Module (TCRT5000/ITR9909)
* **Power Supply:** 1x 3.7V 2000mAh Lithium Polymer / 18650 Battery
* **Voltage Regulation:** MT3608 DC-DC Step-Up Boost Converter Module
* **Power Buffering:** 1x 100µF to 470µF Electrolytic Bulk Capacitor (for motor surge isolation)

---

## ⚡ Power Distribution & Voltage Architecture
Because the system operates on a single 3.7V cell, a **boost-first, split-logic** strategy is implemented to keep the MCU protected while maximizing motor output.

> ⚠️ **CRITICAL RULES:**
> 1. Never feed the boosted MT3608 output line (~7.8V) into any pin on the RP2040-Zero. It will instantly destroy the board.
> 2. Power the 5-Channel IR sensor strictly via the RP2040's `3.3V` pin to prevent `5V` logic level lines from feeding back into the 3.3V tolerant MCU GPIOs.

### Power Wiring Diagram
1. **Battery (+)** ➡️ MT3608 `IN+` **AND** RP2040-Zero `5V` pin (Onboard regulator safely handles the 3.7V-4.2V swing).
2. **Battery (-)** ➡️ MT3608 `IN-` **AND** RP2040-Zero `GND`.
3. **MT3608 OUT+** ➡️ Tuned exactly to **7.8V** ➡️ Connects *only* to L293D **Pin 8 ($V_{CC2}$ - Motor Power)**.
4. **MT3608 OUT-** ➡️ Common Ground (L293D Pins 4, 5, 12, 13).
5. **RP2040-Zero 3.3V OUT** ➡️ L293D **Pin 16 ($V_{CC1}$ - Logic Power)** **AND** IR Sensor **VCC**.

---

## 📌 Pin Mapping (Through-Hole Layout optimization)
To keep the chassis compact without requiring underside surface-mount soldering, all tracking signals utilize the primary outer header lines.

| Component | Component Pin | RP2040-Zero Pin | Function Type |
| :--- | :--- | :--- | :--- |
| **5-Ch IR Sensor** | OUT1 (Far Left) | `GP0` | Digital Input |
| | OUT2 (Mid Left) | `GP1` | Digital Input |
| | OUT3 (Center) | `GP2` | Digital Input |
| | OUT4 (Mid Right)| `GP3` | Digital Input |
| | OUT5 (Far Right)| `GP4` | Digital Input |
| **L293D Driver** | Enable 1,2 (Left Speed) | `GP5` | PWM Output |
| | Input 1 (Left Dir A) | `GP6` (Back Pad) | Digital Output |
| | Input 2 (Left Dir B) | `GP7` (Back Pad) | Digital Output |
| | Input 3 (Right Dir A)| `GP26` | Digital Output |
| | Input 4 (Right Dir B)| `GP27` | Digital Output |
| | Enable 3,4 (Right Speed)| `GP28` | PWM Output |

---

## 🏁 Control Algorithm Concept
The steering logic calculates a weighted average tracking error across the 5-channel array, mapping values from **-2 (hard left)** to **+2 (hard right)**. 

Corrections are dynamically fed into a proportional-derivative (PD) loop running inside a sub-millisecond timer loop. Motor whine is neutralized by forcing the hardware PWM frequency blocks up into the ultrasonic range (**20 kHz**).