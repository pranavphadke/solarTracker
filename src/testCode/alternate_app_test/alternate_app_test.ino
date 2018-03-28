
#define chanA 2
#define chanAInt 0
#define chanB 3
#define chanBInt 1
#define pwm 4
#define dir 5
#define pan A1

//int pan_op;

//const int CPR=64;
volatile long int count=0;
volatile unsigned long int lastMilli=0;
volatile unsigned long int milli=0;
volatile bool chA,chB,chAP,chBP;
volatile bool dr=HIGH;
volatile int updateTime=20;                                                     // Update time period for encoders in milli seconds
volatile float rpm=0.0;
volatile float panRead=0.0;
volatile float moAng=0.0;
volatile float shAng=0.0;

double Kp=1.0;
double Ki=0.0;
double Kd=0.2;
volatile float errPast=0.0;
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
  
  OCR1A=250*updateTime;                                                        //12500; COMPARE MATCH REGISTER SET FOR MEGA RUNNING AT 16MHZ WITH PRESCALER 64 COUNTING FOR 20HZ
  TCCR1B |= (1 << CS11);                                                       // SET PRESCALER TO 64
  TCCR1B |= (1 << CS10);                                                       // SET PRESCALER TO 64
  TCCR1B |= (1 << WGM12);                                                      // CTC MODE
  TIMSK1 |= (1 << OCIE1A);                                                     // ENABLE TIMER 1 COMPARE A INTERRUPT
  
  interrupts();
}

void loop(){
  //  Rest for pre-determined amount of time then
  //  scan() then 
  //  getRef() then
  //  setRef()
  
  delay(20);  
}

void pulseCountA(){
  chA=digitalRead(chanA);
  chB=digitalRead(chanB);  
  if(chA==chB) count++;                                                      // Read and compare digital port 2 and 3
  else count--;  
}

void pulseCountB(){
  chA=digitalRead(chanA);
  chB=digitalRead(chanB);
  if(chB!=chA) count++;                                                      // Read and compare digital port 2 and 3
  else count--;
}

void getPanOp(int panPort){
// panPort = Analog port on which panel is connected
  panRead = 0.0;               
  for(int i =0;i<5;i++)
    panRead += analogRead(panPort);                                         //  get average five consecutive analog readings from A1 pin (pot)
  panRead*=0.009774;                                                        //  convert from 10 bit to 10V -> 0.009774=10/1023
}

ISR(TIMER1_COMPA_vect){                                                     // TIMER 1 COMPARE A ISR
  rpm=float(937.5*count/updateTime);                                        //  60000*count)/(CPR*updateTime)
  moAng+=(5.625*count);
  shAng+=(0.3*count);
  count=0;
  rpm=abs(rpm);
  
}

float PID(double P,double I,double D, int refer, int actual){
  err=refer-actual;
  errDer=err-errPast;
  errInt+=err;
  errInt=constrain(errInt,-180,180);
  cntr=constrain(((P*err)+(I*errInt)+(D*errDer)),-255,255);
  if(cntr<0) dr=LOW; 
  else dr=HIGH;  
  errPast=err;
  return int(cntr);
}

void scan(){
  // Gets correction rotation direction and sets 'dr' variable
}

void getRef(){
  // Gets new reference angle for system by using scanning direction and finding new maximum panel output
}

void setRef(){
  // Set new reference angle for system to move to and optionally use PID controller to reach target
}
