#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>
#include <DS3231.h>

#define servoBinLid 12
#define servoFreshner 13
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
  Setup - Attaches outside ultrasonic sensor to pin 2 and 3.
  Attaches inside sensor to pin 4 and 5. Attaches servos to pins
  12 and 13. Writes the servos to their default positions.
  Begins the real time clock and also the LCD screen.
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

////////////////////////////////////////////////////////////
  /*
  Checks which day the real time clock is displaying. If the day
  is thursday, change LCD screen to "Today is Bin Day!", if the day
  is wednesday, change LCD screen to "Tomorrow is Bin Day!" and if
  its any other day, change to "Feed Me!".
  */
////////////////////////////////////////////////////////////

  if (rtc.getDOWStr() == "Wednesday") {   

   LCD.setCursor(0,0);
   LCD.print("Tomorrow is");
   LCD.setCursor(2,1);
   LCD.print("Bin Day!");

  } else if (rtc.getDOWStr() == "Thursday") {

   LCD.setCursor(2,0);
   LCD.print("Today is");
   LCD.setCursor(2,1);
   LCD.print("Bin Day!");

 } else {

   LCD.setCursor(6,0);
   LCD.print("Feed");
   LCD.setCursor(6,1);
   LCD.print("Me!");
   
 }

 ////////////////////////////////////////////////////////////
  /*
  Works out the distance between an object and the ultrasonic sensor
  on the inside of the bin.
  */
////////////////////////////////////////////////////////////
 

  digitalWrite(trigInside, 0);
  delayMicroseconds(2);
  digitalWrite(trigInside, 1);
  delayMicroseconds(10);
  digitalWrite(trigInside, 0);
  durationIn = pulseIn(echoInside, 1);
  distanceIn = durationIn / 58.2;

  ////////////////////////////////////////////////////////////
  /*
  Checks the distance between the rubbish and the ultrasonic
  sensor and change the percentage displayed on the LCD screen
  to the correct value.
  */
////////////////////////////////////////////////////////////

  if (distanceIn <= 30 && distanceIn >= 25) {
    LCD.setCursor(13, 1);
    LCD.print("25%");
  } else if (distanceIn <= 24 && distanceIn >= 17) {
    LCD.setCursor(13, 1);
    LCD.print("50%");
  } else if (distanceIn <= 16 && distanceIn >= 9) {
    LCD.setCursor(13, 1);
    LCD.print("75%");
  } else if (distanceIn <= 8 && distanceIn >= 0) {
    LCD.setCursor(12, 1);
    LCD.print("100%");
  }


////////////////////////////////////////////////////////////
  /*
  Works out the distance between an object and the ultrasonic sensor
  on the outside of the bin.
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
  bin for 5 seconds. After the 5 seconds are up, close the spray
  the air freshner for 1 second and slowly close the bin lid.
  Once finished, display "Thank you!" on the LCD screen.
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
    LCD.setCursor(3,0);
    LCD.print("Thank You!");
    delay(5000);
    LCD.clear();

  } else {
    
    servoLid.write(180);
    delay(1000);
    LCD.clear();

  }
  
}
