/* Kent's self watering garden project

  The arduino will look at each moisture sensor and decide to run a water pump for time=waterPumpRunTime
  After all 4 moisture sensors have been checked the system will pause to allow any water to settle
  To protect the system from overflowing the pot due to a moisture sensor out of place a counter will keep
    track of how many times in a row the pump has run. If the pump has run for "pumpShutOffCount'X'" number of times
    it will lock out that pump and turn on a red LED. Single LED for all 4 pumps but only locks out 1 pump at a time.
  A sensor will monitor the water source level. If it is below a certain level it will turn on an LED and prevent
  the pumps from running to prevent damage to the pumps
*/

//Adjust only these 4 settings, No hard coded values in program
const int pumpShutOffCount = 7; //how many times it runs before shutoff
const int waterPumpRunTime = 15000; // pump run time in mS
const int settleTimeAfterWatering = 30000; //settle time after all 4 sensors have been checked in mS
const int moistureCutOffValue = 550; //value of capacative moisture sensor to not water

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

int pump3 = 4;
int sensor3 = A2;
int moistureSensorValue3 = 0;
int pumpRunCount3 = 0;
bool pumpLockedOut3 = false;

int pump4 = 5;
int sensor4 = A3;
int moistureSensorValue4 = 0;
int pumpRunCount4 = 0;
bool pumpLockedOut4 = false;

int pumpLockoutLED = 7;

int waterLevelLowLED = 8;
int waterLevelLowSensor = 9;
bool lowWaterLevelLockout = false;

void setup() {
  Serial.begin(9600); //setting baud rate of usb port

  // pin declarations
  pinMode(sensor1, INPUT); //input from moisture sensor 1
  pinMode(sensor2, INPUT); //input from moisture sensor 2
  pinMode(sensor3, INPUT); //input from moisture sensor 3
  pinMode(sensor4, INPUT); //input from moisture sensor 4

  pinMode(pump1, OUTPUT); //output to pump relay 1
  pinMode(pump2, OUTPUT); //output to pump relay 2
  pinMode(pump3, OUTPUT); //output to pump relay 3
  pinMode(pump4, OUTPUT); //output to pump relay 4

  //setting all pumps to low to start
  digitalWrite(pump1, LOW); //turning off the pump
  digitalWrite(pump2, LOW); //turning off the pump
  digitalWrite(pump3, LOW); //turning off the pump
  digitalWrite(pump4, LOW); //turning off the pump

  pinMode(waterLevelLowSensor, INPUT); //Low water level sensor
  pinMode(waterLevelLowLED, OUTPUT); //output to low water level lockout LED

  pinMode(pumpLockoutLED, OUTPUT); //output to pump lockout LED
}

void loop() {
  Serial.println("Program Start");
  /*check water level and set lowWaterLevelLockout value
  NO Level sensor installed yet.
  if (digitalRead(waterLevelLowSensor) == LOW) {
    lowWaterLevelLockout = true;
    
    Serial.println("Low Water Level Lockout!!!!");
  } else {
    lowWaterLevelLockout = false;
    //digitalWrite()
    Serial.println("Water Level Good");
  }
  */
  // Loop1 code
  moistureSensorValue1 = analogRead(sensor1);
  Serial.print("MoistureLevel 1: ");
  Serial.println(moistureSensorValue1);

  if (moistureSensorValue1 > moistureCutOffValue && pumpLockedOut1 == false /*&& lowWaterLevelLockout == false */ && pumpRunCount1 < pumpShutOffCount) {
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
    digitalWrite(pumpLockoutLED, HIGH);
    Serial.println(digitalRead(pumpLockoutLED));
  }
  

  delay(settleTimeAfterWatering); //built in system delay



 // Loop1 code
  moistureSensorValue2 = analogRead(sensor2);
  Serial.print("MoistureLevel 2: ");
  Serial.println(moistureSensorValue2);

  if (moistureSensorValue2 > moistureCutOffValue && pumpLockedOut2 == false /*&& lowWaterLevelLockout == false */ && pumpRunCount2 < pumpShutOffCount) {
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
    digitalWrite(pumpLockoutLED, HIGH);
    Serial.println(digitalRead(pumpLockoutLED));
  }
  

  delay(settleTimeAfterWatering); //built in system delay
}
