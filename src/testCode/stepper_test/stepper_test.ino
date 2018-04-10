int dirPin = 5;
int stepperPin = 4;
int stepDriveSleep = 6;
int MS1 = 7;
int MS2 = 8;
int MS3 = 9;
int driveEnable = 10;
int c=0;
void setup() {
 pinMode(dirPin, OUTPUT);
 pinMode(stepperPin, OUTPUT);
 pinMode(stepDriveSleep,OUTPUT);
 pinMode(MS1,OUTPUT);
 pinMode(MS2,OUTPUT);
 pinMode(MS3,OUTPUT);
 pinMode(driveEnable,OUTPUT);
 Serial.begin(9600);
 digitalWrite(MS1,LOW); // Full Step
 digitalWrite(MS2,LOW);
 digitalWrite(MS3,LOW);
 digitalWrite(driveEnable,LOW); // Drive Enable
 
}
 void step(boolean dir,int steps){
 digitalWrite(dirPin,dir);
 delay(50);
 for(int i=0;i<steps;i++){
   digitalWrite(stepperPin, HIGH);
   delayMicroseconds(800);
   digitalWrite(stepperPin, LOW);
   delayMicroseconds(800);
 }
}
void loop(){
  Serial.print("c=");Serial.println(c);
  c++;
if (c>=0 & c<3){
   digitalWrite(stepDriveSleep,HIGH); // Disable Sleep
   step(true,200);
   delay(1000);
   step(false,200*2);
   delay(1000);
   step(true,200);
   delay(2000);
}else if (c>=3 & c<6){
  digitalWrite(stepDriveSleep,LOW); // Disable Sleep
  delay(4000);
}else if (c>6) c=0;
 
 delay(100);
}
