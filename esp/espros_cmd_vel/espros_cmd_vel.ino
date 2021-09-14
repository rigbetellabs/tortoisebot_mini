#include  <ESP8266WiFi.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>

#define DEBUG 0

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



void cmd_velCallback( const geometry_msgs::Twist& CVel) {
  //geometry_msgs::Twist twist = twist_msg;
  double vel_x = CVel.linear.x;
  double vel_th = CVel.angular.z;
  //Serial.print(vel_x);
  //Serial.print("\t");
  //Serial.println(vel_th);

  if ( vel_x == 0 && vel_th == 0 ){
    //Serial.println("stop");
    bot_stop();
  }

  else if ( vel_x > 0 && vel_th == 0 ){
    //Serial.println("forward");
    forward();
  }
  
  else if ( vel_x < 0 && vel_th == 0 ){
    //Serial.println("backward");
    backward();
  }

  else if ( vel_x == 0 && vel_th > 0 ){
    //Serial.println("left");
    left();
  }
  
  else if ( vel_x == 0 && vel_th < 0 ){
    //Serial.println("right");
    right();
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
