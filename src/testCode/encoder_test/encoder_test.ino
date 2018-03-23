
#define chanA 2
#define chanAInt 0
#define chanB 3
#define chanBInt 1
#define pwm 4
#define dir 5
#define pan A1

//int pan_op;
const int CPR=64;
volatile unsigned long int count=0;
volatile unsigned long int lastMilli=0;
volatile unsigned long int milli=0;
volatile bool chA,chB,chAP,chBP;
volatile int dr=1;
volatile int updateTime=400;
volatile float rpm=0.0;
volatile float panRead=0.0;

void setup(){
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

  attachInterrupt(0,pulseCountA,CHANGE);
  attachInterrupt(1,pulseCountB,CHANGE);
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

  digitalWrite(dir,HIGH);      // set DIR pin HIGH or LOW

//  analogWrite(pwm,100);

  getPanOp(pan);
  analogWrite(pwm,panRead);

  Serial.print("RPM:");Serial.print(getRPM());Serial.print(" ;Direction:");Serial.println(dr);
  delay(100);
    
}

//void pulseCount(){
////  change count (tick) when Hall sensor channel A o/p changes 
////  increment count (tick) (clockwise rot) when channel B o/p is not similar to channel A
////  else decrement count (tick) (anti-clockwise rot)
////  chA=digitalRead(chanA);
////  chB=digitalRead(chanB);
////  if (chA!=chB){
////    count++;
////    dr=HIGH;
////  }else{
////    count--;
////    dr=LOW;
////  }
//
////  if(chA==HIGH && chB==HIGH){
////    if(chAP==HIGH && chBP==LOW){
////      count++;
////      dr=HIGH;
////    }
////    if(chAP==LOW && chBP==HIGH){
////      count--;
////      dr=LOW;
////    }
////  }
////  if(chA==HIGH && chB==LOW){
////    if(chAP==HIGH && chBP==HIGH ){
////      count--;
////      dr=LOW;
////    }
////    if(chAP==LOW && chBP==LOW){
////      count++;
////      dr=HIGH;
////    }
////  }
////  if(chA==LOW && chB==LOW){
////    if(chAP==HIGH && chBP==LOW){
////      count--;
////      dr=LOW;
////    }
////    if(chAP==LOW && chBP==HIGH){
////      count++;
////      dr=HIGH;
////    }
////  }  
////  if(chA==LOW && chB==HIGH){
////    if(chAP==HIGH && chBP==HIGH){
////      count++;
////      dr=HIGH;
////    }
////    if(chAP==LOW && chBP==LOW){
////      count--;
////      dr=LOW;
////    }
////  }
////chAP=chA;
////chBP=chB;
//
//  if(bitRead(PORTD,2)==bitRead(PORTD,3)) count++;// Read and compare digital port 2 and 3
//  else count--;
//  
//}

void pulseCountA(){
  if(bitRead(PORTD,2)==bitRead(PORTD,3)) count++;// Read and compare digital port 2 and 3
  else count--;  
}

void pulseCountB(){
  if(bitRead(PORTD,2)!=bitRead(PORTD,3)) count++;// Read and compare digital port 2 and 3
  else count--;  
}

void getPanOp(int panPort){
// panPort= Analog port on which panel is connected
  panRead = 0.0;               //get average five consecutive analog readings from A1 pin (pot)
  for(int i =0;i<5;i++)
    panRead += analogRead(panPort);
  panRead*=0.04986;//(0.2493/5);        //convert from 10 bit to 8 bit, 0.2493 = 255/1023 5v/1023=0.004887
}

void getRPM(){
// CPR = Counts per revolution of the motor shaft
// updateTime = Time interval in milli seconds for calculating RPM 
// senseActive = Number of hall sensor channels in use
  noInterrupts();
  milli=millis();
  if(milli-lastMilli >= updateTime){
    lastMilli=milli;
    rpm=float(937.5*count/updateTime);//float((60000*count)/(CPR*updateTime));//(count/64)/(0.4/60) for both channels?32 CPR per channel
    count=0;
  }
  if(rpm<0) dr=-1; 
  else dr=1;
  rpm=abs(rpm);
  interrupts();
}
