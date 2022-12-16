#include <ros.h>
#include <std_msgs/UInt8.h>
#include <std_msgs/Bool.h>
#include <WiFi.h>

#define DEBUG 0

const char* ssid = "Rigbetel Labs HQ";
const char* password = "Starsoforion2020";

WiFiClient client;
IPAddress server(192,168,0,223);

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
      for (int i = 0; i < length ; i++)
        client.write(data[i]);
    }
    unsigned long time() {
      return millis();
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

int led_pin = 2;

void ledCallback (const std_msgs::Bool& led_ctrl)
{
  bool rdir = led_ctrl.data ;
  digitalWrite(led_pin, rdir);
  Serial.println();
  Serial.print("Led_status : ");
  Serial.print(rdir);
  Serial.println();
  
}

ros::NodeHandle_<WiFiHardware> nh;
std_msgs::Bool bool_msg;

ros::Subscriber <std_msgs::Bool> led_sub ("/led", &ledCallback );

void setup() {
  
  Serial.begin(115200);
  setupWiFi();
  nh.initNode();
  nh.subscribe(led_sub);

  pinMode (led_pin, OUTPUT);

}

void loop() {

  nh.spinOnce();
  delay(1);

}
