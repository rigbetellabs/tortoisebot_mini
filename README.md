# TortoiseBot Mini

## 1. Installation:

### 1.1 Download Arduino IDE:

Download the latest version Arduino IDE from https://www.arduino.cc/en/software

### 1.2 Setup ESP board in Arduino IDE:

Follow the following tutorial to add ESP board in your Arduino IDE - https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/

Simply go to Files > Preferences and add the following line in the Additional Board Managers URLs:
```
https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
```
After that go into Tools > Board > Boards Manager and search for "esp8266" and install the ESP8266 board by ESP8266 Community. That's it!

### 1.3 Install ROS Serial Library for Arduino on Ubuntu:
```
sudo apt-get install ros-melodic-rosserial-arduino
sudo apt-get install ros-melodic-rosserial
```

### 1.4 Clone the TortoiseBot Mini Repo:
Clone the TortoiseBot Mini repo into your src folder of your catkin workspace
```
git clone https://github.com/rigbetellabs/tortoisebot_mini
```

## 2. Setup:

### 2.1 Change Wi-Fi's SSID and Password:

Open the [espros_cmd_vel](https://github.com/rigbetellabs/tortoisebot_mini/blob/506f832b9e0385ce1e38ce5b1219167dbe5a9f84/esp/espros_cmd_vel/espros_cmd_vel.ino) code in Arduino IDE and change the SSID and Password on line number 10 & 11 to that of your Wi-Fi Connection.

### 2.2 Change the ROS Master IP Address:

Check the IP Address of your ROS Master PC using ifconfig command and put that in line number 14 of [espros_cmd_vel](https://github.com/rigbetellabs/tortoisebot_mini/blob/506f832b9e0385ce1e38ce5b1219167dbe5a9f84/esp/espros_cmd_vel/espros_cmd_vel.ino) code with commas(,) instead of dot(.)

### 2.3 Check Board and Port:

Make sure the switch on motor driver board is turned off and battery is not connected and then connect your ESP8266 board to your computer using USB type B cable. Make sure the board is selected to "NodeMCU 1.0 (ESP-12E Module)" in Arduino IDE and select the appropriate Port as well.

### 2.4 Upload the Code:

Upload the [espros_cmd_vel](https://github.com/rigbetellabs/tortoisebot_mini/blob/506f832b9e0385ce1e38ce5b1219167dbe5a9f84/esp/espros_cmd_vel/espros_cmd_vel.ino) code on your ESP8266 Node MCU board.

### 2.5 Connection:

Once code is Successfully uploaded, disconnect the USB Cable and connect the DC Jack of Battery to Motor Driver Board. <br>
DO NOT TURN ON THE ROBOT YET!

## 3. Demo:

### 3.1 Running ROS Master:

Launch the [tortoisebot_mini.launch](https://github.com/rigbetellabs/tortoisebot_mini/blob/506f832b9e0385ce1e38ce5b1219167dbe5a9f84/launch/tortoisebot_mini.launch) file on your Master PC to start ROS Master along with ROS Serial Node.
```
roslaunch tortoisebot_mini tortoisebot_mini.launch
```
It will prompt a INFO message saying "Waiting for socket connections on port 11411 waiting for socket connection"<br>

### 3.2 Turning on Robot:

Once the above INFO message is popped up, then you can turn on your Robot upon which you will see that different ROS topics start automatically.

### 3.3 Testing out Teleop:

Keep the previous terminal running ad on a new terminal type the following command to start the teleoperation node.
```
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
```
Now you can control the robot using teleoperation commands.

## References:
- https://github.com/agnunez/espros
- https://github.com/RoboTakao/NX15A
