#include <Arduino.h>
#include <DHT.h>
#include <HX711.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DHT_PIN 7
#define DHT_TYPE DHT11
#define FAN_PIN 6
#define HTR_PIN 11
#define LD_CLK 12
#define LD_DO 13
#define DOWN_BTN 4
#define SET_BTN 2
#define UP_BTN 3

DHT dht(DHT_PIN, DHT_TYPE);
HX711 scale;
LiquidCrystal_I2C lcd(0x27, 16, 2);

boolean dhtStatus, fanStatus, htrStatus, timeStatus, weightStatus, dryStatus;
float intialWeight, currentWeight, temp, humidity, calibratedFactor, rawValue, weightLoss;
float highTemp = 45.0;
float lowTemp = 35.0;
String dhtStr, fanHtrStr, cellStr, lossStr;

unsigned long currentMillis = 0;
unsigned long lastActionDHT = 0;
unsigned long lastActionCell = 0;
bool settingsMode = false;
bool tempSelected = false; // Changed: track if a temperature has been selected
bool selectingMaxTemp = true;
unsigned long lastButtonPress = 0;
const unsigned long DEBOUNCE_DELAY = 200;

void displayInLcd(int col, int row, String message);
void handleButtons();
void displaySettings();

void setup()
{
    dht.begin();
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();

    pinMode(HTR_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);
    pinMode(SET_BTN, INPUT_PULLUP);
    pinMode(UP_BTN, INPUT_PULLUP);
    pinMode(DOWN_BTN, INPUT_PULLUP);

    displayInLcd(0, 0, "Booting...");
    displayInLcd(0, 1, "Put 1 on scale");
    delay(5000);

    scale.begin(LD_DO, LD_CLK);
    scale.set_scale(scale.read());

    lcd.clear();
    displayInLcd(0, 0, "Insert the load");
    delay(5000);

    intialWeight = scale.get_units(5);
}

void loop()
{
    handleButtons();

    if (settingsMode)
    {
        displaySettings();
        return;
    }

    while (!dryStatus)
    {
        handleButtons();

        if (settingsMode)
        {
            break;
        }

        temp = dht.readTemperature();
        humidity = dht.readHumidity();
        currentWeight = scale.get_units(5);
        currentMillis = millis();

        if (isnan(temp) || isnan(humidity))
        {
            Serial.println("DHT Sensor error!");
            displayInLcd(0, 0, "DHT error!");
            dhtStatus = false;
        }
        else
        {
            dhtStatus = true;
            dhtStr = "T: " + String(temp, 1) + "C H: " + String(humidity, 1) + "%";
        }

        if (temp > highTemp)
        {
            digitalWrite(FAN_PIN, HIGH);
            digitalWrite(HTR_PIN, LOW);
            fanHtrStr = "FAN: ON HTR: OFF";
        }
        else if (temp < lowTemp)
        {
            digitalWrite(FAN_PIN, LOW);
            digitalWrite(HTR_PIN, HIGH);
            fanHtrStr = "FAN: OFF HTR: ON";
        }
        else
        {
            digitalWrite(FAN_PIN, LOW);
            digitalWrite(HTR_PIN, LOW);
            fanHtrStr = "FAN:OFF HTR: OFF";
        }
        Serial.println(highTemp);

        if (currentMillis - lastActionDHT >= 1000)
        {
            lastActionDHT = currentMillis;
            displayInLcd(0, 0, dhtStr);
            displayInLcd(0, 1, fanHtrStr);
        }

        cellStr = "Weight: " + String(currentWeight, 1) + " kg";
        weightLoss = ((intialWeight - currentWeight) / (intialWeight)) * 100;
        lossStr = "%loss: " + String(weightLoss, 2) + "%";

        if (currentMillis - lastActionCell >= 2000)
        {
            lastActionCell = currentMillis;
            lcd.clear();
            displayInLcd(0, 0, cellStr);
            displayInLcd(0, 1, lossStr);
        }

        if (weightLoss > 70)
        {
            dryStatus = true;
            lcd.clear();
        }
    }

    if (!settingsMode)
    {
        displayInLcd(0, 0, "Dried product!");
    }
}

void handleButtons()
{
    if (millis() - lastButtonPress < DEBOUNCE_DELAY)
    {
        return;
    }

    if (digitalRead(SET_BTN) == LOW)
    {
        lastButtonPress = millis();
        settingsMode = !settingsMode;
        tempSelected = false; // Changed: reset selection when entering settings
        lcd.clear();
        if (!settingsMode)
        {
            displayInLcd(0, 0, "Settings saved!");
            delay(1000);
            lcd.clear();
        }
    }

    if (settingsMode)
    {
        // Changed: First check if no temperature is selected yet
        if (!tempSelected)
        {
            if (digitalRead(UP_BTN) == LOW)
            {
                lastButtonPress = millis();
                selectingMaxTemp = true;
                tempSelected = true;
            }
            if (digitalRead(DOWN_BTN) == LOW)
            {
                lastButtonPress = millis();
                selectingMaxTemp = false;
                tempSelected = true;
            }
        }
        else
        {
            // Changed: Only adjust temperatures after selection is made
            if (digitalRead(UP_BTN) == LOW)
            {
                lastButtonPress = millis();
                if (selectingMaxTemp)
                {
                    highTemp += 1.0;
                    if (highTemp > 80.0)
                        highTemp = 80.0;
                }
                else
                {
                    lowTemp += 1.0;
                    if (lowTemp >= highTemp)
                        lowTemp = highTemp - 1.0;
                }
            }
            if (digitalRead(DOWN_BTN) == LOW)
            {
                lastButtonPress = millis();
                if (selectingMaxTemp)
                {
                    highTemp -= 1.0;
                    if (highTemp <= lowTemp)
                        highTemp = lowTemp + 1.0;
                }
                else
                {
                    lowTemp -= 1.0;
                    if (lowTemp < 10.0)
                        lowTemp = 10.0;
                }
            }
        }
    }
}

void displaySettings()
{
    static unsigned long lastSettingsUpdate = 0;
    static bool showCursor = true;
    static unsigned long lastCursorBlink = 0;

    if (millis() - lastSettingsUpdate >= 500)
    {
        lastSettingsUpdate = millis();
        lcd.clear();

        String maxTempStr = "Max: " + String(highTemp, 1) + "C";
        String minTempStr = "Min: " + String(lowTemp, 1) + "C";
        displayInLcd(0, 0, maxTempStr);
        displayInLcd(0, 1, minTempStr);

        if (millis() - lastCursorBlink >= 250)
        {
            lastCursorBlink = millis();
            showCursor = !showCursor;
        }

        // Changed: Show different prompts based on selection state
        if (!tempSelected)
        {
            if (showCursor)
            {
                displayInLcd(0, 0, "UP=Max DN=Min");
                displayInLcd(0, 1, "Choose setting");
            }
        }
        else
        {
            if (showCursor)
            {
                if (selectingMaxTemp)
                {
                    displayInLcd(15, 0, "<");
                }
                else
                {
                    displayInLcd(15, 1, "<");
                }
            }
        }
    }
}

void displayInLcd(int col, int row, String message)
{
    lcd.setCursor(col, row);
    lcd.print(message);
}