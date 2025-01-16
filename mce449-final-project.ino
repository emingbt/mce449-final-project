const int stepsPerRevolution = 200;  // Full-step stepper motor
const int discSlots = 5;             // Fixed number of slots

int stepsToMove[discSlots];  // Global array for steps to move for each slot

const int dir1Pin = 2;
const int dir2Pin = 3;
const int step1Pin = 4;
const int step2Pin = 5;

class Motor {
public:
  int dirPin;
  int stepPin;
  int ms1Pin;
  int encoderPin;

  Motor(int x, int y, int z, int m) {
    dirPin = x;
    stepPin = y;
    ms1Pin = z;
    encoderPin = m;
  }
};

Motor motor1(dir1Pin, step1Pin, ms1_1, encoder1Pin);
Motor motor2(dir2Pin, step2Pin, ms1_2, encoder2Pin);
const int encoder1Pin = 6;
const int encoder2Pin = 7;

void setup() {
  pinMode(dir1Pin, OUTPUT);
  pinMode(dir2Pin, OUTPUT);
  pinMode(step1Pin, OUTPUT);
  pinMode(step2Pin, OUTPUT);
  pinMode(encoder1Pin, INPUT);
  pinMode(encoder2Pin, INPUT);

  // Declare the directions
  digitalWrite(dir1Pin, HIGH);
  digitalWrite(dir2Pin, HIGH);

  // Initialize Serial communication
  Serial.begin(9600);

  // Precompute steps for the fixed number of slots
  calculateStepsToMove();
}

void loop() {
    for (int i = 0; i < discSlots; i++) {
      moveMotorBySteps(motor1.stepPin, stepsToMove[i]);
      moveMotorBySteps(motor2.stepPin, stepsToMove[i]);
  }
}

void calibrate() {
  bool nonAlignedDisc = digitalRead(encoder1Pin) == 1 ? 1 : 2;
  int stepPin = nonAlignedDisc == 1 ? step1Pin : step2Pin;

  // Align the non aligned disc
  while (digitalRead(nonAlignedDisc == 1 ? encoder1Pin : encoder2Pin) == HIGH) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }

  int totalSlotSteps = 0;

  while (digitalRead(nonAlignedDisc == 1 ? encoder1Pin : encoder2Pin) == LOW) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);

    totalSlotSteps++;
  }

  digitalWrite(nonAlignedDisc == 1 ? dir1Pin : dir2Pin, LOW);

  for (int i = 0; i < (totalSlotSteps / 2); i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }

  digitalWrite(nonAlignedDisc == 1 ? dir1Pin : dir2Pin, HIGH);

void moveMotorBySteps(int stepPin, int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(250);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(250);
  }
}

void calculateStepsToMove() {
  int baseSteps = stepsPerRevolution / discSlots;  // Base steps for each slot
  int remainder = stepsPerRevolution % discSlots;  // Extra steps to distribute

  for (int i = 0; i < discSlots; i++) {
    stepsToMove[i] = baseSteps;  // Start with base steps
  }

  // Distribute the remainder evenly across slots
  for (int i = 0; i < remainder; i++) {
    stepsToMove[(i * discSlots / remainder) % discSlots]++;
  }
}

void moveMotorBySteps(int motor, int steps) {
  int stepPin = motor == 0 ? step1Pin : step2Pin;

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
}
