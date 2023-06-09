#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define DISPLAY_PIN1 21
#define DISPLAY_PIN2 22
#define DHT_PIN 19
#define DHTTYPE DHT11

const char* ssid = "dein_WLAN_SSID"; // SSID deines WLANs
const char* password = "dein_WLAN_Passwort"; // Passwort deines WLANs

const char* server = "api.thingspeak.com";
const char* apiKey = "4022R2DLG3Q2I33R";

WiFiClient client;
DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
  dht.begin();
  lcd.init(); // initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Temperatur: ");
  lcd.setCursor(1,1);
  lcd.print("Feuchtigkeit: ");
  
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
  Serial.println("Connected to WiFi");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  lcd.setCursor(12,0);
  lcd.print(temp);
  lcd.setCursor(13,1);
  lcd.print(hum);

  if (WiFi.status() == WL_CONNECTED) {
    String url = "/update?api_key=";
    url += apiKey;
    url += "&field1=";
    url += String(temp);
    url += "&field2=";
    url += String(hum);
    HTTPClient http;
    http.begin(client, server, 80, url);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      Serial.println("Data sent successfully!");
    }else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
    
  }else {
    Serial.println("WiFi not connected!");
  }
  delay(5000);
}
