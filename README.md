## Project_DoorDetector
# DOOR STATE DETECTOR
This project detects whether a door is open or closed using infrared reflection. It runs on two Adafruit HUZZAH32 boards.

# HOW IT WORKS
One HUZZAH32 sits at the door and reads the WWZMDiB 3 wire IR sensor. The sensor reflects IR off the door surface. When the door sits in front of the sensor, IR bounces back and the output reads one state. When the door swings open, the surface moves out of range and the output flips. A trim pot on the sensor sets the trigger distance.
The second HUZZAH32 acts as the receiver. It gets the door state and lights an LED to show open or closed.

# HARDWARE
- 2x Adafruit HUZZAH32
- WWZMDiB 3 wire IR sensor module
- 2x tactile pushbuttons, one on each HUZZAH32
- Status LED on each HUZZAH32, shows whether that board is awake
- LED on the receiver HUZZAH32, shows door open or closed state
- 220 ohm resistors for all LEDs
- 10k ohm resistors for both tactile buttons
- Rechargeable AA battery pack, wired directly to power and ground

# FIRMWARE
Button input is debounced in firmware on both boards.
Both boards run deep sleep to cut power draw. Each board wakes from deep sleep when you press its tactile button. The status LED on each board turns on when that board is awake and off when it drops back into deep sleep. This keeps both boards off when user does not need it. Utilized ESPnow library to communicate data between microcontrollers.

# POWER
The device runs on rechargeable AA cells instead of a lithium ion pack. Power and ground connect directly to the microcontroller with no charge circuit on board. Charging happens outside the device, in a standalone AA charger.

# STATUS
Breadboard prototype complete. PCB layout in progress in KiCad, targeting a compact 3D printed enclosure.

# NEXT STEPS
- Finish PCB layout in KiCad
- Design 3D printed enclosure around the board

