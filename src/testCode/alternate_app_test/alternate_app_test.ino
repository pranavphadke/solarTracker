#define chanA 2
#define chanAInt 0
#define chanB 3
#define chanBInt 1
#define pwm 4
#define dir 5
#define pan A1

volatile float panOpC=0;
volatile float panOpP=0;
volatile float delPanOpD1=0;
volatile float delPanOpD2=0;
volatile int drTry=0;
volatile int drPass=0;
volatile int sI=0;
volatile bool dr=HIGH;
volatile bool drP=HIGH;
const float drTol=0.2; 

volatile long int count=0;
volatile unsigned long int lastMilli=0;
volatile unsigned long int milli=0;
volatile bool chA,chB,chAP,chBP;
volatile int updateTime=20;                                                     // Update time period for encoders in milli seconds
volatile float rpm=0.0;
volatile float panRead=0.0;
volatile float moAng=0.0;
volatile float shAng=0.0;
volatile float paAng=0.0;
volatile float probe[2][3]={{0,0,0},{0,0,0}};                                  // Index starts at 0
volatile float probeDiff[2]={0,0};
volatile int scanStep=5;
volatile int mul=0;

volatile int refTry=0;
volatile int refPass=0;
volatile float refPast=0;


double Kp=10;                                                               // 1.0
double Ki=0.0;                                                            // 0.0
double Kd=5;                                                                // 0.2
volatile float errPast=0.0;
volatile float err=0.5;
volatile float errDer=0.0;
volatile float errInt=0.0;
volatile float cntr=0.0;
volatile float angTol=0.25;
volatile int pidComp=0;

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
  
  // Attach timer1 interrupt to calculate RPM
  TCCR1A=0;                                                                    // RESET REGISTERS
  TCCR1B=0;
  TCNT1=0;
  
  OCR1A=250*updateTime;                                                        //12500; COMPARE MATCH REGISTER SET FOR MEGA RUNNING AT 16MHZ WITH PRESCALER 64 COUNTING FOR 20HZ
  TCCR1B |= (1 << CS11);                                                       // SET PRESCALER TO 64
  TCCR1B |= (1 << CS10);                                                       // SET PRESCALER TO 64
  TCCR1B |= (1 << WGM12);                                                      // CTC MODE
  TIMSK1 |= (1 << OCIE1A);                                                     // ENABLE TIMER 1 COMPARE A INTERRUPT
  
  // Attach timer interrupt to wake system
  
  interrupts();
}

