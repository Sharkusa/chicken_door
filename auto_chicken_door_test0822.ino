#include <Wire.h>
#include <RTClib.h>
#include <Time.h>
#include <TimeAlarms.h>
 
RTC_DS1307 RTC;
 
// Start Time
int sHour = 13;
int sMinute = 0;
int sSecond = 0;
 
// End Time
int eHour = 20;
int eMinute = 0;
int eSecond = 0;
 
int Relay1 = 2;
int Relay2 = 13;
 
void setup() {
  // Set the relay to off immediately
  //digitalWrite(Relay1, LOW);
  //digitalWrite(Relay2, LOW);
   
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
   
  // Set the pinmode
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  
   
  // Notify if the RTC isn't running
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running");
  }
 
  // Get time from RTC
  DateTime current = RTC.now();
  DateTime compiled = DateTime(__DATE__, __TIME__);
  if (current.unixtime() < compiled.unixtime()) {
    Serial.println("RTC is older than compile time! Updating");
  //  RTC.adjust(DateTime(__DATE__, __TIME__));    //uncomment to set time
  }
    
  // Use RTC time to set the start time
  setTime(sHour, sMinute, sSecond, current.day(), current.month(), current.year());
  time_t s = now();
 
  // Use RTC time to set the end time 
  setTime(eHour, eMinute, eSecond, current.day(), current.month(), current.year());   
  time_t e = now();
   
  // Use RTC time to set the current time
  setTime(current.hour(), current.minute(), current.second(), current.day(), current.month(), current.year());  
  time_t n = now();
 
  // Test if Door should be open
  if (s <= n && n <= e) {
    Serial.println("Open Door");
    digitalWrite(Relay1, HIGH);  // Sets the door open
    digitalWrite(Relay2, LOW);
    delay(60000);              // wait 
   digitalWrite(Relay1, LOW);   // unlatch relays
   digitalWrite(Relay2, LOW);
   delay(600);
  }
// Test if Door should be closed
  else {     //changed this 0123
  Serial.println("Close Door");
  digitalWrite(Relay1, LOW);  // Sets the door closed
  digitalWrite(Relay2, HIGH);
  delay(60000);              // wait 
  digitalWrite(Relay1, LOW);   // unlatch relays
 digitalWrite(Relay2, LOW);  
 }
     
  Alarm.alarmRepeat(sHour, sMinute, sSecond, Dooropen);
  Alarm.alarmRepeat(eHour, eMinute, eSecond, Doorclose);
}
 
void loop() {
  DateTime now = RTC.now();
  setTime(now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());
   
  Serial.print("Current time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
     
  Serial.println();
  Alarm.delay(1000);
}
 
void Dooropen() {
  Serial.println("Open Door");
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, LOW);
  delay(6000);                  // wait 
   digitalWrite(Relay1, LOW);     // unlatch relays
   digitalWrite(Relay2, LOW);
}
 
void Doorclose() {
  Serial.println("Close Door");
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, HIGH);
  delay(6000);                  // wait 
   digitalWrite(Relay1, LOW);     // unlatch relays
   digitalWrite(Relay2, LOW);
}
