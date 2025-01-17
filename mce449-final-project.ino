#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int stepsPerRevolution = 400;  // Half-step stepper motor
const int discSlots = 4;             // Fixed number of slots

int stepsToMove[discSlots];  // Global array for steps to move for each slot

// Motor 1 driver pins
const int dir1Pin = 4;
const int step1Pin = 5;
const int ms1_1 = 2;

// Motor 2 driver pins
const int dir2Pin = 6;
const int step2Pin = 7;
const int ms1_2 = 3;

// Optical encoder pins
const int encoder1Pin = 9;
const int encoder2Pin = 8;

const int buttonPin = 10;

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

bool isRunning = false;

int counter1 = 0;
int counter2 = 0;

// Create an LCD class
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(dir1Pin, OUTPUT);
  pinMode(dir2Pin, OUTPUT);
  pinMode(step1Pin, OUTPUT);
  pinMode(step2Pin, OUTPUT);
  pinMode(encoder1Pin, INPUT);
  pinMode(encoder2Pin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Declare the directions
  digitalWrite(dir1Pin, HIGH);
  digitalWrite(dir2Pin, HIGH);

  // Make motors half step
  digitalWrite(ms1_1, HIGH);
  digitalWrite(ms1_2, HIGH);

  // Initialize Serial communication
  Serial.begin(9600);

  // Precompute steps for the fixed number of slots
  calculateStepsToMove();

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  initializeLCD();

  calibrate();
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    isRunning = !isRunning;
    delay(1000);
  }

  if (isRunning) {
    for (int i = 0; i < discSlots; i++) {
      moveMotorBySteps(motor1.stepPin, stepsToMove[i]);
      counter1++;
      lcd.setCursor(8, 0);
      lcd.print(counter1);
      moveMotorBySteps(motor2.stepPin, stepsToMove[i]);
      counter2++;
      lcd.setCursor(8, 1);
      lcd.print(counter2);
    }
  }
}

void calibrate() {
  if (digitalRead(motor1.encoderPin) == HIGH) {
    calibrateDisc(motor1.dirPin, motor1.stepPin, motor1.encoderPin, false);
    calibrateDisc(motor2.dirPin, motor2.stepPin, motor2.encoderPin, true);
  } else if (digitalRead(motor2.encoderPin) == HIGH) {
    calibrateDisc(motor2.dirPin, motor2.stepPin, motor2.encoderPin, false);
    calibrateDisc(motor1.dirPin, motor1.stepPin, motor1.encoderPin, true);
  } else {
    calibrateDisc(motor1.dirPin, motor1.stepPin, motor1.encoderPin, true);
    calibrateDisc(motor2.dirPin, motor2.stepPin, motor2.encoderPin, true);
  }
}

void calibrateDisc(int dirPin, int stepPin, int encoderPin, bool isAligned) {
  // Calibrate the disc according to the isAligned parameter
  while (digitalRead(encoderPin) == isAligned ? LOW : HIGH) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(10000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(10000);
  }

  int totalSlotSteps = 0;

  while (digitalRead(encoderPin) == isAligned ? HIGH : LOW) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(10000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(10000);

    totalSlotSteps++;
  }

  if (isAligned) {
    totalSlotSteps = 0;

    while (digitalRead(encoderPin) == LOW) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(10000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(10000);

      totalSlotSteps++;
    }
  }

  digitalWrite(dirPin, LOW);

  for (int i = 0; i < (totalSlotSteps / 2); i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(10000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(10000);
  }

  digitalWrite(dirPin, HIGH);
}

void moveMotorBySteps(int stepPin, int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(230);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(230);
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

void initializeLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Disc 1:");
  lcd.setCursor(8, 0);
  lcd.print(counter1);
  lcd.setCursor(0, 1);
  lcd.print("Disc 2:");
  lcd.setCursor(8, 1);
  lcd.print(counter2);
}