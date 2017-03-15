/*
   ETEC Capstone project
   developed by Thoomas Allen
   packet design
   [status_byte/command_byte, pitch_byte, yaw_byte]
*/
const byte j_x = 2;
const byte j_y = 1;
const byte pwr_led = 3;
const byte idle_led = 4;
const byte run_led = 6;
const byte err_led = 5;
      char cmd[10];
String incoming_msg = "";

const int timeout = 1500;
const int polling_rate = 100; //lower this number to increase the transmit rate/ make controls smoother

unsigned long next_tx = millis();
unsigned long last_rx = millis();

boolean connection_lost = false;

void setup() {
  pinMode(j_x, INPUT);
  pinMode(j_y, INPUT);
  Serial.begin(9600);
  digitalWrite(pwr_led, HIGH);
  digitalWrite(idle_led, HIGH);
  digitalWrite(run_led, LOW);
  digitalWrite(err_led, LOW);
}

void loop() {
  //timed loop, dont spam messages
  while (millis() < next_tx) {
    if (Serial.available() > 1)
    {
      delay(10);

      int i = 0;
      while (Serial.available() > 1)
      {
        cmd[i] = Serial.read();
       // cmd[i++] = '\0';
        i = i+1;
      }
      last_rx = millis();
      connection_lost = false;
    }

  }
  if(!connection_lost){
      char b_2 = map(analogRead(j_x),0,1023,0,255);
  char b_3 = map(analogRead(j_y),0,1023,0,255);
  //Serial.write(0);
  Serial.write(b_2);
  Serial.write(b_3);}
if(millis() > last_rx + timeout)
{
  cmd[0] = 'f';
  connection_lost = true;
}
//Serial.write(analogRead(j_x));
//Serial.write(analogRead(j_y));
  if (cmd[0] == '1') //idle
  {
    digitalWrite(idle_led, HIGH);
    digitalWrite(run_led, LOW);
    digitalWrite(err_led, LOW);
  }
  if (cmd[0] == '2')//running
  {
    digitalWrite(idle_led, LOW);
    digitalWrite(run_led, HIGH);
    digitalWrite(err_led, LOW);
  }
  if (cmd[0] == 'f')//error
  {
    digitalWrite(idle_led, LOW);
    digitalWrite(run_led, LOW);
    digitalWrite(err_led, HIGH);
  }
  next_tx = millis() + polling_rate;
}
