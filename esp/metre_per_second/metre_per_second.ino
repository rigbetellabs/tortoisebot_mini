#include <ESP8266WiFi.h>

// left motor   - bottom wire - brown   - A-
// left motor   - top wire    - orange  - A+
// right motor  - bottom wire - brown   - B-
// right motor  - top wire    - orange  - B+

int left_pwm  = 5 ;       //  Left Speed 0 to 1023 
int left_dir  = 0 ;       //  Left Direction 1 for Forward, 0 for Backward

int right_pwm  = 4  ;     //  Left Speed 0 to 1023 
int right_dir  = 2  ;     //  Left Direction 1 for Forward, 0 for Backward

int left_speed_pwm  = 185  ;
int right_speed_pwm = 185  ;

int dir_forward   = 1 ;
int dir_backward  = 0 ;

void bot_stop()
{
  digitalWrite( left_dir  , dir_forward ) ;
  digitalWrite( right_dir , dir_forward ) ;
  analogWrite(  left_pwm  , 0 ) ;
  analogWrite(  right_pwm , 0 ) ;
}

void forward()
{
  digitalWrite( left_dir  , dir_forward ) ;
  digitalWrite( right_dir , dir_forward ) ;
  analogWrite(  left_pwm  , left_speed_pwm ) ;
  analogWrite(  right_pwm , right_speed_pwm ) ;
}

void backward()
{
  digitalWrite( left_dir  , dir_backward ) ;
  digitalWrite( right_dir , dir_backward ) ;
  analogWrite(  left_pwm  , left_speed_pwm ) ;
  analogWrite(  right_pwm , right_speed_pwm ) ;
}

void left()
{
  digitalWrite( left_dir  , dir_backward ) ;
  digitalWrite( right_dir , dir_forward  ) ;
  analogWrite(  left_pwm  , left_speed_pwm ) ;
  analogWrite(  right_pwm , right_speed_pwm ) ;
}

void right()
{
  digitalWrite( left_dir  , dir_forward  ) ;
  digitalWrite( right_dir , dir_backward ) ;
  analogWrite(  left_pwm  , left_speed_pwm ) ;
  analogWrite(  right_pwm , right_speed_pwm ) ;
}


void setup() {

  Serial.begin(115200);
  pinMode ( left_pwm , OUTPUT);
  pinMode ( left_dir , OUTPUT); 
  pinMode ( right_pwm , OUTPUT);
  pinMode ( right_dir , OUTPUT); 

  bot_stop(); 
  
  delay(5000);
  
  forward();

  delay(1000);

  bot_stop();  

}

void loop() {

  bot_stop();

}
