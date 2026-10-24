#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// =====================================================
// ULTRASONIC SENSOR PIN ASSIGNMENTS
// =====================================================

#define TRIG_FRONT 2
#define ECHO_FRONT 7

#define TRIG_BACK 3
#define ECHO_BACK 8

#define TRIG_LEFT 4
#define ECHO_LEFT 9

#define TRIG_RIGHT 5
#define ECHO_RIGHT 10

#define TRIG_DOWN 6
#define ECHO_DOWN 11


// =====================================================
// OUTPUT PIN ASSIGNMENTS
// =====================================================

#define VIBRATION_MOTOR_FRONT 12
#define VIBRATION_MOTOR_BACK A2
#define VIBRATION_MOTOR_LEFT A0
#define VIBRATION_MOTOR_RIGHT A1

#define BUZZER 13


// =====================================================
// DFPLAYER SETUP
// =====================================================

SoftwareSerial mySerial(14, 15);   // RX, TX
DFRobotDFPlayerMini myDFPlayer;


// =====================================================
// SYSTEM SETTINGS
// =====================================================

// Obstacle detection distance
const int OBSTACLE_THRESHOLD = 100;   // cm

// Emergency/very close obstacle distance
const int CRITICAL_DISTANCE = 25;     // cm

// Approximate walking step length
const int STEP_LENGTH_CM = 50;        // cm

// Maximum useful step announcement
const int MAX_STEPS = 5;

// Ultrasonic timeout
const unsigned long ULTRASONIC_TIMEOUT = 30000;

// Delay between individual ultrasonic measurements
const int SENSOR_GAP = 40;

// Prevent repeated voice announcements
const unsigned long VOICE_COOLDOWN = 3000;

unsigned long lastVoiceTime = 0;


// =====================================================
// AUDIO FILE NUMBERS
// Change these according to files on SD card
// =====================================================

// 001.mp3 = "One"
// 002.mp3 = "Two"
// 003.mp3 = "Three"
// 004.mp3 = "Four"
// 005.mp3 = "Five"

#define AUDIO_MOVE_LEFT       10
#define AUDIO_MOVE_RIGHT      11
#define AUDIO_STOP            12
#define AUDIO_POTHOLE         13
#define AUDIO_OBSTACLE_FRONT  14
#define AUDIO_OBSTACLE_BACK   15


// =====================================================
// SETUP
// =====================================================

void setup() {

  // Ultrasonic sensors
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);

  pinMode(TRIG_BACK, OUTPUT);
  pinMode(ECHO_BACK, INPUT);

  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);

  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  pinMode(TRIG_DOWN, OUTPUT);
  pinMode(ECHO_DOWN, INPUT);


  // Vibration motors
  pinMode(VIBRATION_MOTOR_FRONT, OUTPUT);
  pinMode(VIBRATION_MOTOR_BACK, OUTPUT);
  pinMode(VIBRATION_MOTOR_LEFT, OUTPUT);
  pinMode(VIBRATION_MOTOR_RIGHT, OUTPUT);

  pinMode(BUZZER, OUTPUT);


  // Ensure outputs start OFF
  digitalWrite(VIBRATION_MOTOR_FRONT, LOW);
  digitalWrite(VIBRATION_MOTOR_BACK, LOW);
  digitalWrite(VIBRATION_MOTOR_LEFT, LOW);
  digitalWrite(VIBRATION_MOTOR_RIGHT, LOW);

  digitalWrite(BUZZER, LOW);


  // Serial communication
  Serial.begin(9600);
  mySerial.begin(9600);


  // DFPlayer initialization
  if (!myDFPlayer.begin(mySerial)) {

    Serial.println("DFPlayer Mini not detected.");

    while (true) {
      delay(1000);
    }
  }


  myDFPlayer.volume(20);

  Serial.println("Smart Blind Stick Started");
}


// =====================================================
// ULTRASONIC DISTANCE FUNCTION
// =====================================================

long getDistance(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);


  unsigned long duration =
      pulseIn(echoPin, HIGH, ULTRASONIC_TIMEOUT);


  // No echo received
  if (duration == 0) {
    return -1;
  }


  long distance = duration * 0.0343 / 2;


  // Ignore unrealistic readings
  if (distance < 2 || distance > 400) {
    return -1;
  }


  return distance;
}


// =====================================================
// CONVERT DISTANCE INTO APPROXIMATE STEPS
// =====================================================

int getSteps(long distance) {

  if (distance <= 0) {
    return 0;
  }

  int steps =
      (distance + STEP_LENGTH_CM - 1) / STEP_LENGTH_CM;

  if (steps < 1) {
    steps = 1;
  }

  if (steps > MAX_STEPS) {
    steps = MAX_STEPS;
  }

  return steps;
}


// =====================================================
// CHECK WHETHER AUDIO CAN BE PLAYED
// =====================================================

bool canSpeak() {

  if (millis() - lastVoiceTime >= VOICE_COOLDOWN) {

    lastVoiceTime = millis();

    return true;
  }

  return false;
}


// =====================================================
// SPEAK NUMBER OF STEPS
// =====================================================

void speakSteps(int steps) {

  if (steps >= 1 && steps <= 5) {

    myDFPlayer.play(steps);

    // Small delay only to avoid immediately replacing audio
    delay(700);
  }
}


