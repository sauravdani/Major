#define LED_PIN 13  // Built-in LED on most Arduino boards

void setup() {
    pinMode(LED_PIN, OUTPUT);  // Set pin as output
}

void loop() {
    digitalWrite(LED_PIN, HIGH);  // Turn LED ON
    delay(1000);                  // Wait 1 second
    digitalWrite(LED_PIN, LOW);   // Turn LED OFF
    delay(1000);                  // Wait 1 second
}
