// Blynk configuration
#define BLYNK_TEMPLATE_ID "TMPL2UJ7DYPVE"
#define BLYNK_TEMPLATE_NAME "MacetaInteligente"
#define BLYNK_AUTH_TOKEN "Ejn234LXbkKWeOlV4_ZeXcQr7cyH_QtN"

// Include necessary libraries
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Define screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define pins for sensors and relay
#define DHTPIN 19
#define RELAY_PIN 18
const int soilMoisturePin = 35;

// Initialize DHT sensor
DHT dht(DHTPIN, DHT22);

// WiFi credentials
char auth[]=BLYNK_AUTH_TOKEN;
char ssid[] = "CURWIFI";
char pass[] = "uniremington1915";

void setup() {
  Serial.begin(115200);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED display not found"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Initialize DHT22 and relay
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
   Serial.println(F("antes conectarse"));
  // Connect to Blynk
 
  Blynk.begin(auth,ssid,pass);
     Serial.println(F("despues de conectarse"));

}

void loop() {
   Serial.println(F("antes del run blynk"));
  Blynk.run();
    Serial.println(F("run blynk"));

  // Read soil moisture sensor
  int soilMoistureValue = analogRead(soilMoisturePin);
  int humidityPercentage = map(soilMoistureValue, 4095, 0, 0, 100);

  // Read DHT22 values
  float h = dht.readHumidity();
  float t = dht.readTemperature();
    Serial.println(F("antes del display"));

  // Display information on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Humedad suelo:");
  display.setCursor(0, 10);
  display.print(humidityPercentage);
  display.print("%");

  if (!isnan(h) && !isnan(t)) {
    display.setCursor(0, 25);
    display.print("Temp: ");
    display.print(t);
    display.print(" C");
    display.setCursor(0, 35);
    display.print("Hum: ");
    display.print(h);
    display.print(" %");
  } else {
    display.setCursor(0, 25);
    display.print("Error en DHT22");
  }

  // Control relay based on soil moisture
  if (humidityPercentage < 30) {
    digitalWrite(RELAY_PIN, HIGH);
    display.setCursor(0, 50);
    display.print("Riego: ON");
    Blynk.virtualWrite(V5, 255);
    Blynk.virtualWrite(V4,HIGH);
    Serial.println("Relé encendido (Riego activado)");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    display.setCursor(0, 50);
    display.print("Riego: OFF");
    Blynk.virtualWrite(V4, LOW);
    Serial.println("Relé apagado (Riego desactivado)");
  }

  // Send data to Blynk
  Blynk.virtualWrite(V1, humidityPercentage);
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V0, t);

  display.display();

  // Print to serial monitor for debugging
  Serial.print("Humedad del suelo: ");
  Serial.print(humidityPercentage);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println("C");
  Serial.print("Humedad ambiente: ");
  Serial.print(h);
  Serial.println("%");

  delay(1000);
}