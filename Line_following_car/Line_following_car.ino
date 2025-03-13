#include <SPI.h>
#include <Wire.h>
#include <HMC5883L.h>
//#include <MFRC522.h>

#define LINESENSOR1 A2
#define LINESENSOR2 A1
#define LINESENSOR3 A3

#define ENA 3
#define ENB 5
#define IN1 9
#define IN2 10
#define IN3 7
#define IN4 8


#define motorSpeed 110

#define COMMANDS_LENGTH 4

// Function to move forward
void moveForward() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);

    digitalWrite(IN1, HIGH);  // Now controls Right Motor
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);  // Now controls Left Motor
    digitalWrite(IN4, HIGH);
}
// Function to turn left
void turnLeft() {
    analogWrite(ENA, motorSpeed / 2); // Reduce speed for smooth turning
    analogWrite(ENB, motorSpeed + 20);

    digitalWrite(IN1, HIGH);  // Right Motor Forward
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);   // Left Motor Reverse
    digitalWrite(IN4, HIGH);
}
// Function to turn right
void turnRight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, (motorSpeed / 2) + 20); // Reduce speed for smooth turning

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}
// Function to stop motors
void stopMotors() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}


void setup() {

  SPI.begin();
  pinMode(LINESENSOR1, INPUT);
  pinMode(LINESENSOR2, INPUT);
  pinMode(LINESENSOR3, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  moveForward();
  delay(2000);
  stopMotors();
  delay(500);
  turnLeft();
  delay(2000);
  stopMotors();
  delay(500);
  turnRight();
  delay(2000);
  stopMotors();
}

void loop() {

  // if (digitalRead(LINESENSOR1) == LOW && digitalRead(LINESENSOR2) == HIGH && digitalRead(LINESENSOR3) == LOW) {
  //   turnLeft();
  // } else if (digitalRead(LINESENSOR1) == LOW && digitalRead(LINESENSOR2) == HIGH && digitalRead(LINESENSOR3) == HIGH) {
  //   moveForward();
   
  // } else if (digitalRead(LINESENSOR1) == LOW && digitalRead(LINESENSOR2) == LOW && digitalRead(LINESENSOR3) == HIGH) {
  //   turnRight();
  // }
  
}





