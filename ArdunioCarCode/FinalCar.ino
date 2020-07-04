#include <SoftwareSerial.h>
#define LEFT 1
#define STRAIGHT 2
#define RIGHT 3
#define BACKWARDS 0
#define STOP 1
#define FORWARDS 2

//Motor A direction
int INA = 4; 
//Motor A Speed
int PWMA = 5;
//Motor B direction 
int INB = 7; 
//Motor B Speed
int PWMB = 6; 

//use to record the command
int info[2] = {0}; 
int command;
char c;
int i;

//Rx, Tx
SoftwareSerial esp8266(2, 3); 

void setup(){
  Serial.begin(9600);  //For Serial monitor
  esp8266.begin(9600); //ESP Baud rate
  pinMode(INA,OUTPUT);
  pinMode(INB,OUTPUT);   //配置电机输出IO口为输出
}

void loop(){
 //int value = 20;
 //if(value == 100){    //循环每次速度增加2最大到255
 // motosp(-100,100);//电机速度赋值，左电机负值为反转，右电机正转。
 // delay(100);
 // value = value;
 //}
 //->
 //->
 //->
 if (esp8266.available() > 0) {
    c = esp8266.read();
    Serial.println("command: "+c);
    if (c == ':') {
      command = esp8266.parseInt();
      setSpeed(command);
      while (esp8266.read() != -1) {
        //DO NOTHING - to clear the buffer.
      }
    }
  }
}

//set the Car speed
void setSpeed(int newSpeed){
  int temp = newSpeed;
  //Serial Debug
  //Serial.print("SpeedCommand: ");
  //Serial.println(temp);
  //Get Info
  i = 0;
  while (temp) {
    info[i] = temp % 10;
    temp /= 10;
    i++;
  }

  
   // Switch case block for rear wheels.
   //BACKWARDS->0
   //STOP->1
   //FORWARDS->2
  switch (info[0]) {
    case BACKWARDS:
      digitalWrite(INA, LOW);
      digitalWrite(INB, HIGH); 
      analogWrite(PWMA, -5);
      analogWrite(PWMB, -5);
      break;
    case STOP:
      analogWrite(PWMA, 0);
      analogWrite(PWMB, 0);
      break;
      //has problem
    case FORWARDS:
      digitalWrite(INA, HIGH);
      digitalWrite(INB, LOW);
      analogWrite(PWMA, -5);
      analogWrite(PWMB, -5);
      break;
  }

  // Switch case block for front wheels.
  //This place should be improve
  //
  //->
  //
  
  switch (info[1]) {
    case LEFT:
      //forward first
      digitalWrite(INA, HIGH);
      digitalWrite(INB, LOW);
      analogWrite(PWMB,-15); 
      break;
    case STRAIGHT:
      //forward first
      //digitalWrite(INA, HIGH);
      //digitalWrite(INB, LOW);
      break;
    case RIGHT:
      //forward first
      digitalWrite(INA, HIGH);
      digitalWrite(INB, LOW);
      analogWrite(PWMA,-15);
      break;
  }
}

void motosp(int sp1,int sp2)//声明电机速度控制函数。括号内定义的变量分别为左右电机速
//度值，范围-255～+255，正值为正转，负值为反转。
{
  if(sp1>0) {
   digitalWrite(INA, HIGH);    
  }
  else
  {
   digitalWrite(INA, LOW); 
  }
  if(sp2>0)
  {
   digitalWrite(INB, HIGH); 
  }
  else
  {
   digitalWrite(INB, LOW); 
  }
  analogWrite(PWMA,abs (sp1)); 
  analogWrite(PWMB,abs (sp2));
}
