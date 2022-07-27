int temperaturePin = A0; //signal from temperature sensor
int soilPin = A1; //signal from moisture sensor
int soilMoisture; //soil moisture
int threshold = 0; //value to trigger pump
int pump = 8; //pump output
int soilGood = 10; //LED for when soil moisture good
int soilBad = 11; //LED for whne soil moisture bad
int tempGood = 12; //LED for when temp is good
int tempBad = 13; // LED for when temp is bad
float voltage, degreesC, degreesF; //temperature values



void setup()
{
  Serial.begin(9600);
  pinMode(soilPin, INPUT);
  pinMode(pump, OUTPUT);
  pinMode(tempGood, OUTPUT);
  pinMode(tempBad, OUTPUT);
  pinMode(soilGood, OUTPUT);
  pinMode(soilBad, OUTPUT);
  
}


void loop()
{
  //calculations
  voltage = getVoltage(temperaturePin);

  degreesC = (voltage - 0.5) * 100.0;

  degreesF = degreesC * (9.0/5.0) + 32.0;
  soilMoisture = analogRead(soilPin); //capture soil moisture
  soilMoisture = map(soilMoisture, 550, 0, 0, 100); //maps to % value

  Serial.println("voltage: " + String(voltage) + " deg C: " + String(degreesC) + " deg F: " + String(degreesF));
  Serial.println("Moisture: " + String(soilMoisture) + "%");
  
  if (degreesF > 90) {
    updateLED(tempGood, tempBad);
  }
  if (degreesF < 90) {
    updateLED(tempBad, tempGood);
  }
  
  if (soilMoisture > threshold) {
    digitalWrite(pump, LOW); //pump off
    updateLED(soilGood, soilBad);
  }
  else {
    digitalWrite(pump, HIGH); //pump on
    updateLED(soilBad, soilGood);
    Serial.println("Pump on"); 
    delay(3000); //pump runs for 1sec
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
