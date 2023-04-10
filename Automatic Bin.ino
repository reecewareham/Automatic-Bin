#include <Servo.h>
#include <Wire.h>
#define trigPin 8
#define echoPin 9
#define servoPin 4
#define ledPin 5
Servo servo;

long distance, duration;

////////////////////////////////////////////////////////////

  /*
  Setup - Creates a connection on serial 9600. Attaches the
  servo to pin 4. Attaches the ultrasonic sensor to pins 2
  and 3 and lastly attaches the led to pin 5.
  */

////////////////////////////////////////////////////////////

void setup()
{

  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  servo.attach(servoPin);
  pinMode(ledPin, OUTPUT);
  servo.write(90);

}

void loop()
{
////////////////////////////////////////////////////////////
  /*
  Works out the distance between an object and the ultrasonic sensor.
  */
////////////////////////////////////////////////////////////

  digitalWrite(trigPin, 0);
  delayMicroseconds(2);
  digitalWrite(trigPin, 1);
  delayMicroseconds(10);
  digitalWrite(trigPin, 0);
  duration = pulseIn(echoPin, 1);
  distance = duration / 58.2;
  Serial.println(distance);

////////////////////////////////////////////////////////////
  /*
  Checks to see if the distance is less than 45 cm. If it is less, then open
  bin for 5 seconds and turn on red LED. After the 5 seconds are up, close the
  bin.
  */
////////////////////////////////////////////////////////////

  if (distance < 45) {

    digitalWrite(ledPin, HIGH);
    
    for (int i = 70; i >= 0; i--) { 
      servo.write(i);
      delay(14.2857142857);
    }

    delay(5000);
    
    for (int i = 0; i <= 70; i++) { 
      servo.write(i);
      delay(57.1428571429);
    }

    digitalWrite(ledPin, LOW);

  } else {
    
    servo.write(70);
    delay(1000);
    digitalWrite(ledPin, LOW);

  }
}

/*
  To Do

  1) Notify the user when the bin needs emptying.
    - Detects if bin is nearly full
    - Turns on yellow LED to notify user.
    - Requires DS3231 (Clock module)
  2) Notify the user when it is bin day.
    - Checks what day it is, if it matches bin day notify user.
    - Requires ESP8266 (Wifi Module)
*/
  