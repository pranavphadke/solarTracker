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
volatile bool chA,chB,chAP,chBP,dr;
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
  attachInterrupt(1,pulseCount,CHANGE);
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

  Serial.print("RPM:");Serial.print(getRPM(64,updTime));Serial.print(" ;Direction:");Serial.println(dr);
  delay(100);
    
}

void pulseCount(){
//  change count (tick) when Hall sensor channel A o/p changes 
//  increment count (tick) (clockwise rot) when channel B o/p is not similar to channel A
//  else decrement count (tick) (anti-clockwise rot)
  chA=digitalRead(chanA);
  chB=digitalRead(chanB);
//  if (chA!=chB){
//    count++;
//    dr=HIGH;
//  }else{
//    count--;
//    dr=LOW;
//  }
  if(chA==HIGH && chB==HIGH){
    if(chAP==HIGH && chBP==LOW){
      count++;
      dr=HIGH;
    }
    if(chAP==LOW && chBP==HIGH){
      count--;
      dr=LOW;
    }
  }
  if(chA==HIGH && chB==LOW){
    if(chAP==HIGH && chBP==HIGH ){
      count--;
      dr=LOW;
    }
    if(chAP==LOW && chBP==LOW){
      count++;
      dr=HIGH;
    }
  }
  if(chA==LOW && chB==LOW){
    if(chAP==HIGH && chBP==LOW){
      count--;
      dr=LOW;
    }
    if(chAP==LOW && chBP==HIGH){
      count++;
      dr=HIGH;
    }
  }  
  if(chA==LOW && chB==HIGH){
    if(chAP==HIGH && chBP==HIGH){
      count++;
      dr=HIGH;
    }
    if(chAP==LOW && chBP==LOW){
      count--;
      dr=LOW;
    }
  }
chAP=chA;
chBP=chB;
}

void getPanOp(int panPort){
// panPort= Analog port on which panel is connected
  panRead = 0.0;               //get average five consecutive analog readings from A1 pin (pot)
  for(int i =0;i<5;i++)
    panRead += analogRead(panPort);
  panRead*=(0.2493/5);        //convert from 10 bit to 8 bit, 0.2493 = 255/1023 5v/1023=0.004887
}

float getRPM(int CPR,int updateTime){
// CPR = Counts per revolution of the motor shaft
// updateTime = Time interval in milli seconds for calculating RPM 
// senseActive = Number of hall sensor channels in use
  if(millis()-lastMillis >= updTime){
    noInterrupts();
    lastMillis=millis();
    rpm=float((60000*count)/(CPR*updateTime));//(count/64)/(0.4/60) for both channels?32 CPR per channel
    count=0;
    interrupts();
  }
  return rpm;
}
