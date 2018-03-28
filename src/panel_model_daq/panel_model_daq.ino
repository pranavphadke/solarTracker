#define pan A1
float pan_op;
void setup(){
pinMode(pan,INPUT);
Serial.begin(9600);
}

void loop(){
float reading = 0.0;               

for(int i =0;i<5;i++)
reading += analogRead(pan);

reading/=5;                  //  get average five consecutive analog readings from A1 pin (pot)
pan_op=reading*0.009774;     //  convert from 10 bit to 10V -> 0.009774=10/1023
Serial.print(millis());Serial.print(",");Serial.println(pan_op);
delay(20);
}
