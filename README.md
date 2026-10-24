# Smart Blind Stick – IoT-Enabled Assistive Navigation System

An assistive navigation system designed to improve environmental awareness and mobility for visually impaired users through **multi-directional obstacle detection, voice guidance, haptic feedback, ground-level hazard detection, and safe-direction assistance**.

---

## Overview

Traditional white canes help visually impaired users identify obstacles through physical contact. However, they may provide limited information about obstacles outside the cane's immediate contact range.

The **Smart Blind Stick** extends this functionality using ultrasonic sensors and multimodal feedback to detect nearby obstacles and provide navigation assistance.

The system combines:

- Multi-directional ultrasonic sensing
- Approximate step-based distance estimation
- Voice-based navigation guidance
- Directional vibration feedback
- Ground-level pothole/drop detection
- Automatic safe-direction suggestions

---

## Features

### 1. Multi-Directional Obstacle Detection

Ultrasonic sensors monitor multiple directions around the user:

- Front
- Left
- Right
- Rear
- Ground / downward direction

The sensors continuously measure the distance between the stick and nearby objects.

---

### 2. Voice Guidance

A **DFPlayer Mini** module provides prerecorded audio instructions based on the sensor readings.

The system can provide instructions such as:

- "Move Left"
- "Move Right"
- "Stop"
- "Obstacle Behind"
- "Pothole Ahead"

A cooldown mechanism prevents continuous repetition of the same voice alert.

---

### 3. Approximate Step-Based Distance Guidance

The system converts the measured distance of an obstacle into an approximate number of walking steps.

For example:

```text
Obstacle detected approximately 2 steps ahead.
```

The estimated step length can be configured according to the user's walking pattern.

---

### 4. Safe-Direction Suggestion

When an obstacle is detected directly ahead, the system compares the available clearance on the left and right sides.

```text
                 FRONT
               Obstacle
                   |
                   v
LEFT <---- Smart Stick ----> RIGHT
 |                               |
Distance                       Distance
 |                               |
 +---------- Compare ------------+
                   |
                   v
         Select Clearer Path
```

Based on the available space, the system can recommend:

```text
Move Left
```

or

```text
Move Right
```

If neither direction provides sufficient clearance:

```text
STOP
```

This allows the stick to determine a safer direction instead of providing a predetermined directional response.

---

### 5. Haptic Feedback

Vibration motors provide tactile feedback when nearby obstacles are detected.

Directional vibration is provided for:

- Front
- Left
- Right
- Rear

Haptic feedback complements voice guidance by providing an immediate physical warning.

---

### 6. Ground Drop / Pothole Detection

A downward-facing ultrasonic sensor monitors the distance between the stick and the ground.

An increased downward distance can indicate a possible:

- Pothole
- Drop
- Step-down
- Missing surface

When a potential ground hazard is detected, the system activates the buzzer and provides a voice warning.

---

## Hardware Components

| Component | Purpose |
|---|---|
| Microcontroller | Main processing and system control |
| Ultrasonic Sensors | Multi-directional distance measurement |
| DFPlayer Mini | Prerecorded voice feedback |
| Speaker / Earphone | Audio output |
| Vibration Motors | Haptic obstacle warnings |
| Buzzer | Ground-hazard warning |
| MicroSD Card | Stores voice files |
| Battery / Power Supply | Portable power source |

---

## Software and Technologies

The firmware uses:

- Arduino IDE
- C / C++
- DFRobotDFPlayerMini Library
- SoftwareSerial Library

---

## System Workflow

```text
              START
                |
                v
        Initialize Sensors
                |
                v
      Measure Surroundings
                |
        +-------+-------+
        |               |
        v               v
 Obstacle Check     Ground Check
        |               |
        v               v
Calculate Distance   Detect Drop
        |               |
        v               v
Compare Left/Right   Hazard Alert
        |
        v
Select Safer Direction
        |
        v
 Voice + Haptic Feedback
        |
        v
             REPEAT
```

---

## Repository Structure

```text
Smart-Blind-Stick-IoT/
|
├── Smart_Blind_Stick/
│   └── Smart_Blind_Stick.ino
│
└── README.md
```

---

## Required Arduino Libraries

Install the following libraries before compiling the firmware.

### DFRobotDFPlayerMini

Used to communicate with the DFPlayer Mini audio module.

```cpp
#include <DFRobotDFPlayerMini.h>
```

### SoftwareSerial

Used for serial communication with the DFPlayer Mini.

```cpp
#include <SoftwareSerial.h>
```

---

## Audio File Configuration

The DFPlayer Mini uses prerecorded audio files stored on a MicroSD card.

Example configuration:

```text
001.mp3 -> "One"
002.mp3 -> "Two"
003.mp3 -> "Three"
004.mp3 -> "Four"
005.mp3 -> "Five"

010.mp3 -> "Move Left"
011.mp3 -> "Move Right"
012.mp3 -> "Stop"
013.mp3 -> "Pothole Ahead"
014.mp3 -> "Obstacle Ahead"
015.mp3 -> "Obstacle Behind"
```

The audio file numbering must correspond with the constants configured in the Arduino firmware.

---

## Configuration

Several system parameters can be adjusted in the firmware.

### Obstacle Detection Range

```cpp
const int OBSTACLE_THRESHOLD = 100;
```

Defines the distance in centimetres at which an object is considered an obstacle.

### Critical Distance

```cpp
const int CRITICAL_DISTANCE = 25;
```

Objects detected within this distance trigger a high-priority stop warning.

### Approximate Step Length

```cpp
const int STEP_LENGTH_CM = 50;
```

Used to estimate the approximate number of walking steps between the user and an obstacle.

---

## Working Principle

The Smart Blind Stick continuously collects distance measurements from ultrasonic sensors positioned in different directions.

When an obstacle is detected within the configured threshold:

1. The corresponding vibration motor is activated.
2. The distance to the obstacle is calculated.
3. The distance is converted into an approximate number of steps.
4. For a front obstacle, the system compares the left and right sensor readings.
5. The direction with greater clearance is selected.
6. Voice guidance communicates the recommended action to the user.
7. If both directions are blocked, the system issues a stop warning.

The downward-facing sensor independently monitors the ground and activates an alert when a potential drop or pothole is detected.

---

## Safety Notice

This project is an experimental assistive-technology prototype created for educational purposes.

It has not been clinically validated or certified as a mobility or medical device and should not be considered a replacement for professionally tested assistive equipment or established mobility techniques.

---

## Author

**Anushka Surmawala**

B.Tech – Computer Science and Engineering (IoT)  
G. H. Patel College of Engineering and Technology
