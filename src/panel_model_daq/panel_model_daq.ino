#define pan A1
float pan_op;
void setup(){
pinMode(pan,INPUT);
Serial.begin(9600);
}

void loop(){
float reading = 0.0;               //get average five consecutive analog readings from A1 pin (pot)

for(int i =0;i<5;i++)
reading += analogRead(pan);

reading/=5;
pan_op=reading*0.009774;          //convert from 10 bit to 8 bit, 0.2493 = 255/1023 5v/1023=0.004887
Serial.print(millis());Serial.print(",");Serial.println(pan_op);
delay(20);
}
