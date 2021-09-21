/*
   Terminal Module is like a chat box. It allows you to send and receive commands between your
   board and smartphone.
   NOTE:
   1)For Arduino Mega Connect Bluetooth on Serial3 pins.
   2)For Arduino Uno/Nano library uses SoftwareSerial,hence pin 2 and pin 3 are used
   as RX and TX pins respectively on SoftwareSerial.Hence with arduino Uno
   follow below connections for bluetooth.
   UNO  - BLUETOOTH
   2    - TX
   3    - RX
   3)For Uno/Nano keep bluetooth Baudrate below 38400.

   You can reduce the size of library compiled by enabling only those modules that you want
   to use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/terminal-module/
*/

#include <time_scheduler.h>
#include <Constants.h>
#include <TimeLib.h>
#include <Dabble.h>

#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_MODULE

time_t time;
String msg_in = "";             // serial data from serial monitor on arduino to phone
String msg_out = "";            // serial data from phone to arduino
bool dataflag = 0;
Scheduler scheme;


void setup() {
  Serial.begin(9600);       // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin(9600);       //Change this baudrate as per your bluetooth baudrate. Connect bluetooth on digital pin 2(RX) and 3(TX) for Uno/Nano and on Serial3 pins for Mega.

  setTime(12, 0, 0, 19, 9, 2021);

  pinMode(PINSOLENOID, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(PINSOLENOID, LOW);
  digitalWrite(LED_BUILTIN, LOW);

/*   scheme.reset();
  scheme.insert(now()+10, 10);
  scheme.insert(now()+30, 10);
  scheme.insert(now()+50, 10); */

  Serial.println("setup completed");
}


void loop() {
  // Serialdata from phone to arduino-monitor
  Dabble.processInput();
  while (Serial.available() != 0){
    msg_in = String(msg_in + char(Serial.read()));
    dataflag = 1;
  }
  if(dataflag == 1){
    Terminal.print(msg_in);
    Terminal.print(msg_in.length());
    msg_in = "";
    dataflag = 0;
  }

  // from arduino-monitor to phone
  if(Terminal.available()){
    while (Terminal.available() != 0){
      msg_out = String(msg_out + Terminal.read());
      dataflag = 1;
    }
    Serial.println();
    dataflag = 0;
  }
  if(scheme.check_and_execute()){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(1000);
}