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
#define debugButton1 A0
#define debugButton2 A1
#define debugButton3 A2

DS3231 rtc(SDA, SCL);
LiquidCrystal LCD(6, 7, 8, 9, 10, 11);

Servo servoLid;
Servo servoFresh;

long distanceOut, durationOut;
long distanceIn, durationIn;

int buttonState1 = 0;
int buttonState2 = 0;  
int buttonState3 = 0;  

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

  pinMode(debugButton1, INPUT);
  pinMode(debugButton2, INPUT);
  pinMode(debugButton3, INPUT);

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
  Debug code for changing the day. Used to demonstrate the
  LCD screen change based on day.
  */
////////////////////////////////////////////////////////////

  if (buttonState1 == HIGH) {
    rtc.setDOW(TUESDAY);
  } else if (buttonState2 == HIGH) {
    rtc.setDOW(WEDNESDAY);
  } else if (buttonState3 == HIGH) {
    rtc.setDOW(THURSDAY);
  }
 
  digitalWrite(trigInside, 0);
  delayMicroseconds(2);
  digitalWrite(trigInside, 1);
  delayMicroseconds(10);
  digitalWrite(trigInside, 0);
  durationIn = pulseIn(echoInside, 1);
  distanceIn = durationIn / 58.2;

  if (distanceIn <= 32 && distanceIn >= 25) {
    LCD.setCursor(9, 1);
    LCD.print("25%");
  } else if (distanceIn <= 24 && distanceIn >= 17) {
    LCD.setCursor(9, 1);
    LCD.print("50%");
  } else if (distanceIn <= 16 && distanceIn >= 9) {
    LCD.setCursor(9, 1);
    LCD.print("75%");
  } else if (distanceIn <= 8 && distanceIn >= 0) {
    LCD.setCursor(8, 1);
    LCD.print("100%");
  }

  if (rtc.getDOWStr() == "Wednesday") {   

   LCD.setCursor(0,0);
   LCD.print("Tomorrow is");
   LCD.setCursor(0,1);
   LCD.print("bin day!");

  } else if (rtc.getDOWStr() == "Thursday") {

   LCD.setCursor(0,0);
   LCD.print("Today is");
   LCD.setCursor(0,1);
   LCD.print("bin day!");

 } else {

   LCD.setCursor(0,0);
   LCD.print("Feed");
   LCD.setCursor(0,1);
   LCD.print("me!");
   

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