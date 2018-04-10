
#define chanA 2
#define chanAInt 0
#define chanB 3
#define chanBInt 1
#define pwm 4
#define dir 5
#define pan A1

//int pan_op;
const int CPR=4096;// Pololu DC Motor CPR=64
volatile long int count=0;
volatile long int countA=0;
volatile long int countB=0;
volatile unsigned long int lastMilli=0;
volatile unsigned long int milli=0;
volatile bool chA,chB,chAP,chBP;
volatile int dr=1;
volatile int updateTime=20;
volatile float rpm=0.0;
volatile float panRead=0.0;

void setup(){
  noInterrupts();
  pinMode(pwm,OUTPUT);
  pinMode(dir,OUTPUT);
  pinMode(chanA,INPUT);
  digitalWrite(chanA,LOW);
  pinMode(chanB,INPUT);
  digitalWrite(chanB,LOW);
  pinMode(pan,INPUT);
  Serial.begin(9600);
//  attachInterrupt(0,pulseCount,CHANGE);
//  attachInterrupt(1,pulseCount,CHANGE);

  // Attach interrupt on encoder channels
  attachInterrupt(0,pulseCountA,CHANGE);
  attachInterrupt(1,pulseCountB,CHANGE);
  
  // Attached timer interrupt to calculate RPM
  TCCR1A=0;// RESET REGISTER
  TCCR1B=0;
  TCNT1=0;
  
  OCR1A=250*updateTime;//12500;// COMPARE MATCH REGISTER SET FOR MEGA RUNNING AT 16MHZ WITH PRESCALER 64 COUNTING FOR 20HZ
  TCCR1B |= (1 << CS11);// SET PRESCALER TO 64
  TCCR1B |= (1 << CS10);// SET PRESCALER TO 64
  TCCR1B |= (1 << WGM12);// CTC MODE
  TIMSK1 |= (1 << OCIE1A);// ENABLE TIMER 1 COMPARE A INTERRUPT
  
  interrupts();
}

void loop(){

//int pwm_value=-255;
//for(pwm_value=-255;pwm_value<256;pwm_value++){
//  if(pwm_value<0){
//    digitalWrite(dir,LOW);
//  }else{
//    digitalWrite(dir,HIGH);
//  }
//  analogWrite(pwm,abs(pwm_value)); //increase PWM in every 0.1 sec
//  Serial.print("RPM:");Serial.print(getRPM(64,updTime));Serial.print(" ;Direction:");Serial.println(dr);
//  delay(100);
//}
  
//  digitalWrite(dir,HIGH);      // set DIR pin HIGH or LOW

  analogWrite(pwm,255);

//  getPanOp(pan);
//  analogWrite(pwm,panRead);
  
//  getRPM();

  Serial.print("RPM:");Serial.print(rpm);Serial.print(" ;Direction:");Serial.println(dr);

//  Serial.print("Channel A:");Serial.print(countA);Serial.print(" ||| Channel B:");Serial.println(countB);

//  Serial.print("Count:");Serial.println(count);
  delay(100);
    
}

void pulseCountA(){
  chA=digitalRead(chanA);
  chB=digitalRead(chanB);  
  if(chA==chB) count++;// Read and compare digital port 2 and 3
  else count--;  
}

void pulseCountB(){
  chA=digitalRead(chanA);
  chB=digitalRead(chanB);
  if(chB!=chA) count++;// Read and compare digital port 2 and 3
  else count--;
}

void getPanOp(int panPort){
// panPort= Analog port on which panel is connected
  panRead = 0.0;               //get average five consecutive analog readings from A1 pin (pot)
  for(int i =0;i<5;i++)
    panRead += analogRead(panPort);
  panRead*=0.04986;//(0.2493/5);        //convert from 10 bit to 8 bit, 0.2493 = 255/1023 5v/1023=0.004887
}

ISR(TIMER1_COMPA_vect){ // TIMER 1 COMPARE A ISR
  
  rpm=float(14.65*count/updateTime);//float((60000*count)/(CPR*updateTime));//(count/64)/(0.4/60) for both channels?32 CPR per channel
  count=0;
  
  if(rpm<0) dr=-1; 
  else dr=1;
  rpm=abs(rpm);
}
