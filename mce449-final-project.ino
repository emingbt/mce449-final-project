const int stepsPerRevolution = 3200;  // Full-step stepper motor
const int discSlots = 5;              // Fixed number of slots

int stepsToMove[discSlots];  // Global array for steps to move for each slot

const int dir1Pin = 2;
const int dir2Pin = 3;
const int step1Pin = 4;
const int step2Pin = 5;

void setup() {
  pinMode(dir1Pin, OUTPUT);
  pinMode(dir2Pin, OUTPUT);
  pinMode(step1Pin, OUTPUT);
  pinMode(step2Pin, OUTPUT);

  // Initialize Serial communication
  Serial.begin(9600);

  // Precompute steps for the fixed number of slots
  calculateStepsToMove();
}

void loop() {
  // Print stepsToMove every 1 second
  Serial.print("[");
  for (int i = 0; i < discSlots; i++) {
    Serial.print(stepsToMove[i]);
    if (i < discSlots - 1) {
      Serial.print(", ");  // Add a comma and space between elements
    }
  }
  Serial.println("]");  // End the line after printing the array
  delay(1000);          // Wait for 1 second
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
