/*******************************************************************************
 * Robo-Spinner V1.0
 * Filename : Rectangle_Running.pde
 * Running 90 Degree Turnleft And Turnright
 ********************************************************************************/

#include "I2Cdev.h"

//MOTOR CONTROLLER
int ENA = 6;
int IN1 = 8;
int IN2 = 7;
int IN3 = 10;
int IN4 = 11;
int ENB = 9;
int BEEP =12;

int MAX_CICLES=3;
int cicles=0;
int speed=0;

#define PWMRES 8
int max_speed=1<<PWMRES;
int test_type=0;

void setup(){
    Serial.begin ( 115200 );
    while (!Serial); // wait for Leonardo enumeration, others continue immediately
    Serial.print("Starting setup..");

    pinMode(ENA,OUTPUT);              // MotorA Enable Pin
    pinMode(IN1,OUTPUT);              // MotorA pin1
    pinMode(IN2,OUTPUT);              // MotorA pin2
    pinMode(IN3,OUTPUT);              // MotorB pin1
    pinMode(IN4,OUTPUT);              // MotorB pin2
    pinMode(ENB,OUTPUT);              // MotorB Enable Pin

    pinMode(BEEP,OUTPUT);             // Beep

    analogWriteRes(PWMRES);
    Motor_Stop();

    Serial.println("end setup..");

    // wait for ready
    Serial.println(F("\nSend any character to begin PWM motor tests: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again

}

void Forward(int speed){

    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(ENA,speed);
    analogWrite(ENB,speed);

}

void Backward(int speed){

    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    analogWrite(ENA,speed);
    analogWrite(ENB,speed);

}

void Motor_Stop(){

    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
    analogWrite(ENA,0);
    analogWrite(ENB,0);

}

void Beep(){
    int i;
    for (i=0;i<1200;i++){  
        digitalWrite(BEEP,HIGH);
        delayMicroseconds(150);
        digitalWrite(BEEP,LOW);
        delayMicroseconds(150);
    }
}

void loop(){


    Beep();

    if(test_type==0){
        Serial.print("\nTesting Forward PWM pins: "); 
        Serial.print(IN1); Serial.print(","); Serial.println(IN2);

        while(speed<max_speed){
            printSpeedBar(speed);
            Forward(speed++);
            delay(20);
        }
        Serial.print(" "); Serial.print(speed);
        Serial.println("\r\nEnd Test.."); 
        speed=0;
        test_type++;
        delay(2000);
    }

    if(test_type==1){
        Serial.print("\nTesting Backward PWM pins: "); 
        Serial.print(IN3); Serial.print(","); Serial.println(IN4);

        while(speed<max_speed){
            printSpeedBar(speed);
            Backward(speed++);
            delay(20);
        }
        Serial.print(" "); Serial.print(speed);
        Serial.println("\r\nEnd Test.."); 
        speed=0;
        test_type++;
        delay(2000);
    }

    if(test_type==2){
        Motor_Stop();
        Serial.println("END");
    }

    delay(100);

    Serial.println(F("\nSend any character to restart PWM motor tests: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again

    speed=0;
    test_type=0;

}

void printSpeedBar(int speed){
    if((speed % (max_speed/8)) == 0){
        Serial.print(" ");
        Serial.print(speed);
    }
}

