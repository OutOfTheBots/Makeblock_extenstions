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


uint8_t read_raw_line_follower_array()
{  
  uint8_t Sensor_Data[3];  
  long time_out_flag = 0;
  pinMode(DataPin, OUTPUT);
  digitalWrite(DataPin, LOW);
  delayMicroseconds(980);
  digitalWrite(DataPin, HIGH);
  delayMicroseconds(40);
  pinMode(DataPin, INPUT_PULLUP);
  delayMicroseconds(50); 
  time_out_flag = millis();
  while((digitalRead(DataPin) == 0)&&((millis() - time_out_flag) < 6)); 
  time_out_flag = millis();
  while((digitalRead(DataPin) == 1)&&((millis() - time_out_flag) < 6));
  for(uint8_t k=0; k<3; k++){
    Sensor_Data[k] = 0x00;
    for(uint8_t i=0;i<8;i++){
      time_out_flag = millis(); 
      while(digitalRead(DataPin) == 0&&((millis() - time_out_flag) < 6));
      uint32_t HIGH_level_read_time = micros();
      time_out_flag = millis(); 
      while(digitalRead(DataPin) == 1&&((millis() - time_out_flag) < 6));
      HIGH_level_read_time = micros() - HIGH_level_read_time;
      if(HIGH_level_read_time > 50 && HIGH_level_read_time < 100)Sensor_Data[k] |= (0x80 >> i);
    }
  }
  if (Sensor_Data[1] == (uint8_t)(~(uint8_t)Sensor_Data[0])) return Sensor_Data[0];
}


float line_follower_array_error()
{
  int sensor_value;
  float error = 0;
  byte mask, start_pos, finish_pos;
  boolean line_found;
  
  mask = 1;
  line_found = 0;
  start_pos = 0;  
  
  sensor_value = read_raw_line_follower_array();
  sensor_value = (~sensor_value) & 63;  

  while(!line_found & (start_pos < 6)){
    if (sensor_value & (mask << start_pos)){
        line_found = 1;
        finish_pos = start_pos;
        while(sensor_value & (mask << finish_pos))finish_pos = finish_pos +1;
    }
    start_pos +=1;   
  }
  error = float((finish_pos + start_pos)) / 2 - 3.5;
  if(!line_found)error = 5;
  return error; 
}
