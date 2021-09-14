#include <ESP8266WiFi.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>

#define DEBUG 0

double WheelSeparation = 0.1 ;
double WheelDiameter = 0.065 ;
double WheelRadius = WheelDiameter/2.0 ;
double WheelCircum = 2 * 3.14 * WheelRadius ;

double MinVel = 0.1   ;
double MaxVel = 0.22  ;

const char* ssid = "RigBetel Labs Guest"; 
const char* password = "starsoforion2020"; 

WiFiClient client;
IPAddress server(192, 168, 1, 31); //ROS core IP adres


class WiFiHardware {
  public:
    WiFiHardware() {};
    void init() {
      client.connect(server, 11411);   
    }
    int read() {
      return client.read();      
    }
    void write(uint8_t* data, int length) {
      for (int i = 0; i < length; i++)
        client.write(data[i]);
    }
    unsigned long time() {
      return millis(); // easy; did this one for you
    }
};



void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if (i == 21) {
    Serial.print("Could not connect to"); Serial.println(ssid);
    while (1) delay(500);
  }
  Serial.print("Ready! Use ");
  Serial.print(WiFi.localIP());
  Serial.println(" to access client");
}




// left motor   - bottom wire - brown   - A-
// left motor   - top wire    - orange  - A+
// right motor  - bottom wire - brown   - B-
// right motor  - top wire    - orange  - B+

int left_pwm  = 5 ;       //  Left Speed 0 to 1023 
int left_dir  = 0 ;       //  Left Direction 1 for Forward, 0 for Backward

int right_pwm  = 4  ;     //  Left Speed 0 to 1023 
int right_dir  = 2  ;     //  Left Direction 1 for Forward, 0 for Backward

int left_speed_pwm  = 255  ;
int right_speed_pwm = 255  ;

int dir_forward   = 1 ;
int dir_backward  = 0 ;

void bot_stop()
{
  digitalWrite( left_dir  , dir_forward ) ;
  digitalWrite( right_dir , dir_forward ) ;
  analogWrite(  left_pwm  , 0 ) ;
  analogWrite(  right_pwm , 0 ) ;
}

double speedlimitLeft ( double left_vel )
{
  double pwmLeft;
  
  if (left_vel >= MaxVel){
    left_vel = MaxVel ;
  }

  else if (left_vel != 0.0 && left_vel <= MinVel ){
    left_vel = MinVel ;
  }

  pwmLeft = ( 583.334 * left_vel ) + 126.666 ;

  return pwmLeft ;
}

double speedlimitRight ( double right_vel )
{
  double pwmRight;
  
  if (right_vel >= MaxVel){
    right_vel = MaxVel ;
  }

  else if (right_vel != 0.0 && right_vel <= MinVel ){
    right_vel = MinVel ;
  }

  pwmRight = ( 583.334 * right_vel ) + 126.666 ;

  return pwmRight ;
  
}

// y = 583.3333333333334x + 126.66666666666666

double forward( double left_speed , double right_speed )
{
  
  digitalWrite( left_dir  , dir_forward ) ;
  digitalWrite( right_dir , dir_forward ) ;
  analogWrite(  left_pwm  , (int) speedlimitLeft(left_speed) ) ;
  analogWrite(  right_pwm , (int) speedlimitRight(right_speed) ) ;
}

double backward( double left_speed , double right_speed )
{
  digitalWrite( left_dir  , dir_backward ) ;
  digitalWrite( right_dir , dir_backward ) ;
  analogWrite(  left_pwm  , (int) speedlimitLeft(left_speed) ) ;
  analogWrite(  right_pwm , (int) speedlimitRight(right_speed) ) ;
}

double left( double left_speed , double right_speed )
{
  digitalWrite( left_dir  , dir_backward ) ;
  digitalWrite( right_dir , dir_forward  ) ;
  analogWrite(  left_pwm  , (int) speedlimitLeft(left_speed) ) ;
  analogWrite(  right_pwm , (int) speedlimitRight(right_speed) ) ;
}

double right( double left_speed , double right_speed )
{
  digitalWrite( left_dir  , dir_forward  ) ;
  digitalWrite( right_dir , dir_backward ) ;
  analogWrite(  left_pwm  , (int) speedlimitLeft(left_speed) ) ;
  analogWrite(  right_pwm , (int) speedlimitRight(right_speed) ) ;
}



void cmd_velCallback( const geometry_msgs::Twist& CVel) {
  //geometry_msgs::Twist twist = twist_msg;
  double linear_vel = CVel.linear.x;
  double angular_vel = CVel.angular.z;
  double right_vel = 0.0;
  double left_vel = 0.0;
  //Serial.print(vel_x);
  //Serial.print("\t");
  //Serial.println(vel_th);

  double rplusl = ( 2 * linear_vel ) / WheelRadius ;
  double rminusl = ( angular_vel * WheelSeparation ) / WheelRadius ;

  double right_omega = ( rplusl + rminusl ) / 2 ;
  double left_omega  = rplusl - right_omega ;
    
  right_vel = right_omega * WheelRadius ;
  left_vel  = left_omega  * WheelRadius ;

  Serial.print(left_vel);
  Serial.print("\t");
  Serial.println(right_vel);

  if (left_vel == 0.0 && right_vel == 0.0){
    bot_stop();
  }
  
  else if (left_vel >= 0.0 && right_vel >= 0.0){
    forward(abs(left_vel), abs(right_vel));
  }
  
  else if (left_vel <= 0.0 && right_vel <= 0.0){
    backward(abs(left_vel), abs(right_vel));
  }
  
  else if (left_vel < 0.0 && right_vel > 0.0){
    left(abs(left_vel), abs(right_vel));
  }
  
  else if (left_vel > 0.0 && right_vel < 0.0){
    right(abs(left_vel), abs(right_vel));
  }
  
  else{
    bot_stop();
  }
 
  
}


ros::NodeHandle_<WiFiHardware> nh;
std_msgs::String str_msg;
geometry_msgs::Twist twist_msg;

ros::Publisher chatter("chatter", &str_msg);
ros::Subscriber <geometry_msgs::Twist> sub("cmd_vel", &cmd_velCallback );

char hello[15] = "ESP8266 alive!";

void setup() {

  Serial.begin(115200);
  setupWiFi();
  Serial.println("WiFi Connected");
  nh.initNode();
  nh.advertise(chatter);
  nh.subscribe(sub);
  
  pinMode ( left_pwm , OUTPUT);
  pinMode ( left_dir , OUTPUT); 
  pinMode ( right_pwm , OUTPUT);
  pinMode ( right_dir , OUTPUT); 

}

void loop() {

  if (nh.connected()) {

      str_msg.data = hello;
      chatter.publish( &str_msg );
    
  } else {
    if(DEBUG) Serial.println("Not Connected");
  }
  
  nh.spinOnce();
  delay(1);

}
