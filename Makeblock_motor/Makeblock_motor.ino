const int DataPin = A11;

const int PWMA = 11;
const int AIN2 = 49;
const int AIN1 = 48;

const int PWMB = 10;
const int BIN2 = 46;
const int BIN1 = 47;

float kp = 40;
int steer;


void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  
  pinMode(PWMB, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
}  

void loop() {  
  steer = int(kp * line_follower_array_error())
  move_steer(40, steer);   
}


void move_tank (int left, int right){
  if(left>0){
    digitalWrite(BIN2, LOW);
    digitalWrite(BIN1, HIGH);
  }else{
    left *= -1;
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH); 
  }
  if(left>255)left=255;
  analogWrite(PWMB, left);
 
   if(right>0){
    digitalWrite(AIN2, LOW);
    digitalWrite(AIN1, HIGH);
  }else{
    right *= -1;
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH); 
  }
  if(left>255)left=255;
  analogWrite(PWMA, right);  
}
  

void move_steer(int speed, int steer){
  move_tank(speed + steer, speed - steer);  
}

