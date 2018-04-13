
#define chanA 2
#define chanAInt 0
#define chanB 3
#define chanBInt 1
#define pwm 4
#define dir 5
#define pan A1

//int pan_op;

const int CPR=4096;// Pololu CPR=64;
volatile long int count=0;
volatile unsigned long int lastMilli=0;
volatile unsigned long int milli=0;
volatile bool chA,chB,chAP,chBP;
volatile int dr=1;
volatile int updateTime=20;
volatile float rpm=0.0;
volatile float panRead=0.0;
volatile float moAng=0.0;
volatile float shAng=0.0;

double Kp=6.713;
double Ki=564.793;
double Kd=100;
volatile float errPast=0.0;
volatile float parPast=0.0;
volatile float err=0.0;
volatile float errDer=0.0;
volatile float errInt=0.0;
volatile float cntr=0.0;

int ref=0;
int pwmVal=0;
int time=0;

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
  
//  digitalWrite(dir,HIGH);      // set DIR pin HIGH or LOW
//
//  getPanOp(pan);
//  analogWrite(pwm,panRead);
//    ref=90;

  if (time>=0 && time<250){
    ref=0;
  }else if (time>=250 && time<500){
    ref=45;
  }else if (time>=500 && time<750){ 
    ref=-45;
  }else time=0;
  
  pwmVal=abs(PID(Kp,Ki,Kd,ref,rpm));//1.34,0,0.123
  digitalWrite(dir,dr);
  analogWrite(pwm,pwmVal);
  time++;
//  if(time>20000) time=0;
//  Serial.print("Angle:");Serial.print(shAng);Serial.print(" ;Direction:");Serial.println(dr);
//  delay(100);

//  Serial.print("RPM:");Serial.print(rpm);Serial.print(" ;Direction:");Serial.println(dr);
//  delay(100);
//Serial.println(ref);
  delay(20);  
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
//  noInterrupts();
  rpm=float(14.648*count/updateTime);//float((60000*count)/(CPR*updateTime));//(count/64)/(0.4/60) for both channels?32 CPR per channel
//  moAng+=(5.625*count);
//  shAng+=(0.3*count);
//  interrupts();  
  count=0;
//  if(rpm<0) dr=-1; 
//  else dr=1;
  rpm=abs(rpm);
  
}

float PID(double P,double I,double D, int refer, int actual){
  err=refer-actual;
//  errDer=err-errPast;
  errDer=parPast-actual;
  errInt+=err;
  errInt=constrain(errInt,-180,180);
//  cntr=constrain(((P*err)+(I*errInt)+(D*errDer)),-255,255);
  cntr=constrain(((P*err)+(I*errInt)+(D*errDer)),-255,255);
  if(cntr<0) dr=LOW; 
  else dr=HIGH;  
  errPast=err;
  parPast=actual;
  return int(cntr);
}

