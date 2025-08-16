#include <Arduino.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DHT_PIN 7
#define DHT_TYPE DHT11
#define FAN_PIN 6
#define HTR_PIN 11
#define LD_DO 13
#define LD_CLK 12

DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Global variables
boolean dhtStatus = false;
boolean fanStatus = false;
boolean htrStatus = false;
boolean timeStatus = false;
const float highTemp = 45;
const float lowTemp = 35;
String dhtStr, fanHtrStr;
unsigned long currentMillis = 0;
unsigned long lastAction = 0;
const unsigned long lcdInterval = 1500;
const unsigned long clearInterval = 3000;

// Function declaration
void displayInLcd(int col, int row, String message);

void setup() {
    dht.begin();
    Serial.begin(9600); // for debugging
    lcd.init();
    lcd.backlight();

    pinMode(HTR_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);

    displayInLcd(0, 0, "Booting...");
    delay(2000);
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
        dhtStr = "T: " + String(temp, 1) + "°C H: " + String(humidity, 1) + "%";
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

    if (currentMillis - lastAction >= lcdInterval) {
        lastAction = currentMillis;
        displayInLcd(0, 0, dhtStr);
        displayInLcd(0, 1, fanHtrStr);
    }
}

void displayInLcd(int col, int row, String message) {
    lcd.setCursor(col, row);
    lcd.print(message);
}

/*
    Write a timer code to replace the many variable declared above.
*/