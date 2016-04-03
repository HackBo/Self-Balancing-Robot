/*******************************************************************************
 * Self Balancing Robot
 * Filename : Rectangle_Running.pde
 *
 * REVISION
 * 20160403 Testing motors and PWM soft functions with ESP8266
 ********************************************************************************/

int MAX_CICLES=3;
int cicles=0;

#define M1PIN1 D0
#define M1PIN2 D1
#define M2PIN1 D3
#define M2PIN2 D4


void setup(){
    pinMode(M1PIN1,OUTPUT);              // Motor A1
    pinMode(M1PIN2,OUTPUT);              // Motor A2
    pinMode(M2PIN1,OUTPUT);              // Motor B2
    pinMode(M2PIN2,OUTPUT);              // Motor B1

    Serial.begin ( 115200 );
    //pinMode(2,INPUT);               // LeftSwitch
    //pinMode(4,INPUT);               // RightSwitch
    //pinMode(14,OUTPUT);             // PIEZO Speaker

    Serial.println("end setup..");

}

void Forward(int speed){
    analogWrite(M1PIN1,speed);
    digitalWrite(M1PIN2,LOW);
    analogWrite(M2PIN1,speed);
    digitalWrite(M2PIN2,LOW);
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
}

void Beep(){
    int i;
    for (i=0;i<1200;i++){  
        digitalWrite(14,HIGH);
        delayMicroseconds(150);
        digitalWrite(14,LOW);
        delayMicroseconds(150);
    }
}

void loop(){

    Serial.println("on first motor");
    if (digitalRead(2)==0){         // Switch Di2 Press
        cicles=0;
        //Beep();
        delay(1000);
        while(cicles<MAX_CICLES){
            Forward(1023);
            delay(900);
            Spin_Left(512);            // Turnleft 90 degree
            delay(400);
            cicles++;
        }
        Motor_Stop();
    }
    /*
       if (digitalRead(4)==0){         // Switch Di4 Press
       cicles=0;
       Beep();
       delay(1000);
       while(cicles<MAX_CICLES){
       Forward(255);
       delay(900);
       Spin_Right(512);           // Turnright 90 degree
       delay(400);
       cicles++;
       }
       Motor_Stop();
       }
       */

    /*
       Serial.println("on first motor");
       Forward(1024);
       */
    /*
       Serial.println("on second motor");
       digitalWrite(M2PIN1,LOW);
       digitalWrite(M2PIN2,HIGH);
       */
}



