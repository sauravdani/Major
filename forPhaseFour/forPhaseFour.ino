#include <Adafruit_HMC5883_U.h>
#include <SPI.h>
#include <Wire.h>
#include <HMC5883L.h>

#define LINESENSOR1 2  // Leftmost
#define LINESENSOR2 A3
#define LINESENSOR3 A2  // Center
#define LINESENSOR4 A1
#define LINESENSOR5 3  // Rightmost

// Motor Driver Pins
#define ENA 5
#define ENB A5
#define IN1 11
#define IN2 12
#define IN3 6
#define IN4 7

#define motorSpeed 130

volatile bool junctionDetected = false;  // Flag set in ISR
volatile int pathIndex = 0;
int pathSize = 0;
const char** path;
unsigned long lastInterruptTime = 0;  // Debounce timestamp

// Function to retrieve the path sequence
const char** pathSequence(int &size) {
    static const char* sequence[] = { "right", "left", "straight", "right", "straight", "right", "left", "right", "straight"};
    size = sizeof(sequence) / sizeof(sequence[0]);
    return sequence;
}

// ISR to detect junctions
void detectJunction() {
    unsigned long currentTime = millis();
    
    // Debounce: Ignore interrupts occurring too quickly (200ms)
    if (currentTime - lastInterruptTime > 200) {
        junctionDetected = true;
        lastInterruptTime = currentTime;
    }
}

// Motor control functions
void moveStraight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN4, LOW);
}

void turnLeft() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN4, LOW);
    delay(600);
    moveStraight();
}

void turnRight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN4, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN3, LOW);
    delay(600);
    moveStraight();
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Motor setup
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Line sensor setup
    pinMode(LINESENSOR1, INPUT);
    pinMode(LINESENSOR5, INPUT);

    // Attach interrupts
    attachInterrupt(digitalPinToInterrupt(LINESENSOR1), detectJunction, LOW);
    attachInterrupt(digitalPinToInterrupt(LINESENSOR5), detectJunction, LOW);

    // Get path sequence
    path = pathSequence(pathSize);
}

void loop() {
    if (pathIndex >= pathSize) {
        stopMotors();
        Serial.println("Path completed!");
        while (1); // Stop execution
    }

    // Wait until a junction is detected
    Serial.println("Waiting for junction...");
    while (!junctionDetected);  // Blocking wait

    // Execute the current command
    Serial.print("Junction detected! Moving to next command: ");
    Serial.println(path[pathIndex]);

    if (strcmp(path[pathIndex], "straight") == 0) {
        moveStraight();
    } 
    else if (strcmp(path[pathIndex], "left") == 0) {
        turnLeft();
    } 
    else if (strcmp(path[pathIndex], "right") == 0) {
        turnRight();
    }

    // Move to next command
    pathIndex++;
    junctionDetected = false;  // Reset flag
    delay(500);  // Allow sensor time to stabilize
}
