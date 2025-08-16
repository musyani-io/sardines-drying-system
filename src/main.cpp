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
const float highTemp = 45;
const float lowTemp = 30;
unsigned long currentMillis = 0;

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
    lcd.clear();
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temp) || isnan(humidity)) {
        Serial.println("DHT Sensor error!");
        dhtStatus = false;
    }
    else {
        dhtStatus = true;
        String dhtStr = "T: " + String(temp, 1) + "°C H: " + String(humidity, 1) + "%";
        Serial.println(dhtStr);
    }

    if (temp > highTemp) {
        digitalWrite(FAN_PIN, HIGH);
        digitalWrite(HTR_PIN, LOW);
        Serial.println("Fan ON, Heater OFF");
    }
    else if (temp < lowTemp) {
        digitalWrite(FAN_PIN, LOW);
        digitalWrite(HTR_PIN, HIGH);
        Serial.println("Fan OFF, Heater ON");
    }

}

void displayInLcd(int col, int row, String message) {
    lcd.setCursor(col, row);
    lcd.print(message);
}

/*
    Write a function for a timer in order to display the variables after some interval.
*/