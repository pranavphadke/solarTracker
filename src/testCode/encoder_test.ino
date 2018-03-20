// #define diy_pwm A2
#define chanA 2
#define chanAInt 0
#define chanB 3
#define chanBInt 1
#define pwm 4
#define dir 5
#define pan A1
//int pan_op;
volatile long count=0;
volatile unsigned long lastMillis=0;
volatile bool chA,chB;
volatile bool dr;
int updTime=400;
float rpm=0.0;
volatile float panRead=0.0;
void setup(){
  pinMode(pwm,OUTPUT);
  pinMode(dir,OUTPUT);
  pinMode(chanA,INPUT);
  digitalWrite(chanA,LOW);
  pinMode(chanB,INPUT);
  digitalWrite(chanB,LOW);
  //pinMode(diy_pwm,OUTPUT);
  pinMode(pan,INPUT);
  Serial.begin(9600);
  attachInterrupt(0,pulseCount,CHANGE);
//  attachInterrupt(1,pulseCount,CHANGE)
}

void loop(){
//int pwm_value=0;
  digitalWrite(dir,HIGH);      // set DIR pin HIGH or LOW
//  analogWrite(pwm,100);
  getPanOp(pan);
  //Serial.println(pan_op);
  //delay(100);
  analogWrite(pwm,panRead);
//for(pwm_value=0;pwm_value<256;pwm_value++){
//analogWrite(pwm,pwm_value); //increase PWM in every 0.1 sec
//delay(100);
//}
  Serial.print("RPM:");Serial.print(-getRPM(64,updTime,1)/18.75);Serial.print(" ;Direction:");Serial.println(!dr);
  delay(100);  
}

void pulseCount(){
//  change count (tick) when Hall sensor channel A o/p changes 
//  increment count (tick) (clockwise rot) when channel B o/p is not similar to channel A
//  else decrement count (tick) (anti-clockwise rot)
  chA=digitalRead(chanA);
  chB=digitalRead(chanB);
  if (chA!=chB){
    count++;
    dr=HIGH;
  }else{
    count--;
    dr=LOW;
  }
}
void getPanOp(int panPort){
// panPort= Analog port on which panel is connected
  panRead = 0.0;               //get average five consecutive analog readings from A1 pin (pot)
  for(int i =0;i<5;i++)
    panRead += analogRead(panPort);
  panRead*=(0.2493/5);        //convert from 10 bit to 8 bit, 0.2493 = 255/1023 5v/1023=0.004887
}
float getRPM(int CPR,int updateTime, int senseActive){
// CPR = Counts per revolution of the motor shaft
// updateTime = Time interval in milli seconds for calculating RPM 
// senseActive = Number of hall sensor channels in use
  if(millis()-lastMillis >= updTime){
    noInterrupts();
    lastMillis=millis();
    rpm=float(2*60000*count/(CPR*senseActive*updateTime));//(count/64)/(0.4/60) for both channels?32 CPR per channel
    count=0;
    interrupts();
  }
  return rpm;
}
