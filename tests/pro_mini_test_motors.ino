/*******************************************************************************
 * Robo-Spinner V1.0
 * Filename : Rectangle_Running.pde
 * Running 90 Degree Turnleft And Turnright
 ********************************************************************************/
#define PWMRANGE 1023

#define M1PIN1 2
#define M1PIN2 4
#define M2PIN1 7 
#define M2PIN2 8

#define M1ENE  3
#define M2ENE  6

#define BEEP   11

int speed=0;
int max_speed=256;
int test_type=0;

void setup(){
    Serial.begin ( 115200 );
    delay(1000);
    Serial.print("Starting setup..");

    pinMode(M1PIN1,OUTPUT);              // Motor A1
    pinMode(M1PIN2,OUTPUT);              // Motor A2
    pinMode(M2PIN1,OUTPUT);              // Motor B2
    pinMode(M2PIN2,OUTPUT);              // Motor B1

    pinMode(M1ENE,OUTPUT);              // Motor A
    pinMode(M2ENE,OUTPUT);              // Motor B
    pinMode(BEEP,OUTPUT);                  // Motor B1

    //pinMode(2,INPUT);               // LeftSwitch
    //pinMode(4,INPUT);               // RightSwitch
    //pinMode(14,OUTPUT);             // PIEZO Speaker
    Motor_Stop();

    Serial.println("end setup..");

    // wait for ready
    Serial.println(F("\nSend any character to begin PWM motor tests: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again

}

void Forward(int speed){
    digitalWrite(M1PIN1,HIGH);
    digitalWrite(M1PIN2,LOW);
    digitalWrite(M2PIN1,HIGH);
    digitalWrite(M2PIN2,LOW);
    analogWrite(M1ENE,speed);
    analogWrite(M2ENE,speed);
}

void Backward(int speed){
    digitalWrite(M1PIN1,LOW);
    digitalWrite(M1PIN2,HIGH);
    digitalWrite(M2PIN1,LOW);
    digitalWrite(M2PIN2,HIGH);
    analogWrite(M1ENE,speed);
    analogWrite(M2ENE,speed);
}

void Spin_Left(int speed){
    analogWrite(M1PIN2,speed);
    digitalWrite(M1PIN1,LOW);
    analogWrite(M2PIN1,speed);
    digitalWrite(M2PIN2,LOW);
}

void Spin_Right(int speed){
    analogWrite(M1PIN1,speed);
    digitalWrite(M1PIN2,LOW);
    analogWrite(M2PIN2,speed);
    digitalWrite(M2PIN1,LOW);
}

void Motor_Stop(){
    digitalWrite(M1PIN1,LOW);
    digitalWrite(M1PIN2,LOW);
    digitalWrite(M2PIN1,LOW);
    digitalWrite(M2PIN2,LOW);
    analogWrite(M1ENE,0);
    analogWrite(M2ENE,0);
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

    if(test_type==0){
        Serial.print("\nTesting Forward PWM pins: "); 
        Serial.print(M1PIN1); Serial.print(","); Serial.println(M2PIN1);

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
        Serial.print(M1PIN2); Serial.print(","); Serial.println(M2PIN2);

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


