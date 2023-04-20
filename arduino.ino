#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>



#define DISPLAY_PIN1 21
#define DISPLAY_PIN2 22
#define DHT_PIN 19
#define DHTTYPE DHT11
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHTTYPE);
 
void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
  dht.begin();
}
 
void loop() {
  float temp = dht.readTemperature();
  float hum =  dht.readHumidity(); 

  
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Temperatur: ");
  lcd.print(temp); 
  lcd.setCursor(1,1);
  lcd.print("Feuchtigkeit: ");
  lcd.print(hum);
  
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
 
  delay(5000);          
}