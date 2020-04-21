#define PWMA 11
#define AIN2 49
#define AIN1 48

#define PWMB 10
#define BIN2 46
#define BIN1 47

#define pos2degrees 0.507280

#define ENA_A 19
#define ENA_B 42
#define ENB_A 18
#define ENB_B 43

volatile long int encoderA_pos;
volatile long int encoderB_pos;
volatile long int encoderA_speed;
volatile long int encoderB_speed;
volatile long int encoderA_target_speed;
volatile long int encoderB_target_speed;

void setup() {
  Serial.begin(9600);
  
  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  
  pinMode(PWMB, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  
  pinMode(ENA_A, INPUT); //setup all four encoder pins as input
  pinMode(ENA_B, INPUT);
  pinMode(ENB_A, INPUT);
  pinMode(ENB_B, INPUT);
  
  //setup encoder interupts
  EICRA |= B01010000; //setup pins 18 and 19 and external interups, falling & rising
  EIMSK |= B00001100; //enable external interputs on pins 18 and 19
  
  //setup timer intrupt for 25Hz
  TCCR5A = 0;
  TCCR5B = 0;
  TCNT5  = 0;
  OCR5A = 2499;         // compare match register 25Hz
  TCCR5B |= (1 << WGM12);   // CTC mode
  TCCR5B |= (1 << CS12);    // 256 prescaler
  TIMSK5 |= (1 << OCIE1A);  // enable timer compare interrupt
  
  
  move_tank(0, 46);
}

void loop() {
 

    Serial.println(get_motor_speed(1)); 
    delay(100);   
  
}




ISR (INT3_vect){  //interupt call back for pin 19
   if( ((PIND & (1 << 3)) && (PINL & (1 << 6))) || ((~PIND & (1 << 3)) && (~PINL & (1 << 6))) )encoderA_pos--;
   else encoderA_pos++;
}

ISR (INT2_vect){  //interupt call back for pin 18
   if( ((PIND & (1 << 2)) && (PINL & (1 << 7))) || ((~PIND & (1 << 2)) && (~PINL & (1 << 7))) )encoderB_pos--;
   else encoderB_pos++; 
}

ISR(TIMER5_COMPA_vect){  //interupt call back for timer for speed regulation.
  volatile long int encoderA_last_pos;
  volatile long int encoderB_last_pos;
  encoderA_speed = encoderA_pos - encoderA_last_pos;
  encoderB_speed = encoderB_pos - encoderB_last_pos;
  encoderA_last_pos = encoderA_pos;
  encoderB_last_pos = encoderB_pos;  
}



float encoder_degress_pos(int encoder_num){
  if(encoder_num) return pos2degrees * encoderB_pos;
  else return pos2degrees * encoderA_pos;  
} 

float get_motor_speed(int encoder_num){
  if(encoder_num) return encoderB_speed * 2.11366;
  else return encoderA_speed * 2.11366;
  
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

 
