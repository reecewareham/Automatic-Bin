#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>
#include <DS3231.h>

#define servoBinLid 0
#define servoFreshner 1
#define trigOutside 2
#define echoOutside 3
#define trigInside 4
#define echoInside 5

DS3231 rtc(SDA, SCL);
LiquidCrystal LCD(6, 7, 8, 9, 10, 11);

Servo servoLid;
Servo servoFresh;

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

  pinMode(echoOutside, INPUT);
  pinMode(trigOutside, OUTPUT);

  pinMode(echoInside, INPUT);
  pinMode(trigInside, OUTPUT);

  servoLid.attach(servoBinLid);
  servoFresh.attach(servoFreshner);
  
  servoLid.write(180);
  servoFresh.write(180);

  rtc.begin();
  LCD.begin(16, 2);

  Serial.println("Setup done");

}

void loop()
{

  if (rtc.getDOWStr() == "Wednesday") {   

   LCD.setCursor(2,0);
   LCD.print("Tomorrow is");
   LCD.setCursor(4,1);
   LCD.print("bin day!");

  } else if (rtc.getDOWStr() == "Thursday") {

   LCD.setCursor(4,0);
   LCD.print("Today is");
   LCD.setCursor(4,1);
   LCD.print("bin day!");

 }


////////////////////////////////////////////////////////////
  /*
  Works out the distance between an object and the ultrasonic sensor.
  */
////////////////////////////////////////////////////////////

  digitalWrite(trigOutside, 0);
  delayMicroseconds(2);
  digitalWrite(trigOutside, 1);
  delayMicroseconds(10);
  digitalWrite(trigOutside, 0);
  duration = pulseIn(echoOutside, 1);
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

    Serial.println("servo activated");

    for (int i = 180; i >= 110; i--) { 
      servoLid.write(i);
      delay(14.2857);
    }

    delay(5000);

    long distance2, duration2;

    while (distance2 < 45) {
      
      digitalWrite(trigOutside, 0);
      delayMicroseconds(2);
      digitalWrite(trigOutside, 1);
      delayMicroseconds(10);
      digitalWrite(trigOutside, 0);
      duration2 = pulseIn(echoOutside, 1);
      distance2 = duration2 / 58.2;
      Serial.println(distance2);

    }

    servoFresh.write(130);
    delay(1000);
    servoFresh.write(180);
    
    for (int i = 110; i <= 180; i++) { 
      servoLid.write(i);
      delay(57.1428);
    }

    LCD.clear();
    LCD.print("Thank you!");
    delay(3000);
    LCD.clear();


  } else {
    
    Serial.println("default");
    servo.write(180);
    delay(1000);
    lcd.clear();

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
  