// =====================================================
// DETERMINE SAFEST DIRECTION
// =====================================================

void suggestDirection(
    long leftDistance,
    long rightDistance) {


  bool leftClear =
      (leftDistance == -1 ||
       leftDistance > OBSTACLE_THRESHOLD);


  bool rightClear =
      (rightDistance == -1 ||
       rightDistance > OBSTACLE_THRESHOLD);


  // LEFT SIDE CLEARER
  if (leftClear &&
      (!rightClear ||
       leftDistance > rightDistance)) {

    myDFPlayer.play(AUDIO_MOVE_LEFT);

    Serial.println("Guidance: MOVE LEFT");
  }


  // RIGHT SIDE CLEARER
  else if (rightClear) {

    myDFPlayer.play(AUDIO_MOVE_RIGHT);

    Serial.println("Guidance: MOVE RIGHT");
  }


  // BOTH SIDES BLOCKED
  else {

    myDFPlayer.play(AUDIO_STOP);

    Serial.println("Guidance: STOP");
  }
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  // ===================================================
  // READ ALL SENSORS
  // Sensors are staggered to reduce ultrasonic cross-talk
  // ===================================================

  long distanceFront =
      getDistance(TRIG_FRONT, ECHO_FRONT);

  delay(SENSOR_GAP);


  long distanceLeft =
      getDistance(TRIG_LEFT, ECHO_LEFT);

  delay(SENSOR_GAP);


  long distanceRight =
      getDistance(TRIG_RIGHT, ECHO_RIGHT);

  delay(SENSOR_GAP);


  long distanceBack =
      getDistance(TRIG_BACK, ECHO_BACK);

  delay(SENSOR_GAP);


  long distanceDown =
      getDistance(TRIG_DOWN, ECHO_DOWN);


  // ===================================================
  // DEBUG OUTPUT
  // ===================================================

  Serial.print("Front: ");
  Serial.print(distanceFront);

  Serial.print(" cm | Left: ");
  Serial.print(distanceLeft);

  Serial.print(" cm | Right: ");
  Serial.print(distanceRight);

  Serial.print(" cm | Back: ");
  Serial.print(distanceBack);

  Serial.print(" cm | Down: ");
  Serial.print(distanceDown);

  Serial.println(" cm");


  // ===================================================
  // RESET OUTPUTS
  // ===================================================

  digitalWrite(VIBRATION_MOTOR_FRONT, LOW);
  digitalWrite(VIBRATION_MOTOR_BACK, LOW);
  digitalWrite(VIBRATION_MOTOR_LEFT, LOW);
  digitalWrite(VIBRATION_MOTOR_RIGHT, LOW);

  digitalWrite(BUZZER, LOW);


  // ===================================================
  // POTHOLE / DROP DETECTION
  // ===================================================

  // Temporary fixed threshold.
  // Later this can be replaced with adaptive ground
  // calibration + IMU angle compensation.

  if (distanceDown != -1 &&
      distanceDown > 50) {

    digitalWrite(BUZZER, HIGH);

    Serial.println("WARNING: Possible pothole/drop");


    if (canSpeak()) {

      myDFPlayer.play(AUDIO_POTHOLE);
    }
  }


  // ===================================================
  // FRONT OBSTACLE
  // ===================================================

  if (distanceFront != -1 &&
      distanceFront < OBSTACLE_THRESHOLD) {

    digitalWrite(
        VIBRATION_MOTOR_FRONT,
        HIGH
    );


    int steps =
        getSteps(distanceFront);


    Serial.print("Front obstacle approximately ");
    Serial.print(steps);
    Serial.println(" step(s) away");


    // Very close obstacle
    if (distanceFront <= CRITICAL_DISTANCE) {

      if (canSpeak()) {

        myDFPlayer.play(AUDIO_STOP);

        Serial.println("CRITICAL: STOP");
      }
    }

    // Normal obstacle
    else {

      if (canSpeak()) {

        // Announce approximate distance in steps
        speakSteps(steps);

        // Then determine safer direction
        suggestDirection(
            distanceLeft,
            distanceRight
        );
      }
    }
  }


  // ===================================================
  // LEFT OBSTACLE
  // ===================================================

  if (distanceLeft != -1 &&
      distanceLeft < OBSTACLE_THRESHOLD) {

    digitalWrite(
        VIBRATION_MOTOR_LEFT,
        HIGH
    );

    Serial.println("Obstacle detected on LEFT");
  }


  // ===================================================
  // RIGHT OBSTACLE
  // ===================================================

  if (distanceRight != -1 &&
      distanceRight < OBSTACLE_THRESHOLD) {

    digitalWrite(
        VIBRATION_MOTOR_RIGHT,
        HIGH
    );

    Serial.println("Obstacle detected on RIGHT");
  }


  // ===================================================
  // REAR OBSTACLE
  // ===================================================

  if (distanceBack != -1 &&
      distanceBack < OBSTACLE_THRESHOLD) {

    digitalWrite(
        VIBRATION_MOTOR_BACK,
        HIGH
    );


    Serial.println("Obstacle detected BEHIND");


    if (canSpeak()) {

      myDFPlayer.play(
          AUDIO_OBSTACLE_BACK
      );
    }
  }


  // ===================================================
  // SMALL LOOP DELAY
  // ===================================================

  delay(100);
}