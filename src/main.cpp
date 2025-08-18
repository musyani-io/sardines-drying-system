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
boolean dhtStatus, fanStatus, htrStatus, timeStatus, weightStatus;  // Control statuses
float intialWeight, currentWeight, finalWeight, temp, humidity, calibratedFactor, rawValue, weightLoss;
const float highTemp = 45;  // Threshold values
const float lowTemp = 35;   
String dhtStr, fanHtrStr, cellStr, lossStr;   // Control Strings for display
unsigned long currentMillis = 0;    // Timer variables
unsigned long lastActionDHT = 0;
unsigned long lastActionCell = 0;

// Function declaration
void displayInLcd(int col, int row, String message);
boolean afterSeconds(unsigned long seconds);

void setup() {
    dht.begin();
    Serial.begin(9600); // for debugging
    lcd.init();
    lcd.backlight();

    pinMode(HTR_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);

    displayInLcd(0, 0, "Booting...");
    displayInLcd(0, 1, "Put 1 on scale");
    delay(5000);

    scale.begin(LD_DO, LD_CLK);
    scale.set_scale(scale.read());
    lcd.clear();
    displayInLcd(0, 0, "Insert the load");
    delay(5000);
    intialWeight = scale.get_units(5);
    // Serial.println(scale.read());
    // Serial.println(intialWeight);
}

void loop() {
    temp = dht.readTemperature();
    humidity = dht.readHumidity();
    currentWeight = scale.get_units(5);
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

    // DHT and fan/heater display every 1 second
    if (currentMillis - lastActionDHT >= 1000) {
        lastActionDHT = currentMillis;
        displayInLcd(0, 0, dhtStr);
        displayInLcd(0, 1, fanHtrStr);
    }

    // Load cell display every 2 seconds
    cellStr = "Weight: " + String(currentWeight, 1) + " kg";
    weightLoss = ((intialWeight - currentWeight) / (intialWeight)) * 100;
    lossStr = "%loss: " + String(weightLoss, 2) + "%";
    if (currentMillis - lastActionCell >= 2000) {
        lastActionCell = currentMillis;
        lcd.clear();
        displayInLcd(0, 0, cellStr);
        displayInLcd(0, 1, lossStr);
        Serial.println("Load cell displayed!");
    }

    // What should the finalWeight be? (When the sardines have dried)... Its code follows.
}

void displayInLcd(int col, int row, String message) {
    lcd.setCursor(col, row);
    lcd.print(message);
}