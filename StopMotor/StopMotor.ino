#include <Adafruit_HMC5883_U.h>
#include <SPI.h>
#include <Wire.h>
#include <HMC5883L.h>

#define LINESENSOR1 A4  // Leftmost
#define LINESENSOR2 A3
#define LINESENSOR3 A2  // Center
#define LINESENSOR4 A1
#define LINESENSOR5 A0  // Rightmost

// Motor Driver Pins
#define ENA 5
#define ENB A5
#define IN1 11
#define IN2 12
#define IN3 6
#define IN4 7

#define motorSpeed 130

volatile bool junctionDetected = false;
volatile int pathIndex = 0;
int pathSize = 0;
const char** path;

// Function to retrieve the path sequence
const char** pathSequence(int &size) {
    static const char* sequence[] = { "right", "left", "straight", "right", "straight", "right", "left", "right", "straight"};
    size = sizeof(sequence) / sizeof(sequence[0]);
    return sequence;
}

volatile unsigned long lastInterruptTime = 0;  // Stores last interrupt time to debounce



// Motor control functions
void moveStraight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 25);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN4, LOW);

    if (junctionDetected) {
        Serial.print("Junction detected! Moving to next command: ");
        Serial.println(path[pathIndex + 1]);
        
        pathIndex++;
        junctionDetected = false; // Reset flag
    }
}

void turnLeft() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN4, LOW);
    delay(600);
}

void turnRight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN4, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN3, LOW);
    delay(600);

    
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

    // // Attach interrupts with improved accuracy
    // attachInterrupt(digitalPinToInterrupt(LINESENSOR1), detectJunction, RISING);
    // attachInterrupt(digitalPinToInterrupt(LINESENSOR5), detectJunction, RISING);

    // // Get path sequence
    // path = pathSequence(pathSize);

    stopMotors();
}

void loop() {
    // if (pathIndex >= pathSize) {
    //     stopMotors();
    //     Serial.println("Path completed!");
    //     while (1); // Stop execution
    // }

    // if (strcmp(path[pathIndex], "straight") == 0) {
    //     moveStraight();
    // } 
    // else if (strcmp(path[pathIndex], "left") == 0) {
    //     turnLeft();
    // } 
    // else if (strcmp(path[pathIndex], "right") == 0) {
    //     turnRight();
    // }

    // // Verify the sensor state before moving to the next command
    // if (junctionDetected) {
    //     Serial.print("Junction detected! Moving to next command: ");
    //     Serial.println(path[pathIndex + 1]);
        
    //     pathIndex++;
    //     junctionDetected = false; // Reset flag
    // }
}