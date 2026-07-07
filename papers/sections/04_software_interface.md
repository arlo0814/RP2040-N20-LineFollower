## 4. Software Hardware-Interface Map
The firmware runs on the bare-metal Arduino framework compiled through PlatformIO for the RP2040 core. 

### 4.1 Ultrasonic PWM Modifications
Standard microcontroller PWM frequencies ($\sim490\text{ Hz}$) induce audible magnetic magnetostriction hum within the N20 motor coils and degrade fine low-duty velocity responses. The hardware timers of the RP2040 are forced to run at $20\text{ kHz}$ with 10-bit resolution ($0 - 1023$ discrete steps). This elevates the switching transitions above human and biological acoustic limits while enabling precise differential velocity transitions.