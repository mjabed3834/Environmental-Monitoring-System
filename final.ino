#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

// Pin Definitions
#define LED_PIN 21
#define LED_PIN2 22
#define AM2302_PIN 34
#define MQ7_ANALOG_PIN 35
#define GP2Y10_SIGNAL_PIN 27

// Define DHT parameters
#define DHT_TYPE DHT22
#define DHT_PIN AM2302_PIN

// Initialize sensor object
DHT dhtSensor(DHT_PIN, DHT_TYPE);

// Initialize LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust 0x27 if your I2C address is different

// Wi-Fi credentials
const char* ssid = "iPhone";
const char* password = "riyad12345";

// ThingSpeak API settings
const char* apiKey = "69VXGORTMYJ9M831"; // Replace with your ThingSpeak Write API key
const char* server = "api.thingspeak.com"; // ThingSpeak server
unsigned long myChannelNumber = 2553463; // Replace with your ThingSpeak channel number

WiFiClient wifiClient;

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Initialize the DHT sensor
  dhtSensor.begin();

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("LCD Initialized!");

  // Set LED pins as output
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);

  // Connect to Wi-Fi
  connectWiFi();
}

void loop() {
  // Read data from MQ-7 gas sensor
  int mq7Value = analogRead(MQ7_ANALOG_PIN);
  Serial.print("MQ-7 Gas Sensor Value: ");
  Serial.println(mq7Value);

  // Determine warning level for gas sensor value
  String gasWarning = getWarningLevel(mq7Value);

  // Display gas sensor value and warning on LCD
  displayDataOnLCD("Gas Sensor", mq7Value, gasWarning);

  delay(5000); // Delay for 5 seconds

  // Read data from AM2302 temperature and humidity sensor
  float temp = dhtSensor.readTemperature();
  float hum = dhtSensor.readHumidity();
  Serial.print("Temperature (°C): ");
  Serial.println(temp);
  Serial.print("Humidity (%): ");
  Serial.println(hum);

  // Determine warning level for temperature and humidity
  String tempHumWarning = getTempHumWarning(temp, hum);

  // Display temperature and humidity values and warning on LCD
  displayTempHumidityOnLCD(temp, hum, tempHumWarning);

  delay(5000); // Delay for 5 seconds

  // Read data from Sharp GP2Y10 dust sensor
  int gp2y10Value = analogRead(GP2Y10_SIGNAL_PIN);
  Serial.print("Sharp GP2Y10 Dust Sensor Value: ");
  Serial.println(gp2y10Value);

  // Determine warning level for dust sensor value
  String dustWarning = getWarningLevel(gp2y10Value);

  // Display dust sensor value and warning on LCD
  displayDataOnLCD("Dust Sensor", gp2y10Value, dustWarning);

  delay(5000); // Delay for 5 seconds

  // Send data to ThingSpeak
  sendDataToThingSpeak(mq7Value, temp, hum, gp2y10Value);

  delay(20000); // Delay for 20 seconds to comply with ThingSpeak rate limits
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    lcd.print(".");
  }

  Serial.println("\nWi-Fi connected successfully!");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wi-Fi Connected");
  delay(2000); // Display message for 2 seconds
}

void sendDataToThingSpeak(int mq7Value, float temp, float hum, int gp2y10Value) {
  if (WiFi.status() == WL_CONNECTED) {
    String url = "/update?api_key=" + String(apiKey) +
                 "&field1=" + String(mq7Value) + "&field2=" + String(temp) +
                 "&field3=" + String(hum) + "&field4=" + String(gp2y10Value);

    if (wifiClient.connect(server, 80)) {
      // Make a HTTP GET request:
      wifiClient.println("GET " + url + " HTTP/1.1");
      wifiClient.println("Host: " + String(server));
      wifiClient.println("Connection: close");
      wifiClient.println();

      // Wait for the response
      while (wifiClient.connected() || wifiClient.available()) {
        if (wifiClient.available()) {
          String line = wifiClient.readStringUntil('\n');
          Serial.println(line); // Print the response to Serial Monitor
        }
      }
      wifiClient.stop();
    } else {
      Serial.println("Connection to ThingSpeak failed");
    }
  } else {
    Serial.println("WiFi not connected");
  }
}

String getWarningLevel(int value) {
  if (value < 300) {
    return "Safe";
  } else if (value >= 300 && value < 600) {
    return "Moderate";
  } else {
    return "Dangerous";
  }
}

String getTempHumWarning(float temp, float hum) {
  if (temp >= 20 && temp <= 30 && hum >= 40 and hum <= 60) {
    return "Safe";
  } else {
    return "Moderate";
  }
}

void displayDataOnLCD(String sensorName, int sensorValue, String warning) {
  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0); // Set cursor to the first column of the first row
  lcd.print(sensorName + ": ");
  lcd.print(sensorValue);
  lcd.setCursor(0, 1); // Set cursor to the first column of the second row
  lcd.print("(");
  lcd.print(warning);
  lcd.print(")");
}

void displayTempHumidityOnLCD(float temp, float hum, String warning) {
  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0); // Set cursor to the first column of the first row
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C (");
  lcd.print(warning);
  lcd.print(")");
  lcd.setCursor(0, 1); // Set cursor to the first column of the second row
  lcd.print("Humidity: ");
  lcd.print(hum);
  lcd.print(" %");
}
