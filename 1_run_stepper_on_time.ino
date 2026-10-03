#include <Wire.h>
#include "RTClib.h"
 //is it working th

#define I2C_SDA 21
#define I2C_SCL 22

RTC_DS3231 rtc;

#define M1_IN1 18
#define M1_IN2 19
#define M1_IN3 23
#define M1_IN4 25

#define M2_IN1 26
#define M2_IN2 27
#define M2_IN3 32
#define M2_IN4 33

#define M3_IN1 16
#define M3_IN2 17
#define M3_IN3 13
#define M3_IN4 14

const int stepDelay = 3;


const long STEPS_PER_REVOLUTION = 4096;

const unsigned long readInterval = 1000;  // 1000 ms = 1 second
unsigned long lastReadTime = 0;



const int sequence[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};


long currentStep[3] = {0, 0, 0};

struct Schedule {
  int hour;
  int minute;
  int motor1Turns;
  int motor2Turns;
  int motor3Turns;
};

Schedule schedules[] = {
  {14, 58, 1, 0, 0},
  {17, 54, 2, 1, 0},
  {21, 15, 0, 0, 3}
};

const int NUMBER_OF_SCHEDULES =
  sizeof(schedules) / sizeof(schedules[0]);


  void setup() {

   Serial.begin(115200);

  Wire.begin(I2C_SDA, I2C_SCL);

  if (!rtc.begin()) {
    Serial.println("RTC NOT FOUND!");
    while (1);
  }

  Serial.println("RTC FOUND");

 
  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);
  pinMode(M1_IN3, OUTPUT);
  pinMode(M1_IN4, OUTPUT);



  pinMode(M2_IN1, OUTPUT);
  pinMode(M2_IN2, OUTPUT);
  pinMode(M2_IN3, OUTPUT);
  pinMode(M2_IN4, OUTPUT);



  pinMode(M3_IN1, OUTPUT);
  pinMode(M3_IN2, OUTPUT);
  pinMode(M3_IN3, OUTPUT);
  pinMode(M3_IN4, OUTPUT);


  
  releaseMotor(1);
  releaseMotor(2);
  releaseMotor(3);

 


}



  void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - lastReadTime >= readInterval) {
    lastReadTime = currentMillis;

    readTime();
    checkSchedules();
  }
  

}

  void rotateMotor(int motor, int parts) {

  if (motor < 1 || motor > 3) {
    return;
  }

  if (parts <= 0) {
    return;
  }


  
  long totalSteps =
    round((double)STEPS_PER_REVOLUTION * parts / 6.0);


  int motorIndex = motor - 1;


 
  for (long i = 0; i < totalSteps; i++) {

    currentStep[motorIndex]++;

    setMotorStep(
      motor,
      currentStep[motorIndex]
    );

    delay(stepDelay);
  }
  releaseMotor(motor);
}

  void releaseMotor(int motor) {

  if (motor == 1) {
    digitalWrite(M1_IN1, LOW);
    digitalWrite(M1_IN2, LOW);
    digitalWrite(M1_IN3, LOW);
    digitalWrite(M1_IN4, LOW);
  }
  else if (motor == 2) {
    digitalWrite(M2_IN1, LOW);
    digitalWrite(M2_IN2, LOW);
    digitalWrite(M2_IN3, LOW);
    digitalWrite(M2_IN4, LOW);
  }
  else if (motor == 3) {
    digitalWrite(M3_IN1, LOW);
    digitalWrite(M3_IN2, LOW);
    digitalWrite(M3_IN3, LOW);
    digitalWrite(M3_IN4, LOW);
  }
}


  void setMotorStep(int motor, long step) {

  int IN1;
  int IN2;
  int IN3;
  int IN4;

  if (motor == 1) {

    IN1 = M1_IN1;
    IN2 = M1_IN2;
    IN3 = M1_IN3;
    IN4 = M1_IN4;

  }
  else if (motor == 2) {

    IN1 = M2_IN1;
    IN2 = M2_IN2;
    IN3 = M2_IN3;
    IN4 = M2_IN4;
  

  }
  else {

    IN1 = M3_IN1;
    IN2 = M3_IN2;
    IN3 = M3_IN3;
    IN4 = M3_IN4;
  }


  int s = step % 8;

  digitalWrite(IN1, sequence[s][0]);
  digitalWrite(IN2, sequence[s][1]);
  digitalWrite(IN3, sequence[s][2]);
  digitalWrite(IN4, sequence[s][3]);
}





  void readTime() {

  DateTime now = rtc.now();

  int timeHM = now.hour() * 100 + now.minute();

  Serial.printf("HM: %04d\n", timeHM);
}

void checkSchedules() {

  DateTime now = rtc.now();

  int currentHour = now.hour();
  int currentMinute = now.minute();

  static int lastMinute = -1;

  // Prevent running repeatedly during the same minute
  if (currentMinute == lastMinute) {
    return;
  }

  lastMinute = currentMinute;

  for (int i = 0; i < NUMBER_OF_SCHEDULES; i++) {

    if (schedules[i].hour == currentHour &&
        schedules[i].minute == currentMinute) {

      Serial.printf(
        "Schedule matched: %02d:%02d\n",
        currentHour,
        currentMinute
      );

      if (schedules[i].motor1Turns > 0) {
        rotateMotor(1, schedules[i].motor1Turns);
        }

      if (schedules[i].motor2Turns > 0) {
        rotateMotor(2, schedules[i].motor2Turns);
        }

      if (schedules[i].motor3Turns > 0) {
        rotateMotor(3, schedules[i].motor3Turns);
        }
    }
  }
}



