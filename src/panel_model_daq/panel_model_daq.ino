#define chanA 2
#define chanAInt 0
#define chanB 3
#define chanBInt 1
#define pwm 4
#define dir 5
#define pan A1
int pan_op;
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


}

void loop(){

int pwm_value=0;
digitalWrite(dir,HIGH);                                          // set DIR pin HIGH or LOW
float reading = 0.0;               //get average five consecutive analog readings from A1 pin (pot)

for(int i =0;i<5;i++)
reading += analogRead(pan);

reading/=5;
pan_op=reading*0.2493;          //convert from 10 bit to 8 bit, 0.2493 = 255/1023 5v/1023=0.004887
Serial.println(pan_op);
delay(100);
analogWrite(pwm,pan_op);
//for(pwm_value=0;pwm_value<256;pwm_value++){
//analogWrite(pwm,pwm_value);                               //increase PWM in every 0.1 sec
//delay(100);
//}


//while(1) continue;                                               //continue,terminate loop;
}
