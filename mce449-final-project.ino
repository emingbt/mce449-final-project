const int stepsPerRevolution = 200;  // Full-step stepper motor
const int discSlots = 5;             // Fixed number of slots

int stepsToMove[discSlots];  // Global array for steps to move for each slot

const int dir1Pin = 2;
const int dir2Pin = 3;
const int step1Pin = 4;
const int step2Pin = 5;

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
  digitalWrite(dir2Pin, LOW);

  // Initialize Serial communication
  Serial.begin(9600);

  // Precompute steps for the fixed number of slots
  calculateStepsToMove();
}

void loop() {
  for (int i = 0; i < discSlots; i++) {
    moveMotorBySteps(0, stepsToMove[i]);
    moveMotorBySteps(1, stepsToMove[i]);
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