void loop(){
  //  Rest for pre-determined amount of time then
  //  scan() then 
  //  getRef() then
  //  setRef()
//  while(sI==0){
//    scan();
////    Serial.print("Si:");Serial.print(sI);Serial.print(";dir:");Serial.println(dr);
//  }
  setRef(10,&paAng);
  
  delay(500);  
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

float getPanOp(int panPort){
// panPort = Analog port on which panel is connected
  panRead = 0.0;               
  for(int i =0;i<5;i++)
    panRead += analogRead(panPort);                                         //  get average five consecutive analog readings from A1 pin (pot)
  panRead*=0.009774/5;                                                        //  convert from 10 bit to 10V -> 0.009774=10/1023
  return panRead;
}

ISR(TIMER1_COMPA_vect){                                                     // TIMER 1 COMPARE A ISR
  rpm=float(937.5*count/updateTime);                                        //  60000*count)/(CPR*updateTime)
  moAng+=(5.625*count);
  shAng+=(0.3*count);
  paAng+=(0.01*count);
  count=0;
  rpm=abs(rpm);
}

float PID(double P,double I,double D, float refer, float actual){
  Serial.print("Inside PID with err:");Serial.print(err);Serial.print(" and pidComp:");Serial.println(pidComp);
  err=refer-actual;
  if (abs(err)<angTol & abs(errPast)<angTol){ 
    pidComp=1;
  }else pidComp=0;
  errDer=err-errPast;
  errInt+=err;
  errInt=constrain(errInt,-100,100);
  cntr=constrain(((P*err)+(I*errInt)+(D*errDer)),-255,255);
  if(cntr<0) dr=LOW; 
  else dr=HIGH;  
  errPast=err;
  return int(cntr);
}

void scan(){
  // Gets correction rotation direction and sets 'dr' variable
  drPass=0;
  drTry=0;
  while (drPass==0){
    drTry++;
//    digitalWrite(dir,LOW);
    panOpP=getPanOp(pan);
    setRef(paAng-scanStep,&paAng);
//    analogWrite(pwm,100);
//    delay(100);
//    analogWrite(pwm,0);
//    Serial.print(paAng);
//    Serial.print(" ");
//    delay(100);
    panOpC=getPanOp(pan);
    delPanOpD1=panOpP-panOpC;
//    Serial.println(delPanOpD1);
//    Serial.print(" ");
    setRef(paAng+2*scanStep,&paAng);
//    digitalWrite(dir,HIGH);
//    analogWrite(pwm,100);
//    delay(100);
//    analogWrite(pwm,0);
//    Serial.print(paAng);
//    delay(1000);
    panOpC=getPanOp(pan);
    delPanOpD2=panOpP-panOpC;
//    Serial.print(" ");
//    Serial.println(delPanOpD2);
//    Serial.print(" ");
    setRef(paAng-scanStep,&paAng);
//    digitalWrite(dir,LOW);
//    analogWrite(pwm,100);
//    delay(100);
//    analogWrite(pwm,0);
//    Serial.println(paAng);
    if (delPanOpD1<-drTol){
      dr=LOW;
      mul=-1;
      drPass=1;
      sI=1;
    }else if (delPanOpD2<-drTol){
      dr=HIGH;
      mul=1; 
      drPass=1;
      sI=1; 
    }else {
      drPass=0;                                                             // Find process to randomize direction selection
      setRef(paAng+3*scanStep,&paAng);
//      digitalWrite(dir,HIGH);                                               
//      analogWrite(pwm,100);
//      delay(1500);
//      analogWrite(pwm,0);
//      delay(100);
    }
    if (drTry>2){
      drPass=-1;                                                            // Find optimal way to allow system to sleep if no light source present
      sI=1;
    }
  }
  drP=dr;                                                                   // Store previous direction(Used in future additions)
}

void getRef(){
  // Gets new reference angle for system by using scanning direction and finding new maximum panel output
  // move through 5 step in direction identified
  refTry=0;
  refPass=0;
  
  while (refPass!=1){
    // find difference in past and current panel reading
    for(int i=0;i<2;i++){
      probe[0][i+1]=probe[0][i];
      probe[1][i+1]=probe[1][i];
    }
    setRef(paAng+(mul*scanStep/2),&paAng);
    probe[0][0]=getPanOp(pan);
    probe[1][0]=paAng;
    probeDiff[0]=probe[1]-probe[0];
    probeDiff[1]=probe[2]-probe[1];
    // if difference changes to positive stop and go back one step
    if (probeDiff[0]>0 & probeDiff[1]<0){
      refPass=1;// use sI=1
      setRef(probe[1][1],&paAng);
    }else if (refTry>5){
      refPass=1;// use sI=1
      setRef(refPast,&paAng);
    }
    refTry+=1;
  }
  refPast=paAng;
}

void setRef(float angRef,volatile float *opAngPt){
  // Set new reference angle for system to move to and optionally use PID controller to reach target
  Serial.print("In setRef with ");Serial.print(angRef);Serial.print(" & ");Serial.println(*opAngPt);
  ref=angRef;
  while (pidComp==0 | abs(angRef-*opAngPt)>angTol){
    pwmVal=abs(PID(Kp,Ki,Kd,angRef,*opAngPt));
    digitalWrite(dir,dr);
    analogWrite(pwm,pwmVal);
  }
  analogWrite(pwm,0);
  delay(100);
}
