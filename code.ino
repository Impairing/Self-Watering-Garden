int temperaturePin = A0; //signal from temperature sensor
int soilPin = A1; //signal from moisture sensor
int lightPin = A2; //signal from photoresistor
int soilMoisture; //soil moisture
int threshold = 20; //value to trigger pump
int pump = 6; //pump output
int lightLevel, high = 0, low = 1023;
int soilGood = 8; //LED for when soil moisture good
int soilBad = 9; //LED for whne soil moisture bad
int tempGood = 10; //LED for when temp is good
int tempBad = 11; // LED for when temp is bad
int lightBad = 12; //LED for when light is bad
int lightGood = 13; //LED for when light is good
float voltage, degreesC, degreesF; //temperature values



void setup()
{
  Serial.begin(9600);
  pinMode(soilPin, INPUT);
  pinMode(temperaturePin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(pump, OUTPUT);
  pinMode(tempGood, OUTPUT);
  pinMode(tempBad, OUTPUT);
  pinMode(soilGood, OUTPUT);
  pinMode(soilBad, OUTPUT);
  pinMode(lightGood, OUTPUT);
  pinMode(lightBad, OUTPUT);
  
}


void loop()
{
  //calculations
  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;
  
  soilMoisture = analogRead(soilPin); //capture soil moisture
  soilMoisture = map(soilMoisture, 1017, 0, 0, 100); //maps to % value

  lightLevel = analogRead(lightPin); //capture light level
  tune();

  Serial.println("voltage: " + String(voltage) + " deg C: " + String(degreesC) + " deg F: " + String(degreesF));
  Serial.println("Moisture: " + String(soilMoisture) + "%");
  Serial.println("Light level: " + String(lightLevel));
  
  if (degreesF > 65) {
    updateLED(tempGood, tempBad);
  }
  else {
    updateLED(tempBad, tempGood);
  }

  if (lightLevel > 128) {
    updateLED(lightGood, lightBad);
  }
  else {
    updateLED(lightBad, lightGood);
  }
  
  if (soilMoisture > threshold) {
    digitalWrite(pump, LOW); //pump off
    updateLED(soilGood, soilBad);
  }
  else {
    digitalWrite(pump, HIGH); //pump on
    updateLED(soilBad, soilGood);
    Serial.println("Pump on"); 
    delay(1000); //pump runs for 1 sec
    digitalWrite(pump, LOW); //pump off
    Serial.println("Pump off");
  }
  delay(3000);

}

void updateLED(char ledOn, char ledOff)
{
  digitalWrite(ledOn, HIGH);
  digitalWrite(ledOff, LOW);
}

float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882814);
}

void tune()
{
  if (lightLevel < low)
  {
    low = lightLevel;
  }
  if (lightLevel > high)
  {
    high = lightLevel;
  }
  lightLevel = map(lightLevel, low+30, high-30, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);
}
