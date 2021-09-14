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

#### References:
- https://github.com/agnunez/espros
- https://github.com/RoboTakao/NX15A
