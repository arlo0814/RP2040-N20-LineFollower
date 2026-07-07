## 2. Hardware Design & Electrical Architecture
The electrical design isolates processing logic from inductive transients. The system utilizes a dual-rail power infrastructure mapped out below:

### 2.1 System Architecture Topology
The structural dependencies, voltage boundaries, and closed-loop data interactions of the system are formally modeled below using a directed graph layout.

```mermaid
graph TB
    %% Core Component Clusters
    subgraph Power_Subsystem [Power Supply & Distribution Stage]
        BAT[3.7V 2000mAh Li-ion Battery]
        MT3608[MT3608 DC-DC Step-Up Converter]
    end

    subgraph Perception_Layer [Input Subsystem]
        IRS[5-Channel IR Reflectance Array]
    end

    subgraph Processing_Core [Central Processing Unit]
        RP2040[Waveshare RP2040-Zero Core]
        LDO[Onboard 3.3V LDO Linear Regulator]
    end

    subgraph Actuation_Stage [Output Subsystem]
        L293D[L293D Dual H-Bridge Driver]
        M_Left[Left N20 Motor 6V 300RPM]
        M_Right[Right N20 Motor 6V 300RPM]
    end

    %% Power Rail Routing (Solid Lines)
    BAT == "Raw V_Batt (3.7V - 4.2V)" ==> LDO
    BAT == "Unregulated Feed" ==> MT3608
    MT3608 == "Boosted V_Motor (7.8V Rail)" ==> L293D
    LDO -.->|"Regulated V_Logic (3.3V)"| RP2040
    RP2040 -.->|"3.3V Logic Power Rail"| IRS
    RP2040 -.->|"Pin 16 (Vcc1) Logic Reference"| L293D

    %% Data / Signaling Paths (Arrow Connections)
    IRS -->|"5x Digitized Input Signals (GP0 - GP4)"| RP2040
    RP2040 -->|"2x Ultrasonic 20kHz PWM (GP5, GP28)"| L293D
    RP2040 -->|"4x Direction States (GP6, GP7, GP26, GP27)"| L293D
    L293D -->|"Isolated High-Current Drive"| M_Left
    L293D -->|"Isolated High-Current Drive"| M_Right

    %% Style Class Allocations
    style BAT fill:#2c3e50,stroke:#fff,stroke-width:2px,color:#fff
    style MT3608 fill:#e67e22,stroke:#fff,stroke-width:2px,color:#fff
    style RP2040 fill:#2980b9,stroke:#fff,stroke-width:2px,color:#fff
    style L293D fill:#8e44ad,stroke:#fff,stroke-width:2px,color:#fff
    style IRS fill:#27ae60,stroke:#fff,stroke-width:2px,color:#fff
```

### 2.2 Transient Noise Mitigation
A bulk electrolytic capacitor ($100\mu\text{F} - 470\mu\text{F}$) is positioned across the MT3608 output terminals to serve as a low-impedance reservoir during abrupt motor direction or velocity changes, preventing voltage sags from causing microcontroller resets.    