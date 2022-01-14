// C++ code
//

#include <SoftwareSerial.h>

char choice; // variable store the user's choice when using Bluetooth app

long timeDuration; // variable store time duration sound wave transmitted from the sensor to the object
int cm; // variable store the distance between sensor and the object

SoftwareSerial bluetoothDevice(7, 8); // the bluetooth module connected in pin 7 and 8
 
int triggerPin = 9; // the trigger pin of the ultrasonic distance sensor connected to pin 9
int echoPin = 10; // the echo pin of the ultrasonic distance sensor connected to pin 10

int lightBulb =  11; // the relay connected in pin 11

void setup()
{

  Serial.begin(9600); // Sets Serial Communication with the 9600 of baudrate speed

  bluetoothDevice.begin(9600); // Sets the 9600 of baudrate speed for AT-command Mode

  pinMode(lightBulb, OUTPUT); // sets the relay at pin 11 as output

  digitalWrite(lightBulb, LOW); // sets the relay off

  pinMode(triggerPin, OUTPUT); // sets the trigger at pin 9 as output
  pinMode(echoPin, INPUT); // sets the echo at pin 10 as input
}

void loop()
{
  // Make sure the triggerPin is clear 
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);

  // Generate the sound wave
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  timeDuration = pulseIn(echoPin, HIGH); // read the travel time value 
  
  cm = timeDuration * 0.034 / 2; // calculate the distance in centimeters

  // Check if there is a data send to the Bluetooth module
  if (bluetoothDevice.available()){ 
    choice = bluetoothDevice.read(); // read the data 
  }

  // If the distance lower than 5 or the data is equal to 4, the light will be turned on. Otherwise, the light will be turned off
  if (cm < 5 || choice == '4'){
      digitalWrite(lightBulb, HIGH); // sets the light on    
  }

  else{
    digitalWrite(lightBulb, LOW); // sets the light off
  }

  if (choice == '3'){ // If the data is equal to 3
    // if the distance is equal or larger than 5, the light bulb will be turned off. Otherwise, it will be turned on
    if (cm >= 5)
    {
      digitalWrite(lightBulb, LOW); // sets the light off
    }
    else
    {
      digitalWrite(lightBulb, HIGH); // sets the light on
    }
  }
  delay(50);
}
