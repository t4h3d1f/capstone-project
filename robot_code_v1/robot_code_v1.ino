/*
   ETEC Capstone project
   developed by Thoomas Allen

   packet design
   [status_byte/command_byte, pitch_byte, yaw_byte]
*/
#define IDLE '1' //these are used for commands and reports between the robot and controller
#define COMM_FAIL 'f'
#define CODE_EXECUTE '2'
#define PITCH 0x01
#define YAW 0x02
#define THROTTLE 0x03
/*
   min pitch 123
   140 is centered
   max pitch 160
*/

#include<Servo.h>

Servo pitchServ, yawServ, esc;

unsigned long cnt;
long time_since_last_msg = 0;

const int throttle_max = 1200;

int j_x = 140;
int j_y = 90;
int throttle = 1100;
String incoming_msg = "";

boolean isRunning = false;

void setup() {
  pitchServ.attach(11);
  yawServ.attach(10);
  esc.attach(9);
  Serial.begin(9600);
  pitchServ.write(140);
  yawServ.write(80);
  esc.write(1000);
}

void loop() {
  time_since_last_msg = millis() - cnt;

  if (time_since_last_msg >= 1000)
  {
    Serial.write(IDLE);
    cnt = millis(); 
    j_x = 140;
    j_y = 90;
    throttle = 1000;
  }
  if (Serial.available() > 1)
  {
  //  Serial.print("recieving");
    char cmd[10];
    int i = 0;
    delay(10);
    while (Serial.available() > 0)
    {
      cmd[i] = Serial.read();
      //cmd[i++] = '\0';
      i = i+1;
    }
    incoming_msg = cmd;

//Serial.write(cmd);
//    char b_01 = incoming_msg & 0xF00;
//    char b_02 = (incoming_msg << 8) & 0xF0;
//    char b_03 = (incoming_msg << 16) & 0xF;

    char b_01 = 0;
    byte b_02 = cmd[0];
    byte b_03 = cmd[1];
    if (b_01 == 0)
      Serial.print(CODE_EXECUTE);
    if (b_03 == 0)
      Serial.print(COMM_FAIL);
          if (b_02 == 0)
      Serial.print(COMM_FAIL);
  
  j_y = map(b_03, 0, 255, 80, 110);
  j_x = map(b_02, 0, 255, 123, 157);
  throttle = abs((b_02 - 128));
  throttle = map (throttle, 0, 128, 1000, throttle_max);
  cnt = millis();
  }
  pitchServ.write(j_x);
  yawServ.write(j_y);
  esc.write(throttle);

  //pitchServ.write(125);
  //yawServ.write(80);
  //esc.write(1100);
  //delay(1000);
  //pitchServ.write(160);
  //yawServ.write(100);
  //esc.write(1200);
  //delay(1000);
  //pitchServ.write(145);
  //yawServ.write(90);
  //esc.write(1000);
  //delay(1000);

}
