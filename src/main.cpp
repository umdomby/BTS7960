#include <Arduino.h>

#define RPWM D5
// #define LPWM 6
// #define REN 8
// #define LEN 9


int pot;
int out1;
int out2;

void setup() {
  Serial.begin(9600);
  pinMode(RPWM,OUTPUT);
  // pinMode(LPWM,OUTPUT);
  // pinMode(LEN,OUTPUT);
  // pinMode(REN,OUTPUT);
  // digitalWrite(REN,HIGH);
  // digitalWrite(LEN,HIGH);

}
 
 
void loop() {

  analogWrite(RPWM, 0);

  //analogWrite(LPWM,0);//NOOOOOOOOOOOO!!!
  
  //pot=analogRead(A0);

  // if(pot>512){
  //   out1=map(pot,512,1023,0,255);
  //   analogWrite(RPWM,out1);
  //   analogWrite(LPWM,0);
    
  // }
  
  // if(pot<512){
  //   out2=map(pot,512,0,0,255);
  //   analogWrite(LPWM,out2);
  //   analogWrite(RPWM,0);
    
  // }
}