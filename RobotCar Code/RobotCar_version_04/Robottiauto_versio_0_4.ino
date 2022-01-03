#include "SpeedSensor.h"
#include <Servo.h>
#include <PS4BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#endif
#include <SPI.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include "RF24_TX.h"
USB Usb;
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
PS4BT PS4(&Btd);
Servo myservo;  //luodaan servo objekti
volatile byte count10;
byte reload = 0x9C;
int val;
const int trigPin = 44;
const int echoPin = 8;
long kesto;
int etaisyys;

void setup()
{
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  cli();
  TCCR2B = 0; // Alustetaan timer2 rekisterit
  OCR2A = reload;
  TCCR2A = 1 << WGM21;
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
  TIMSK2 = (1 << OCIE2A);
  sei();
  //startRadioSetup();
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
  pinMode(33, OUTPUT);  //Moottorinohjausyksikön liitännät alkaen vasemmalta
  pinMode(32, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(3, OUTPUT);   //Moottorin PVM pinnit
  pinMode(5, OUTPUT);
  pinMode(trigPin, OUTPUT); // ultraäänisensorin pinnit
  pinMode(echoPin, INPUT);
  myservo.attach(46);  //liittää servon arduinon digipinnille 46
  
}
void ETAANPAIN() //Auto liikkuu eteenpäin
{
  digitalWrite(33, LOW);
  digitalWrite(32, HIGH);
  digitalWrite(31, LOW);
  digitalWrite(30, HIGH);
  analogWrite(3, PS4.getAnalogButton(R2));
  analogWrite(5, PS4.getAnalogButton(R2));
}
void TAAKSEPAIN() //Auto liikkuu taaksepäin
{
  digitalWrite(33, HIGH);
  digitalWrite(32, LOW);
  digitalWrite(31, HIGH);
  digitalWrite(30, LOW);
  analogWrite(3, PS4.getAnalogButton(L2));
  analogWrite(5, PS4.getAnalogButton(L2));
}
void MOOTTORIT_SEIS() // KUN MEINAA AJAA SEINÄÄN!
{
  analogWrite(3, 255);
  analogWrite(5, 255);
  digitalWrite(33, HIGH);
  digitalWrite(32, LOW);
  digitalWrite(31, HIGH);
  digitalWrite(30, LOW);
}
void loop(){
  BTControl();
  SpeedSensor();
  //startRadioTransfer();
  if (count10 == 10) // 100ms välein mitataan etäisyyttä
  {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); // lähetetään 10μs ultraäänipulssi
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    kesto = pulseIn(echoPin, HIGH); // lasketaan kuinka kauan pulssilla kestää tulla takaisin
    etaisyys = kesto * 0.034 / 2; // 0.034cm/μs kertoimena
    count10 = 0;    // nollataan aloitusehto
  }
  if (etaisyys < 70 && rpm > 150 ) { 
    MOOTTORIT_SEIS();
  }
}

void BTControl()
{
  Usb.Task();
  if (PS4.connected()){
    if (PS4.getAnalogButton(R2) > 0) //Jos ps4 -ohjaimen R2 painetaan "kaasu"
    {
      ETAANPAIN();
    }
    else if (PS4.getAnalogButton(L2) > 0) //Jos ps4 -ohjaimen L2 painetaan "pakki"
    {
      TAAKSEPAIN();
    }
    else
    {
      digitalWrite(33, LOW);
      digitalWrite(32, LOW);
      digitalWrite(31, LOW);
      digitalWrite(30, LOW);
    }
    val = PS4.getAnalogHat(LeftHatX); //lukee ohjaimen antaman arvon (luku välillä 0 ja 225)
    
    if (val > 128) //Kääntyy oikealle
    {
      val = map(val, 0, 225, 40, 130); // annetaan servon kääntyvyyteen rajat
      myservo.write(val);

    }
    else if (val < 122) //Kääntyy vasemmalle
    {
      val = map(val, 0, 225, 40, 130);  // // annetaan servon kääntyvyyteen rajat
      myservo.write(val);

    }
    else
    {
      myservo.write(101);
    }
    if (PS4.getButtonClick(PS)) {
      PS4.disconnect();


    }
  }
  else if(!PS4.connected()) // jos menettää yhteyden ohjaimeen niin pysyy paikallaan
    {
      digitalWrite(33, LOW);
      digitalWrite(32, LOW);
      digitalWrite(31, LOW);
      digitalWrite(30, LOW);
    }
}
ISR(TIMER2_COMPA_vect)  //Timer2 keskeytys 10ms välein
{
  count10++;            //Lisätään pykälä etäisyydenlaskenta toimintoon
  OCR2A = reload;
}
