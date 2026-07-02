#include <OneWire.h> 
#include <DallasTemperature.h> 
#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 

#define ONE_WIRE_BUS 6 // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS); // Create a OneWire object
DallasTemperature sensors(&oneWire); // Create a DallasTemperature object

#define SCREEN_WIDTH 128 // OLED display width
#define SCREEN_HEIGHT 64 // OLED display size
#define SCREEN_ADDRESS 0x3C //OLED I2C address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Create display object
const int relayPin = 3; // Relay pin
const int tempwPin = 5;

void setup() {
    Serial.begin(9600); // Start serial communication
    sensors.begin(); // Start the DS18B20 sensor
    pinMode(relayPin, OUTPUT);
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed")); // Error handling
        for(;;);
    }
    display.clearDisplay(); 
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println(F("ADJ LINEAR PSU")); // Display title
    display.display(); 
    delay(1500); 
}

void loop(){
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);
    display.clearDisplay();

    if(tempC == DEVICE_DISCONNECTED_C) {
        display.setTextSize(1);
        display.setCursor(0,0);
        display.println(F("Error with sensor")); // Error handling
        display.println(F("Check wiring")); // Error handling
        display.display();
    }
    else {
        display.setTextSize(1);
        display.setCursor(0,0);
        display.println(F("LINEAR ADJ PSU")); // Title
        display.drawFastHLine(0, 10, SCREEN_WIDTH, SSD1306_WHITE);

        display.setTextSize(2);
        display.setCursor(0, 20);
        display.print(tempC);
        display.print(" C");
        display.setCursor(0, 40);
        display.setTextSize(1);
        display.print("Operation: ");
        if(tempC <= 50) {
            display.print("Regular");
            digitalWrite(relayPin, HIGH);
        }
        else if(tempC > 50 && tempC <= 70) {
            display.print("Caution");
            digitalWrite(relayPin, HIGH);
            digitalWrite(tempwPin, HIGH);
            delay(1000);
            digitalWrite(tempwPin, LOW);
            delay(1000);
        }
        else {
            display.print("Overheat");
            digitalWrite(relayPin, LOW);
            digitalWrite(tempwPin, HIGH);
        }

    }

    display.display();
    delay(2000);
}