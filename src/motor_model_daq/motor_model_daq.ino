#define chanA 2
#define chanAInt 0
#define chanB 3
#define chanBInt 1
#define pwm 4
#define dir 5
#define pan A1

//const int CPR=64;
int comm=0;
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
  digitalWrite(dir,HIGH);
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

ISR(TIMER1_COMPA_vect){ // TIMER 1 COMPARE A ISR  
  rpm=float(937.5*count/updateTime);//float((60000*count)/(CPR*updateTime));//(count/64)/(0.4/60) for both channels?32 CPR per channel
  count=0;
  milli=millis();
}

void loop(){
//  if(Serial.available() > 0)comm=Serial.parseInt();
//  analogWrite(pwm,0);
//  while(comm==1){
    analogWrite(pwm,255);
    Serial.print(milli);Serial.print(",");Serial.println(rpm);
    delay(20);
//    if(Serial.available() > 0)comm=Serial.parseInt();
//  }  
}
