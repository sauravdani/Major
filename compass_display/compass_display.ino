#include <Wire.h>
#include <QMC5883LCompass.h>  // Install "QMC5883LCompass" library from Arduino Library Manager

QMC5883LCompass compass;

float headingOffset = 300.0;  // Offset correction variable (adjustable)

void setup() {
    Serial.begin(115200);
    Wire.begin();
    compass.init();
    Serial.println("QMC5883L Compass Initialized!");
}

void loop() {
    compass.read();  // Read magnetometer data

    // Get X and Y values (adjusted for your sensor orientation)
    int x = compass.getX();
    int y = -compass.getY();  // Invert Y since your sensor is rotated

    // Calculate heading in degrees
    float heading = atan2(y, x) * 180 / PI;
    if (heading < 0) heading += 360;  // Convert to 0-360 range

    // Apply calibration offset
    heading -= headingOffset;  
    if (heading < 0) heading += 360;  // Ensure 0-360 range

    // Print corrected heading
    Serial.print("Calibrated Heading: ");
    Serial.print(heading);
    Serial.println("°");

    delay(100);  // Update every 100ms
}
