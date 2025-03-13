#include <Arduino.h>

// Define IR sensor pins
#define LINESENSOR1 A2  // Left IR sensor
#define LINESENSOR2 A1  // Center IR sensor
#define LINESENSOR3 A3  // Right IR sensor

// Motor Driver Pins
#define ENA 3
#define ENB 5
#define IN1 9
#define IN2 10
#define IN3 7
#define IN4 8

#define motorSpeed 110
#define COMMANDS_LENGTH 4

// Grid size
const int ROWS = 4;
const int COLS = 4;

// Grid with weights
int grid[ROWS][COLS] = {
    {1, 1, 4, 2},
    {3, 1, 1, 1},
    {1, 5, 1, 1},
    {1, 1, 1, 1}
};

// Movement structure
struct Direction {
    int dx, dy;
    const char* name;
};

// Possible movements (Right, Left, Down, Up)
Direction moves[] = {
    {0, 1, "right"},
    {0, -1, "left"},
    {1, 0, "straight"},
    {-1, 0, "straight"}
};

// Reverse array function
template <typename T>
void reverseArray(T arr[], int size) {
    int left = 0, right = size - 1;
    while (left < right) {
        T temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

// Dijkstra’s algorithm to find shortest path
void dijkstra(int startX, int startY, int endX, int endY, const char* directions[], int &dirSize) {
    int dist[ROWS][COLS];
    int parent[ROWS][COLS][2];
    const char* moveDirection[ROWS][COLS];

    // Initialize distances
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            dist[i][j] = 10000;
        }
    }

    dist[startX][startY] = 0;

    while (true) {
        int minDist = 10000, x = -1, y = -1;

        // Find the node with the smallest distance
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (dist[i][j] < minDist) {
                    minDist = dist[i][j];
                    x = i;
                    y = j;
                }
            }
        }

        if (x == -1 || y == -1 || (x == endX && y == endY)) break;

        // Update neighbors
        for (Direction move : moves) {
            int nx = x + move.dx, ny = y + move.dy;
            if (nx >= 0 && ny >= 0 && nx < ROWS && ny < COLS) {
                int newCost = dist[x][y] + grid[nx][ny];
                if (newCost < dist[nx][ny]) {
                    dist[nx][ny] = newCost;
                    parent[nx][ny][0] = x;
                    parent[nx][ny][1] = y;
                    moveDirection[nx][ny] = move.name;
                }
            }
        }

        dist[x][y] = 10000; // Mark as visited
    }

    // Reconstruct path
    dirSize = 0;
    int px = endX, py = endY;
    while (!(px == startX && py == startY)) {
        directions[dirSize++] = moveDirection[px][py];
        int tempX = parent[px][py][0];
        int tempY = parent[px][py][1];
        px = tempX;
        py = tempY;
    }

    // Reverse directions to get correct order
    reverseArray(directions, dirSize);
}

// Motor control functions
void moveStraight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(500);
}

void turnRight() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(500);  // Adjust turning time
}

void turnLeft() {
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed + 20);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(500);  // Adjust turning time
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

// Function to follow line and handle junctions
void followLine(const char* directions[], int size) {
    int index = 0;
    
    while (index < size) {
        int left = digitalRead(LINESENSOR1);
        int center = digitalRead(LINESENSOR2);
        int right = digitalRead(LINESENSOR3);

        if (center == HIGH && left == LOW && right == LOW) {
            moveStraight();
        } 
        else if (left == HIGH && center == LOW && right == LOW) {
            turnLeft();
        } 
        else if (right == HIGH && center == LOW && left == LOW) {
            turnRight();
        } 
        else if (left == HIGH && right == HIGH) { // Junction detected!
            Serial.println("Junction detected!");
            stopMotors();
            delay(500);

            // Get next movement command
            Serial.print("Executing: ");
            Serial.println(directions[index]);

            if (strcmp(directions[index], "right") == 0) {
                turnRight();
            } else if (strcmp(directions[index], "left") == 0) {
                turnLeft();
            } else if (strcmp(directions[index], "straight") == 0) {
                moveStraight();
            }

            delay(1000); // Move for 1 sec
            index++;  // Move to next command
        }
    }
    stopMotors();
}

// Arduino setup function
void setup() {
    // Serial.begin(9600);

    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(LINESENSOR1, INPUT);
    pinMode(LINESENSOR2, INPUT);
    pinMode(LINESENSOR3, INPUT);

    moveStraight();
    // delay(2000);
    // turnLeft();
    // delay(1000);
    // moveStraight();
    // delay(2000);
    // turnRight();
    // delay(1000);
    // moveStraight();
    // delay(1500);
    stopMotors();

    // Serial.println("Finding shortest path...");
    
    // const char* directions[COMMANDS_LENGTH];  
    // int dirSize = 0;

    // dijkstra(0, 0, 3, 3, directions, dirSize);

    // Serial.println("Path found. Starting navigation...");
    // followLine(directions, dirSize);
}

void loop() {
    // Nothing to do in the loop
}
