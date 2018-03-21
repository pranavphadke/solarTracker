#define chanA 2
#define chanAInt 0
#define chanB 3
#define chanBInt 1
#define pwm 4
#define dir 5
volatile long count=0;
volatile unsigned long lastMillis=0;
volatile bool chA,chB;
volatile bool dr;
int updTime=10;
float rpm=0.0;

void setup(){
  pinMode(pwm,OUTPUT);
  pinMode(dir,OUTPUT);
  pinMode(chanA,INPUT);
  digitalWrite(chanA,LOW);
  pinMode(chanB,INPUT);
  digitalWrite(chanB,LOW);
  Serial.begin(9600);
  attachInterrupt(0,pulseCount,CHANGE);
}

void loop(){
  digitalWrite(dir,HIGH);      // set DIR pin HIGH or LOW
  analogWrite(pwm,255);
  Serial.print(millis());Serial.print(",");Serial.println(-getRPM(64,updTime,1)/18.75);
  delay(20);  
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
