#include <Wire.h>                                       
#include <LiquidCrystal.h>
#include "DHT.h"
#include <math.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

#define echoPin 7 
#define trigPin 8

long duration;
int distance;
const int buzzer = 9; //buzzer to arduino pin 9
const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHT dht(DHTPIN, DHTTYPE);
const int thermistor_output = A1;
const int RELAY_PIN = A5;
int gasLevel = 0;        
String quality ="";
int t;


void setup() {
  
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows:

  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);

  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

  pinMode(trigPin, OUTPUT);

  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  
}

//------------------------------------------------------------------------------------------

void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    lcd.println(F("Failed to read from DHT sensor!"));
  }

  lcd.setCursor(0, 0);  // set the cursor to column 0, line 1
  lcd.print(F("Humidity: "));
  lcd.print(h);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print(F("Temp: "));
  lcd.print(t);
  lcd.print(F(" C"));

  if(h>= 60){
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
  }
  else 
    noTone(buzzer);     // Stop sound...


  if(t < 32){
    digitalWrite(RELAY_PIN, HIGH);
  }
  else digitalWrite(RELAY_PIN, LOW);
  
  delay(2000);
  lcd.clear();

  //---------------------------------------------------------------------------------------------------
  
  gasLevel = analogRead(A0);

  Serial.print(gasLevel);

  if(gasLevel<182){
    quality = "  GOOD!";
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
  }
  else if (gasLevel >182 && gasLevel<225){
    quality = "  Poor!";
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec     
  }
  else if (gasLevel >225 && gasLevel<300){
    quality = "Very bad!";
     tone(buzzer, 1000); // Send 1KHz sound signal...
     delay(1000);        // ...for 1 sec
  }
    else if (gasLevel >300 && gasLevel<350){
    quality = "ur dead!";
     tone(buzzer, 1000); // Send 1KHz sound signal...
     delay(1000);        // ...for 1 sec
  }
    else{
    quality = " Toxic";
     tone(buzzer, 1000); // Send 1KHz sound signal...
     delay(1000);        // ...for 1 sec   
}
  lcd.setCursor (0,0);  
  lcd.print("Air Quality  ");
  lcd.setCursor (3,1);
  lcd.print(quality);
  
  delay(2000);
  lcd.clear();

  //-----------------------------------------------------------------------------------------------

  int thermistor_adc_val;
  double output_voltage, thermistor_resistance, therm_res_ln, temperature; 
  thermistor_adc_val = analogRead(thermistor_output);
  output_voltage = ( (thermistor_adc_val * 5.0) / 1023.0 );
  thermistor_resistance = ( ( 5 * ( 10.0 / output_voltage ) ) - 10 ); /* Resistance in kilo ohms */
  thermistor_resistance = thermistor_resistance * 1000 ; /* Resistance in ohms   */
  therm_res_ln = log(thermistor_resistance);

  temperature = ( 1 / ( 0.001129148 + ( 0.000234125 * therm_res_ln ) + ( 0.0000000876741 * therm_res_ln * therm_res_ln * therm_res_ln ) ) ); /* Temperature in Kelvin */
  temperature = temperature - 273.15; /* Temperature in degree Celsius */

  if(temperature > 35){
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  }
  else noTone(buzzer);     // Stop sound...

  lcd.setCursor (0,0);
  lcd.print("Incubator Temp  ");
  lcd.setCursor (3,1);
  lcd.print(temperature);
  lcd.print(" C");
  
  delay(2000);
  lcd.clear();

  //-----------------------------------------------------------------------------------------------------
  // ULTRASOUND - DIstance Measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 /2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if(distance > 18){
    tone(buzzer, 2000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
  }
  else noTone(buzzer);  


}
