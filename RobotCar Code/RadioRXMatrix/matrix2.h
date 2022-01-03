/* Made with two hc595n components. We want to use the minimum amount of digital pins so code readability had to be sacrificed. The arrows moving left and right
 * are printed by moving the starting row of the arrow. The up and down arrows have to be manually made in to an array for printing, since using only two bitshifters with 
 * 3 arduino outputs we are unable to scans the horizontal lines for printing.
 */

enum Motion{CROSS,UP,LEFT,DOWN,RIGHT}; //Enum for our motions

bool turning = false; //Bool for if turning

int STORE = 4; //Store pin for 74HC595N
int SHIFT = 5; //Shift pin for 74HC595N
int DATA = 6;  //Data pin for 74HC595N

int pic[8] = {255,231,195,153,60,126,255,255}; //Right arrow
int pic3[8] = {255,126,60,153,195,231,255,255}; //Left arrow
int pic2[8][8] = {{48,96,192,129,129,192,96,48},{96,192,129,3,3,129,192,96},{192,129,3,6,6,3,129,192},
                 {129,3,6,12,12,6,3,129},{3,6,12,24,24,12,6,3},{6,12,24,48,48,24,12,6},
                 {12,24,48,96,96,48,24,12}, {24,48,96,192,192,96,48,24}}; //Upwards Arrow
int pic4[8][8] = {{48,24,12,6,6,12,24,48},{24,12,6,129,129,6,12,24},{12,6,129,192,192,129,6,12},
                {6,129,192,96,96,192,129,6},{129,192,96,48,48,96,192,129},{192,96,48,24,24,48,96,192},
                {96,48,24,12,12,24,48,96},{48,24,12,6,6,12,24,48}}; //Down Arrow
int pic5[8] = {129,66,36,24,24,36,66,129}; //Cross

int start = 0; //Index for the start position of last iteration through the array
Motion mtn; //Variable mtn type of Motion

void shifterSetup(){ //Setups pins for bitshifter
  pinMode(DATA, OUTPUT); //Setup pins for bitshifter
  pinMode(SHIFT, OUTPUT);
  pinMode(STORE, OUTPUT);
}

void store() { //Function of saving data into bitshifter
  digitalWrite(STORE, HIGH);
  delayMicroseconds(100);
  digitalWrite(STORE, LOW);
  delayMicroseconds(100);
}

void rightArrow(){ //Function to draw arrow pointing right
  for (int i=0; i<8; i++) {
    shiftOut(DATA, SHIFT, LSBFIRST, pic[(i+start)%8]);
    shiftOut(DATA, SHIFT, LSBFIRST, 128 >> i);
    store();
    delay(10);
  }
  start = start +1;
  if(start>= 8) start = 0;
}

void leftArrow(){ //Function to draw arrow pointing left
  for (int i=0; i<8; i++) {
    shiftOut(DATA, SHIFT, LSBFIRST, pic3[(i+start)%8]);
    shiftOut(DATA, SHIFT, LSBFIRST, 128 >> i);
    store();
    delay(10);
  }
  start = start +-1;
  if(start<= 0) start = 8;
}

void upArrow(){ //Function to draw arrow pointing upwards
  for(int j = 0; j<9; j++){
    for (int i=0; i<8; i++) {
      shiftOut(DATA, SHIFT, LSBFIRST, ~pic2[j][i]);
      shiftOut(DATA, SHIFT, LSBFIRST, 128 >> i);
      store();
      delay(3);
    }
    delay(10);
  }
}

void downArrow() { //Function to draw arrow pointing downwards
  for(int j = 0; j<8; j++){
  for (int i=0; i<8; i++) {
    shiftOut(DATA, SHIFT, LSBFIRST, ~pic4[j][i]);
    shiftOut(DATA, SHIFT, LSBFIRST, 128 >> i);
    store();
    delay(3);
  }
  delay(10);
  } 
}

void cross(){ //Function to draw cross, for when not moving
  for (int i=0; i<8; i++) {
    shiftOut(DATA, SHIFT, LSBFIRST, ~pic5[i]);
    shiftOut(DATA, SHIFT, LSBFIRST, 128 >> i);
    store();
    delay(1);
  }
}

void chooseArrow(float dvelocity[]){ //Funcion to choose what to draw based on speed and direction we get from radio module.
  int velocity = dvelocity[2]; //turn float of controllers input to int
  int turning = dvelocity[1]; //turn float gyro z axis to int
  if(turning <= -5){ //If turning more than -5 degrees to the right (sensor is upside down)
    mtn = RIGHT; //Set mtn to equal Enum motion
    turning = true;
  }
  else if(turning >= 5) //If turning more than 5 degrees to the right (sensor is upside down)
  {
    mtn = LEFT;   
    turning = true;
  }
    else if(velocity < 490 && velocity > 10 && !turning)
  {
    mtn = DOWN;
  }
    else if(velocity > 510 && velocity < 1024 && !turning)
  {
    mtn = UP;   
  }
  else{
    mtn = CROSS;
  }
  switch(mtn){ //Switch case to choose which shape we draw. Arduino accepts only int and float in switch so we have to use Enum to use other expressions.
    case DOWN:
      downArrow();
      break;
    case UP:
      upArrow();
      break;
    case RIGHT:
      rightArrow();
      turning = false;
      break;
    case LEFT:
      leftArrow();
      turning = false;
      break;
    default: //If we drew nothing default to cross
      cross(); 
  }
}
