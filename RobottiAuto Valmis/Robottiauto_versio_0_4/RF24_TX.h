#include <SPI.h>
#include "printf.h"
#include "RF24.h"
 
RF24 radio(48, 49); // using pin 7 for the CE pin, and pin 8 for the CSN pin

byte address[][6] = {"1Node", "2Node"};
float data[3] = {5.2,400,1022};

void startRadioSetup(){
  radio.begin();

  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(124);
  
  radio.openWritingPipe(address[1]);  
  radio.openReadingPipe(1, address[0]);
  
  radio.startListening(); // put radio in RX mode 
}

void startRadioTransfer(){
  radio.stopListening();

  if(!radio.write(&data, sizeof(data))){
    //Serial.println("No aknowledgement of transmission");
  }
}
