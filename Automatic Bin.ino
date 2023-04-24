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

long distanceOut, durationOut;
long distanceIn, durationIn;

////////////////////////////////////////////////////////////

  /*
  Setup - Creates a connection on serial 9600. Attaches the
  servo to pin 4. Attaches the ultrasonic sensor to pins 2
  and 3 and lastly attaches the led to pin 5.
  */

////////////////////////////////////////////////////////////

void setup()
{
  
  pinMode(echoOutside, INPUT);
  pinMode(trigOutside, OUTPUT);

  pinMode(echoInside, INPUT);
  pinMode(trigInside, OUTPUT);

  servoLid.attach(servoBinLid);
  servoFresh.attach(servoFreshner);
  
  servoLid.write(180);
  servoFresh.write(0);

  rtc.begin();
  LCD.begin(16, 2);

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

 } else {

   LCD.setCursor(4,0);
   LCD.print("Hello!");
   

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
  durationOut = pulseIn(echoOutside, 1);
  distanceOut = durationOut / 58.2;

////////////////////////////////////////////////////////////
  /*
  Checks to see if the distance is less than 45 cm. If it is less, then open
  bin for 5 seconds and turn on red LED. After the 5 seconds are up, close the
  bin.
  */
////////////////////////////////////////////////////////////

  if (distanceOut < 45) {


    for (int i = 180; i >= 110; i--) { 
      servoLid.write(i);
      delay(14.2857);
    }

    delay(5000);

    servoFresh.write(150);
    delay(1000);
    servoFresh.write(0);
    delay(1000);
    
    for (int i = 110; i <= 180; i++) { 
      servoLid.write(i);
      delay(57.1428);
    }

    LCD.clear();
    LCD.print("Thank you!");
    delay(3000);
    LCD.clear();


  } else {
    
    servoLid.write(180);
    delay(1000);
    LCD.clear();

  }
  
}
  