#include <LiquidCrystal.h>
#include <DHT.h>

#define DHT_PIN 3
#define DHT_TYPE DHT11
#define MQ_DIGITAL 5
#define MQ_ANALOG A0

LiquidCrystal lcd(2,4,8,9,10,11);
DHT dht(DHT_PIN, DHT_TYPE);

byte water_char[] = {
  B00000, B00100, B01110, B11111, B11111, B11111, B01110, B00000
};
byte gas_warning[] = {
  B10010, B01010, B10010, B01000, B10010, B01000, B11111, B11111
};

void setup() {
  // Initialize temp sensor
  dht.begin();

  // Initialize gas sensor
  pinMode(MQ_DIGITAL, INPUT);

  // Initialize LCD display
  lcd.begin(16,2);
  lcd.createChar(0, water_char);
  lcd.createChar(1, gas_warning);
  lcd.setCursor(0, 0);
  lcd.print("Sensor station");
  lcd.setCursor(0, 1);
  lcd.print("Initialization..");
  delay(30000);
  lcd.clear();  
}

void loop() {
  // Get readings of the sensors
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  uint16_t gas_value = analogRead(MQ_ANALOG);
  bool gas_detected = digitalRead(MQ_DIGITAL);

  // Display error message if temp sensor is not connected
  if (isnan(temperature) || isnan(humidity)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No data...");
  } 

  // Display sensor readings
  else {
    lcd.clear();
      
    // Temperature readings
    lcd.setCursor(0, 0);
    String temp_reading = String(temperature);
    temp_reading.remove(temp_reading.length() - 1);
    lcd.print(temp_reading);
    lcd.print((char)223);
    lcd.print("C");

    // Gas pre-warning
    uint16_t translated_readings = map(gas_value, 0, 1023, 0, 100);
    if (gas_value > 80) {
      lcd.print(" ");
      lcd.write(byte(1));
      lcd.print(map(gas_value, 80, 1023, 0, 100));
      lcd.print("%");
    }
  
    // Humidity readings
    lcd.setCursor(12, 0);
    lcd.write(byte(0));
    lcd.print(round(dht.readHumidity()));
    lcd.print("%");
          
    // Gas sensor readings
    lcd.setCursor(0, 1);
    lcd.print(gas_value);
    lcd.print("ppm");

    // Check gas value
    if (gas_value <= 80) {
      lcd.print(" Good");
    } else if (gas_value > 80 && gas_value <= 120) {
      lcd.print(" Detected");
    } else if (gas_value > 120) {
      lcd.print(" Danger!");
    }
    
  }

  delay(5500);
}
