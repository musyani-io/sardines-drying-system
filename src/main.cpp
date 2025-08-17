#include <Arduino.h>
#include <DHT.h>
#include <HX711.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DHT_PIN 7
#define DHT_TYPE DHT11
#define FAN_PIN 6
#define HTR_PIN 11
#define LD_DO 13
#define LD_CLK 12

DHT dht(DHT_PIN, DHT_TYPE);
HX711 scale;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Global variables
boolean dhtStatus = false;  // Control statuses
boolean fanStatus = false;
boolean htrStatus = false;
boolean timeStatus = false;
boolean weightStatus = false;
float intialWeight = 0.0;
float finalWeight = 0.0;
const float highTemp = 45;  // Threshold values
const float lowTemp = 35;   
String dhtStr, fanHtrStr;   // Control Strings for display
unsigned long currentMillis = 0;    // Timer variables
unsigned long lastAction = 0;

// Function declaration
void displayInLcd(int col, int row, String message);
boolean afterSeconds(unsigned long seconds);
void calibrateWeight();

void setup() {
    dht.begin();
    Serial.begin(9600); // for debugging
    lcd.init();
    lcd.backlight();

    pinMode(HTR_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);

    displayInLcd(0, 0, "Booting...");
    delay(2000);

    scale.begin(LD_DO, LD_CLK);
    scale.set_scale();  // Set scale to default
    scale.tare();   // Reset the scale to 0

    Serial.println("Put the load on scale");
    displayInLcd(0, 0, "Put the load");
    displayInLcd(2, 1, "on the scale");
    while (!weightStatus) {
        intialWeight = scale.get_units(10);
        if (intialWeight > 1.0) {
            weightStatus = true;
        }
        Serial.println("Put the load on scale");
        delay(100);
    }
}

void loop() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    currentMillis = millis();

    if (isnan(temp) || isnan(humidity)) {   // Checks DHT integrity
        Serial.println("DHT Sensor error!");
        displayInLcd(0, 0, "DHT error!");
        dhtStatus = false;
    }
    else {
        dhtStatus = true;
        dhtStr = "T: " + String(temp, 1) + "C H: " + String(humidity, 1) + "%";
        // Serial.println(dhtStr);
    }

    if (temp > highTemp) {      // Validates the threshold
        digitalWrite(FAN_PIN, HIGH);
        digitalWrite(HTR_PIN, LOW);
        fanHtrStr = "FAN: ON HTR: OFF";
        // Serial.println("Fan ON, Heater OFF");
    }
    else if (temp < lowTemp) {
        digitalWrite(FAN_PIN, LOW);
        digitalWrite(HTR_PIN, HIGH);
        fanHtrStr = "FAN: OFF HTR: ON";
        // Serial.println("Fan OFF, Heater ON");
    }
    else {
        fanHtrStr = "FAN:OFF HTR: OFF";
    }

    if (afterSeconds(1UL)) {
        displayInLcd(0, 0, dhtStr);
        displayInLcd(0, 1, fanHtrStr);
    }
}

void displayInLcd(int col, int row, String message) {
    lcd.setCursor(col, row);
    lcd.print(message);
}

boolean afterSeconds(unsigned long seconds) {
    seconds = 1000 * seconds;
    if (currentMillis - lastAction >= seconds) {
        lastAction = currentMillis;
        return true;
    }
    else {
        return false;
    }
}

void calibrateWeight() {
    // Write a calibrating weight function.
}
