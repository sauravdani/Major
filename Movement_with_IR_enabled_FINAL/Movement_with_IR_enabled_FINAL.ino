#include <SPI.h>
#include <Wire.h>

// IR Sensor Definitions
#define LINESENSOR1 A4  // Outer Right (Junction Detection)
#define LINESENSOR2 A3  // Inner Right (Course Correction)
#define LINESENSOR3 A2  // Center (Main Line Following)
#define LINESENSOR4 A1  // Inner Left (Course Correction)
#define LINESENSOR5 A0  // Outer Left (Junction Detection)

// Motor Driver Pins
#define ENA 3
#define ENB A5
#define IN1 9
#define IN2 10
#define IN3 7
#define IN4 6

#define motorSpeed 130
#define correctionSpeed 100  // Speed for slight course correction
#define turnDelay 400  // Adjust for precise turning

// Function prototype
const char** pathSequence(int &size);

// Global variables
int pathIndex = 0;  // Current position in path sequence
int pathSize = 0;
const char** path;

// Motor control functions
void moveStraight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(500);
}

void turnLeft() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(turnDelay);
}

void turnRight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(turnDelay);
}

void slightLeft() {
    analogWrite(ENA, correctionSpeed);
    analogWrite(ENB, motorSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void slightRight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, correctionSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

// Path sequence
const char** pathSequence(int &size) {
    static const char* sequence[] = { "start", "right", "left", "straight", "straight", "right", "left", "right", "straight", "straight", "left", "straight", "right", "straight"};
    size = sizeof(sequence) / sizeof(sequence[0]);
    return sequence;
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Set motor pins as output
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Set IR sensors as input
    pinMode(LINESENSOR1, INPUT);
    pinMode(LINESENSOR2, INPUT);
    pinMode(LINESENSOR3, INPUT);
    pinMode(LINESENSOR4, INPUT);
    pinMode(LINESENSOR5, INPUT);

    // Load path sequence
    path = pathSequence(pathSize);
}

void loop() {
    int sensor1 = digitalRead(LINESENSOR1);  // Outer Right (Junction)
    int sensor2 = digitalRead(LINESENSOR2);  // Inner Right (Correction)
    int sensor3 = digitalRead(LINESENSOR3);  // Center (Main Line Following)
    int sensor4 = digitalRead(LINESENSOR4);  // Inner Left (Correction)
    int sensor5 = digitalRead(LINESENSOR5);  // Outer Left (Junction)

    // 🚀 **Line Following Logic**
    if (sensor3 == HIGH) {  
        moveStraight();  // Move straight if center detects white
    } 
    else if (sensor4 == HIGH) {  
        slightRight();  // Adjust right if inner left detects white
    } 
    else if (sensor2 == HIGH) {  
        slightLeft();  // Adjust left if inner right detects white
    } 
    else {  
        stopMotors();  // Stop if lost
        Serial.println("Lost the line! Stopping...");
        return;
    }

    // 🔄 **Junction Detection and Path Execution**
    if (sensor1 == HIGH || sensor5 == HIGH) {  
        if (pathIndex < pathSize) {
            Serial.print("Executing: ");
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
            pathIndex++;
        } 
        else {  
            stopMotors();
            Serial.println("Path completed");
        }
    }
}
