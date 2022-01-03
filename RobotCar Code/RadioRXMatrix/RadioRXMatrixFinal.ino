#include "RF24_RX.h"
#include "matrix2.h"
  
void setup() {
  Serial.begin(9600);
  startRadioSetup(); //Setup radio
  shifterSetup(); //Setup shifter for matrix control
 } // setup()
  
void loop() {
  startRadioRead(); //Read from pipe if data avaivable
  chooseArrow(data); //Chooses the direction of arrow, cross if not moving
  Serial.print(data[0]); //Prints km/h from radio transmitted data
  Serial.println(" km/h");
} // loop
