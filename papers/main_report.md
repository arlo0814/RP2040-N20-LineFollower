# Comprehensive Engineering Design Paper: Autonomous Mini Line Follower Utilizing a Dual-Rail RP2040 Architecture

**Author:** Embedded Systems Development Log  
**Environment:** VS Code & PlatformIO IDE Core  
**Target Hardware:** Waveshare RP2040-Zero & N20 Actuators  

---

## 📋 Document Overview
This document serves as the master assembly registry for the formal engineering analysis. The system implementation details are broken down into modular section blocks for clear tracking and version control.

---

### Section 1: Executive Summary
*Refer to `sections/01_abstract.md` for the standalone publication text.*

### Section 2: Hardware Design & Physical Topology
*Refer to `sections/02_hardware_architecture.md` for the complete Mermaid diagram and dual-rail voltage isolation profiles.*

### Section 3: Mathematical Modeling & Kinematics
*Refer to `sections/03_control_theory.md` for the discrete parallel Proportional-Derivative LaTeX transfer functions.*

### Section 4: Software Hardware-Interface Map
*Refer to `sections/04_software_interface.md` for the 20 kHz ultrasonic timer clock calculations and driver truth tables.*

### Section 5: Experimental Verification & Calibration Log
*Refer to `sections/05_results_tuning.md` for the steady-state tracking parameters and diagnostic matrices.*

---

## 6. Sourcing & Reference Appendix
1. Raspberry Pi Ltd. (2021). *RP2040 Microcontroller Datasheet: Hardware Design with the RP2040.*
2. STMicroelectronics. (2003). *L293D Push-Pull Four-Channel Driver Technical Specifications.*
3. Ogata, K. (2010). *Modern Control Engineering (5th Edition).* Prentice Hall. (Discrete Time Feedback Dampening Frameworks).