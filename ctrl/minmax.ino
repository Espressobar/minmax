#include <Arduino.h>
#include "control_messages.h"

/**
 *  TODO:
 *    CTRL:
 *      RECIPES: 
 *      -  Control messages must send along a recipe
 *      SETTINGS:
 *      - Get information from outside world. Initialize STM32 with it in a big message.
 *      - 
 *      
 *    UI:
 *    
 */
 
String inputString = "";         // a string to hold incoming data
boolean cmdReceived = false;  // whether the string is complete
char cmd;
const int ledPin =  13;      // the number of the LED pin

//const int buttonPin = 2;     // the number of the pushbutton pin
//int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
//  pinMode(buttonPin, INPUT);
      
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  //send test
  delay(2000);

  //initialize - test
  
  if(Serial.available())
  {
    Serial.write(INITIALIZE); 
  }
}

void loop() {

//Simulate boot up - send commands to STM32.

/**************************
 *  Button
 ***************************/
/*
   // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
*/

/****************************
 *  Serial
 ****************************/
  
  // print the string when a newline arrives:
  if (cmdReceived) {

//print raw
    Serial.println(inputString);
    
    //Evaluate what we've received
    switch(cmd)
    {
      case ControlMessages::INITIALIZE:
      {
        Serial.println("Received INITIALIZE");
        break;
      }
      case ControlMessages::EXECUTE_RECIPE:
      {
        Serial.println("Received START_MIXING");
        break;
      }
      default:
        break;
    }
   
    // clear the string:
    inputString = "";
    cmdReceived = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); //todo: send more complex telegrams 
    
    // add it to the inputString:
    inputString += inChar;
    
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:

   //Check for end-of-telegram here instead. this is just the only thing in the telegrams right now
   if(inChar == ControlMessages::INITIALIZE || inChar == ControlMessages::EXECUTE_RECIPE )
   {
    cmdReceived = true;
    cmd = inChar;
   }
   
//    if (inChar == '\n') { //Listen to an ENDOFMSG event instead, if multiple pieces of data are sent in the same message
//      cmdReceived = true;
//    }

  }
}


