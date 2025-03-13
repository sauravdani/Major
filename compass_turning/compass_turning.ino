#include <Wire.h>
#include <QMC5883LCompass.h>  // Correct library for your sensor

// Compass object
QMC5883LCompass compass;

// Motor Driver Pins
#define motorSpeed 110
#define ENA 3
#define ENB 5
#define IN1 9
#define IN2 10
#define IN3 7
#define IN4 8

// DRDY pin for compass
// #define DRDY 2 

// Calibration offset
float headingOffset = 300.0;  

// Function prototype for pathSequence()
const char** pathSequence(int &size);

// Motor control functions
void moveStraight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnLeft() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(500);
}

void turnRight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(500);
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

// Function to get calibrated heading
int getHeading() {
    int x = compass.getX();
    int y = -compass.getY();  // Adjusting for module rotation

    float heading = atan2(y, x) * 180 / PI;
    if (heading < 0) heading += 360;  // Convert to 0-360 range

    // Apply calibration offset
    heading -= headingOffset;  
    if (heading < 0) heading += 360;  

    return int(heading);
}

// Function to rotate to a specific angle
void turnToAngle(int targetAngle) {
    int currentAngle = getHeading();

    while (abs(targetAngle - currentAngle) > 5) {  // 5-degree tolerance
        if ((targetAngle - currentAngle + 360) % 360 > 180) {
            turnLeft();
        } else {
            turnRight();
        }
        delay(100);
        currentAngle = getHeading();
    }

    stopMotors();
}

// Movement functions
void turnToLeft() {
    int targetAngle = (getHeading() + 270) % 360;
    turnToAngle(targetAngle);
    moveStraight();
    delay(1000);
}

void turnToRight() {
    int targetAngle = (getHeading() + 90) % 360;
    turnToAngle(targetAngle);
    moveStraight();
    delay(1000);
}

void moveToStraight() {
    int targetAngle = 0;  // North
    turnToAngle(targetAngle);
    moveStraight();
    delay(1000);
}

void moveToBack() {
    int targetAngle = 180;  // South
    turnToAngle(targetAngle);
    moveStraight();
    delay(1000);
}

// Move robot based on a sequence of directions
void moveInSequence(const char* directions[], int size) {
    for (int i = 0; i < size; i++) {
        Serial.print("Moving: ");
        Serial.println(directions[i]);

        if (strcmp(directions[i], "left") == 0) {
            turnToLeft();
        } else if (strcmp(directions[i], "right") == 0) {
            turnToRight();
        } else if (strcmp(directions[i], "straight") == 0) {
            moveToStraight();
        } else if (strcmp(directions[i], "back") == 0) {
            moveToBack();
        }

        delay(1000);
    }
}

// Function to generate the movement sequence
const char** pathSequence(int &size) {
    static const char* sequence[] = { "right", "straight", "back"};
    size = sizeof(sequence) / sizeof(sequence[0]);
    return sequence;
}

void setup() {
    Serial.begin(9600);
    Wire.begin();

    // Initialize compass
    compass.init();
    compass.setCalibration(-3094, 1338, -4041, 2228, -1917, 1483);

    // Set motor pins as output
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    //  Serial.println("Moving Forward...");
    // turnRight();
    // delay(2000);
    // stopMotors();
    // Serial.println("Stopped");
    // pinMode(DRDY, INPUT);
}

void loop() {
    int sequenceSize;
    const char** movementSequence = pathSequence(sequenceSize);
    
    moveInSequence(movementSequence, sequenceSize);
    
    while (1);
}
