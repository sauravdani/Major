#include <Adafruit_HMC5883_U.h>

#include <SPI.h>
#include <Wire.h>
#include <HMC5883L.h>
//#include <MFRC522.h>

#define LINESENSOR1 A2
#define LINESENSOR2 A1
#define LINESENSOR3 A3

// Motor Driver Pins

#define ENA A3
#define ENB A5
#define IN1 11
#define IN2 12
#define IN3 6
#define IN4 7

#define motorSpeed 130
#define COMMANDS_LENGTH 4

// Function prototype for pathSequence()
const char** pathSequence(int &size);

// Motor control functions
void moveStraight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed+25);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN4, LOW);
    
    delay(500);

}

void turnLeft() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN4, LOW);
    
    delay(600);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN4, LOW);
    delay(500);
    
}

void turnRight() {  // Corrected Right Turn Logic
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN4, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN3, LOW);
    
    delay(600);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN4, LOW);
    delay(500);
    
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

const char** pathSequence(int &size) {
    static const char* sequence[] = {"start", "right", "left", "straight", "right", "straight", "right", "left", "right", "straight"};
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
 
    // Get the path sequence
    int size;
    const char** path = pathSequence(size);

    // Execute movement based on path sequence
    for (int i = 0; i < size; i++) {
        Serial.print("Executing: ");
        Serial.println(path[i]);

        if ((strcmp(path[i], "straight" ) == 0) || (strcmp(path[i], "start") == 0)) {
            moveStraight();
        } 
        else if (strcmp(path[i], "left") == 0) {
            turnLeft();
        } 
        else if (strcmp(path[i], "right") == 0) {
            turnRight();
        }else if(strcmp(path[i], "start") == 0){
            Serial.println("Cold starting the car");
            continue;
        }
        else {
            Serial.println("Invalid command");
        }
        delay(500);
    }

    stopMotors(); // Stop after completing the path
}

void loop() {
}
