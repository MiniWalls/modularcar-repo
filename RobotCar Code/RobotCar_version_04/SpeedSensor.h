////////////////LM393 nopeus-sensori///////////////////
//Käytetään sensorin digitaalista "D0"-pinniä datan lukemiseen
//Tehnyt: Jarno Liedes
const int inputPin7 = 7; //Nopeussensori ->Arduino digital pin 7, josta luetaan sensorin D0 -pinni
unsigned long count, reset, nextTime; //Nopeussensorin muuttujat
float rpm, kmph; //Nopeussensorin kierrokset/minuutti
void SpeedSensorDraw();
void SpeedSensor()//Lasketaan kiekon antama arvo
{
  unsigned int pin7 = digitalRead(inputPin7);
  
  if(pin7==HIGH && reset==0) //Jos sensori saa kosketuksen kiekkoon "reset=0" -tilassa
  {
    reset = 1; //Merkataan HIGH -tila luetuksi
    count++; //Lisätään kierrosten määrä
  }
  else if(pin7==LOW)  //Jos sensori on LOW-tilassa niin nollataan "reset"
  {
    reset = 0;
  }
  
  unsigned long arduinoTimer = millis();
  if (nextTime < arduinoTimer)
  {
    rpm = ((float)count/6.0)*60.0; //(count/"rattaan pykälät")*60
    kmph = rpm * 6.5 * 0.001885;
    //SpeedSensorDraw();
    count = 0;
    nextTime = arduinoTimer+1000; //arduinon oma ajastin + 1 sekunti - delayt
  }
}
void SpeedSensorDraw() //Tulostetaan nopeus-sensorin arvot
{
  Serial.print("RPM = "); //Montako kierrosta minuutissa
  Serial.print(rpm,0);
  Serial.print("\t");
  Serial.print("KMPH = "); //Montako kierrosta minuutissa
  Serial.println(kmph,2);
}
