/*Functions for RF24 receiver. includes library "RF24" made for the module.
 * Transmitter has it's own code differing from the receivers
*/

#include <SPI.h>
#include "printf.h"
#include "RF24.h"

RF24 radio(48, 49); // using pin 48 for the CE pin, and pin 49 for the CSN pin

byte address[][6] = {"1Node", "2Node"}; // Addresses to be used for the pair
float data[3]; //Data variable for speed, rotation (gyro) and direction (controller input) 

void startRadioSetup(){ //Setups radio
  if(!radio.begin()) Serial.println("RADIO NOT WORK");

  radio.setPALevel(RF24_PA_LOW); //PA low for maximum power savings, range is atleast 15m
  radio.setChannel(124); //Channel for communication
  
  radio.openWritingPipe(address[0]);  //Opens writing pipe on specified address
  radio.openReadingPipe(1, address[1]); //Opens reading pipe on specified address
  
  radio.startListening(); // put radio in RX mode  
}

void startRadioRead(){ //Function for reading data from pipe if avaivable
    if(radio.available()){
      while(radio.available()){
        radio.read(&data, sizeof(data));
      }
      Serial.print("Speed - "); //Prints the data for display 
      Serial.println(data[0]);
      Serial.print("Gyro - ");
      Serial.println(data[1]);
      Serial.print("Accel - ");
      Serial.println(data[2]);
      Serial.print("\n");
   }
}
