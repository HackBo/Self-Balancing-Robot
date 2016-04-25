/*******************************************************************************************
 * Self Balancing Robot (2016 @hpsaturn)
 *
 * REVISION
 * ________________________________________________________________________________________
 *
 * 20160412 Add macros for serial/analog tunning vars
 * 20160411 Refactor functions and tunning
 * 20160410 Ready test on ArduinoProMini y vertical IMU (change input and vars on PID)
 * 20160409 Change motor constans and pin for IMU interrupt
 * 20160409 Initial base code from Franco Robot (@gabricluka https://github.com/lukagabric)
 *
 ******************************************************************************************/

#include <PID_v1.h>
#include <LMotorController.h>
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

#define LOG_INPUT               0
#define MANUAL_TUNING           0
#define MANUAL_TUNING_SERIAL    0
#define LOG_PID_CONSTANTS       0 //MANUAL_TUNING must be 1
#define MOVE_BACK_FORTH         0
#define MIN_ABS_SPEED           1

//MPU
#define MPU_INT  5
MPU6050 mpu;


// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer


// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

//PID
#define YPR_OUTPUT_SELECT 1
#if MANUAL_TUNING
double prevKp, prevKi, prevKd;
#endif
#define SAMPLE_TIME 5
//double kp=60, ki=240, kd=1.23;
double kp=57, ki=240, kd=2.0;
double originalSetpoint = 169.43;  // for vertical orientation (IMU board)
double setpoint = originalSetpoint;
double movingAngleOffset = 0.3;
double input, output;
int moveState=0; //0 = balance; 1 = back; 2 = forth

//PID pid(&input, &output, &setpoint, 70, 240, 1.9, DIRECT);
//PID pid(&input, &output, &setpoint, 62, 242, 1.28, DIRECT);
PID pid(&input, &output, &setpoint, kp, ki, kd, DIRECT);

//MOTOR CONTROLLER
int ENA = 6;
int IN1 = 7;
int IN2 = 8;
int IN3 = 11;
int IN4 = 10;
int ENB = 9;

LMotorController motorController(ENA, IN1, IN2, ENB, IN3, IN4, 1, .90);


//timers
long time1Hz = 0;
long time5Hz = 0;

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady()
{
    //Serial.println(F("dmpDataReady!"));
    mpuInterrupt = true;
}

void setup()
{
    // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    // initialize serial communication
    Serial.begin(115200);
    delay(1000);
    //while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
    //mpu.setZAccelOffset(1688); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0)
    {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        pinMode(MPU_INT,INPUT);
        attachInterrupt(digitalPinToInterrupt(MPU_INT), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();

        //setup PID    
        pid.SetMode(AUTOMATIC);
        pid.SetSampleTime(SAMPLE_TIME);
        pid.SetOutputLimits(-255, 255);  

    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}


void loop()
{
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize)
    {
        //no mpu data - performing PID calculations and output to motors

        pid.Compute();
        motorController.move(output, MIN_ABS_SPEED);

        unsigned long currentMillis = millis();

        if (currentMillis - time1Hz >= 1000)
        {
            loopAt1Hz();
            time1Hz = currentMillis;
        }

        if (currentMillis - time5Hz >= 5000)
        {
            loopAt5Hz();
            time5Hz = currentMillis;
        }

    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024)
    {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

        // otherwise, check for DMP data ready interrupt (this should happen frequently)
    }
    else if (mpuIntStatus & 0x02)
    {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);

        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

#if LOG_INPUT
        Serial.print("ypr\t");
        Serial.print(ypr[0] * 180/M_PI);
        Serial.print("\t");
        Serial.print(ypr[1] * 180/M_PI);
        Serial.print("\t");
        Serial.print(ypr[2] * 180/M_PI);
        Serial.print("\tout: ");
        Serial.println(ypr[YPR_OUTPUT_SELECT] * 180/M_PI + 180);
#endif
        input = ypr[YPR_OUTPUT_SELECT] * 180/M_PI + 180;

    }

}

void loopAt1Hz()
{
#if MANUAL_TUNING
    setPIDTuningValues();
#endif
}


void loopAt5Hz()
{
#if MOVE_BACK_FORTH
    moveBackForth();
#endif
}

//move back and forth
void moveBackForth()
{
    moveState++;
    if (moveState > 2) moveState = 0;

    if (moveState == 0)
        setpoint = originalSetpoint;
    else if (moveState == 1)
        setpoint = originalSetpoint - movingAngleOffset;
    else
        setpoint = originalSetpoint + movingAngleOffset;
}

//PID Tuning (3 potentiometers)
#if MANUAL_TUNING
void setPIDTuningValues()
{
#if MANUAL_TUNING_SERIAL
    receiveData();
#else
    readPIDTuningValues();
#endif

    if (kp != prevKp || ki != prevKi || kd != prevKd)
    {
        printConsts();
        pid.SetTunings(kp, ki, kd);
        prevKp = kp; prevKi = ki; prevKd = kd;
    }
}

void readPIDTuningValues()
{
    int potKp = analogRead(A0);
    int potKi = analogRead(A1);
    int potKd = analogRead(A2);

    kp = map(potKp, 0, 1023, 0, 25000) / 100.0; //0 - 250
    ki = map(potKi, 0, 1023, 0, 100000) / 100.0; //0 - 1000
    kd = map(potKd, 0, 1023, 0, 500) / 100.0; //0 - 5
}

// This function is used to debug the robot, changig the set 
// value of the PID
void receiveData(){

    if (Serial.available()){
        char a = Serial.read();
        switch (a){
            case 'q':
                setSetpoint(getSetPoint()+0.01);
                break;
            case 'w':
                setSetpoint(getSetPoint()-0.01);
                break;
            case 'e':
                setSetpoint(getSetPoint()+0.1);
                break;
            case 'r':
                setSetpoint(getSetPoint()-0.1);
                break;
            case 'p':
                kp--;
                break;
            case 'i':
                ki=ki-2;
                break;
            case 'd':
                kd=kd-0.01;
                break;
            case 'P':
                kp++;
                break;
            case 'I':
                ki=ki+2;
                break;
            case 'D':
                kd=kd+0.01;
                break;
        } 

    }

}

void setSetpoint(double d){
    setpoint = d; 
    originalSetpoint=setpoint;
    printConsts();
}

void printConsts(){
#if LOG_PID_CONSTANTS
    Serial.print(kp);Serial.print(", ");Serial.print(ki);Serial.print(", ");Serial.print(kd);Serial.print(", ");Serial.println(setpoint);
#endif
}

double getSetPoint(){
    return setpoint;
}

#endif
