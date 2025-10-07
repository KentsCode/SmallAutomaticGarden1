/* Kent's self watering garden project

  The arduino will look at each moisture sensor and decide to run a water pump for time=waterPumpRunTime
  After both moisture sensors have been checked the system will pause to allow any water to settle
  To protect the system from overflowing the pot due to a moisture sensor out of place a counter will keep
  track of how many times in a row the pump has run. If the pump has run for "pumpShutOffCount'X'" number of times
  it will lock out that pump and turn on a red LED. There is a separate LED for each pump and it only locks out the 
  pump that exceeded the run count. This is to prevent the system from flooding a planter if the sensor gets pulled out or stops working.
  There is a level switch that detects if there is water in the tank. If it is below a certain level the display turns off the blue LED and 
  turns on a yellow one.
  */

//Adjust only these 4 settings, No hard coded values in rest of program
const int pumpShutOffCount = 8; //how many times it runs before shutoff
const int waterPumpRunTime = 15000; // pump run time in mS
const int settleTimeAfterWatering = 30000; //settle time after all 4 sensors have been checked in mS
const int moistureCutOffValue = 475; //value of capacative moisture sensor to not water
//No adjustable settings below here

int pump1 = 2;
int sensor1 = A0;
int moistureSensorValue1 = 0; 
int pumpRunCount1 = 0;
bool pumpLockedOut1 = false;

int pump2 = 3;
int sensor2 = A1;
int moistureSensorValue2 = 0;
int pumpRunCount2 = 0;
bool pumpLockedOut2 = false;

int pumpLockoutLED1 = 8;
int pumpLockoutLED2 = 9;
int waterLevelLowLED = 10;
int waterLevelNormalLED = 11;
int waterLevelLowSensor = 13;
bool lowWaterLevelLockout = false;


void setup() {
  Serial.begin(9600); //setting baud rate of usb port

  // pin declarations
  pinMode(sensor1, INPUT); //input from moisture sensor 1
  pinMode(sensor2, INPUT); //input from moisture sensor 2

  pinMode(pump1, OUTPUT); //output to pump relay 1
  pinMode(pump2, OUTPUT); //output to pump relay 2

  //setting all pumps to low to start
  digitalWrite(pump1, LOW); //turning off the pump
  digitalWrite(pump2, LOW); //turning off the pump


  pinMode(waterLevelLowSensor, INPUT); //Low water level sensor
  pinMode(waterLevelLowLED, OUTPUT); //output to low water level lockout LED
  pinMode(waterLevelNormalLED, OUTPUT);
  pinMode(pumpLockoutLED1, OUTPUT); //output to pump lockout LED
  pinMode(pumpLockoutLED2, OUTPUT); //output to pump lockout LED
}


void loop() {
  Serial.println("Program Start");
  //check water level, set lowWaterLevelLockout value, Turn water level LEDs on and off
  if (digitalRead(waterLevelLowSensor) == LOW) {
    lowWaterLevelLockout = true;
    digitalWrite(waterLevelLowLED, HIGH);
    digitalWrite(waterLevelNormalLED, LOW);
    Serial.println("Low Water Level Lockout!!!!");
  } else {
    lowWaterLevelLockout = false;
    digitalWrite(waterLevelNormalLED, HIGH);
    digitalWrite(waterLevelLowLED, LOW);
    Serial.println("Water Level Good");
  }
  
  // Loop1 code
  moistureSensorValue1 = analogRead(sensor1);
  Serial.print("MoistureLevel 1: ");
  Serial.println(moistureSensorValue1);

  if (moistureSensorValue1 > moistureCutOffValue && pumpLockedOut1 == false && lowWaterLevelLockout == false && pumpRunCount1 < pumpShutOffCount) {
    digitalWrite(pump1, HIGH); //turning on the pump
    delay(waterPumpRunTime); //running pump for set time
    digitalWrite(pump1, LOW); //turning off the pump

    pumpRunCount1 = pumpRunCount1 +1;
    Serial.print("Pump Run Count 1: ");
    Serial.println(pumpRunCount1);
  } else if (moistureSensorValue1 < moistureCutOffValue) {
    pumpRunCount1 = 0;
    Serial.print("Moisture sensor Wet! Pump Run Count Reset, Count at: ");
    Serial.println(pumpRunCount1);
  } else {
    Serial.println("No action this loop");
  }

  if(pumpRunCount1 >= pumpShutOffCount) {
    Serial.print("Sensor Displaced? Pump Run Count now at: ");
    Serial.println(pumpRunCount1);
    digitalWrite(pumpLockoutLED1, HIGH);
    Serial.println(digitalRead(pumpLockoutLED1));
  } else {
    digitalWrite(pumpLockoutLED1, LOW);
  }
  

 // Loop2 code
  moistureSensorValue2 = analogRead(sensor2);
  Serial.print("MoistureLevel 2: ");
  Serial.println(moistureSensorValue2);

  if (moistureSensorValue2 > moistureCutOffValue && pumpLockedOut2 == false && lowWaterLevelLockout == false && pumpRunCount2 < pumpShutOffCount) {
    digitalWrite(pump2, HIGH); //turning on the pump
    delay(waterPumpRunTime); //running pump for set time
    digitalWrite(pump2, LOW); //turning off the pump

    pumpRunCount2 = pumpRunCount2 +1;
    Serial.print("Pump Run Count 2: ");
    Serial.println(pumpRunCount2);
  } else if (moistureSensorValue2 < moistureCutOffValue) {
    pumpRunCount2 = 0;
    Serial.print("Moisture sensor Wet! Pump Run Count Reset, Count at: ");
    Serial.println(pumpRunCount2);
  } else {
    Serial.println("No action this loop");
  }

  if(pumpRunCount2 >= pumpShutOffCount) {
    Serial.print("Sensor Displaced? Pump Run Count now at: ");
    Serial.println(pumpRunCount2);
    digitalWrite(pumpLockoutLED2, HIGH);
    Serial.println(digitalRead(pumpLockoutLED2));
  } else {
    digitalWrite(pumpLockoutLED2, LOW);
  }
  
  delay(settleTimeAfterWatering); //built in system delay
}
