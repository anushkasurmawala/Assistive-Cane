 

Smart Blind Stick – IoT-Enabled Assistive Navigation System

An IoT-oriented assistive stick designed to improve environmental awareness and mobility for visually impaired users through multi-directional obstacle detection, voice guidance, haptic feedback, and ground-level hazard detection.

This project is currently under development. The present prototype focuses on reliable proximity sensing and multimodal user feedback, with additional intelligent navigation and IoT features planned for future versions.

⸻

Overview

Traditional white canes are effective for detecting obstacles close to the ground but may provide limited information about obstacles outside the cane’s physical contact range.

The Smart Blind Stick extends this concept using electronic sensors to detect nearby obstacles and provide the user with immediate feedback.

The current prototype combines:

* Multi-directional ultrasonic sensing
* Approximate distance-to-step estimation
* Voice-based navigation guidance
* Directional vibration feedback
* Ground-level drop/pothole detection
* Automatic safe-direction suggestions

The system is designed so that additional IoT, computer vision, and navigation capabilities can be integrated in later development stages.

⸻

Current Features

Multi-Directional Obstacle Detection

Ultrasonic sensors monitor multiple directions around the user:

* Front
* Left
* Right
* Rear
* Ground/downward direction

The sensors continuously measure the distance between the stick and nearby objects.

⸻

Voice Guidance

A DFPlayer Mini module provides prerecorded audio instructions to communicate navigation information to the user.

Depending on sensor readings, the system can provide instructions such as:

* Move left
* Move right
* Stop
* Obstacle behind
* Pothole/drop ahead

Voice announcements include a cooldown mechanism to prevent continuous repetition of the same alerts.

⸻

Approximate Step-Based Distance Guidance

Detected obstacle distance can be converted into an approximate number of walking steps.

For example:

Obstacle detected approximately 2 steps ahead.

The current implementation uses a configurable estimated step length. Future versions can incorporate individual stride calibration for more personalized distance estimation.

⸻

Safe-Direction Suggestion

When an obstacle is detected directly ahead, the system compares the available clearance on the left and right sides.

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
         Select clearer path

The system can then recommend:

Move Left

or

Move Right

If neither side provides sufficient clearance:

STOP

This prevents the system from blindly recommending a predetermined direction when an obstacle is detected.

⸻

Haptic Feedback

Vibration motors provide immediate tactile feedback when nearby obstacles are detected.

Directional vibration is currently available for:

* Front
* Left
* Right
* Rear

Haptic feedback complements voice guidance and provides an immediate warning channel.

⸻

Ground Drop / Pothole Detection

A downward-facing ultrasonic sensor monitors the distance between the stick and the ground.

A significant increase in downward distance can indicate a possible:

* Pothole
* Drop
* Step-down
* Missing surface

When a potential hazard is detected, the system activates an audible warning and voice alert.

Note: The current prototype uses threshold-based ground detection. Adaptive ground calibration and stick-angle compensation are planned for future versions.

⸻

Hardware Components

The current prototype uses:

Component	Purpose
Microcontroller	Main processing and control
Ultrasonic Sensors	Multi-directional distance measurement
DFPlayer Mini	Prerecorded voice feedback
Speaker / Earphone	Audio output
Vibration Motors	Haptic obstacle warnings
Buzzer	Ground hazard warning
MicroSD Card	Stores voice files
Battery / Power Supply	Portable power source

Additional components will be introduced as the project develops.

⸻

Software

The firmware is developed using:

* Arduino IDE
* C/C++
* DFRobotDFPlayerMini library
* SoftwareSerial library

⸻

System Workflow

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
 Obstacle Check    Ground Check
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
      Repeat

⸻

Repository Structure

Smart-Blind-Stick-IoT/
|
├── Smart_Blind_Stick/
│   └── Smart_Blind_Stick.ino
│
├── README.md
├── LICENSE
└── .gitignore

⸻

Arduino Libraries

Install the following libraries before compiling the firmware:

DFRobotDFPlayerMini

Used to communicate with the DFPlayer Mini audio module.

#include <DFRobotDFPlayerMini.h>

SoftwareSerial

Used for serial communication with the DFPlayer Mini.

#include <SoftwareSerial.h>

⸻

Audio File Configuration

The DFPlayer Mini uses audio files stored on a MicroSD card.

Example configuration:

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

The exact file numbering should match the constants configured in the Arduino firmware.

⸻

Configuration

Several parameters can be adjusted in the firmware.

Obstacle Detection Range

const int OBSTACLE_THRESHOLD = 100;

Defines the distance in centimetres at which an object is considered an obstacle.

Critical Distance

const int CRITICAL_DISTANCE = 25;

Objects within this distance trigger a high-priority stop warning.

Approximate Step Length

const int STEP_LENGTH_CM = 50;

Used to estimate the number of walking steps between the user and an obstacle.

These values should be calibrated during physical testing.

⸻

Planned Development

Future versions of the Smart Blind Stick may explore:

* Adaptive ground and pothole detection
* IMU-based stick-angle compensation
* Personalized stride calibration
* GPS-based outdoor navigation
* Emergency SOS and location sharing
* Mobile application connectivity
* IoT-based remote monitoring
* Camera-based environmental perception
* Object recognition
* Staircase recognition
* Traffic signal recognition
* Indoor navigation
* Fall detection
* Context-aware voice guidance
* Sensor fusion for improved environmental understanding

These features are planned concepts and are not claimed as implemented in the current prototype.

⸻

Safety Notice

This project is an experimental assistive-technology prototype developed for educational and research purposes.

It has not been clinically validated or certified as a medical or mobility device and should not be treated as a replacement for professionally tested assistive equipment or established mobility techniques.

⸻

Development Status

Current Stage: Prototype / Active Development

Current development priorities include:

1. Improving ultrasonic measurement reliability
2. Reducing sensor cross-talk
3. Testing obstacle-detection thresholds
4. Improving voice-feedback timing
5. Calibrating ground-hazard detection
6. Conducting controlled real-world testing

⸻

Author

Anushka Surmawala

B.Tech – Computer Science and Engineering (IoT)
G. H. Patel College of Engineering and Technology

⸻
 
