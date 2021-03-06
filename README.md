# SmartHomeProject

## Setup and installation of software

After printing the provided cases, start by compiling and uploading given code to their respective D1 Mini.
To connect the D1 Minis to your local network change the X's at _const char* ssid_ to the name of the local
network and below that change the X's to the network's password. In the following lines are the respective 
mqtt-inputs. Depending on your chosen node-red-instance change the ports, etc. to the ones you chose.

Next up is the setup of the mqtt-protocol. The first step here is choosing between a public Node-Red server
or installing a private instance on a server of your own, like a raspberry pi. If you chose the latter the 
following guide may be helpful: https://nodered.org/docs/getting-started/raspberrypi .

For this project you will want to use three mqtt-input nodes. Two are for the sensors within the CCS811 and 
one for the OLED-Shield. The mqtt-nodes for the sensors are only for debugging and testing of the outputs of
the CCS811, making them optional. The third input-node is not optional since it will be used to connect to a
Discord-node which will make a bot send a message to a dedicated channel on a server.

To make a Discord bot follow the given guide to make a bot account: 
https://www.freecodecamp.org/news/create-a-discord-bot-with-python/
To use this bot accountant in node-red, click on the send-message-node and add the token you can redeem in 
the Discord API. If you do not already have a private Discord server, this guide can help you create one:
https://blog.discord.com/starting-your-first-discord-server-4dcacda8dad5

Next up you need the channel ID of the Discord channel, in which the discord bot will send the messages.
To get this ID, enable developer mode in Discord. This can be done by going to user setting, then going to 
Advanced and enabling the Developer Mode. Now you can right-click on any channel and have the option to
copy its ID. After choosing the channel for your bot and getting the ID, paste it in the send-message-node.

All there is left to do is deploying the node-red instance.



## Libraries used

CCS811 sensor:
Wire.h ??? enables communication via the I2C protocol
WifiManager by tzapu,tablatronix ??? enables WiFi connection
PubSubClient by Nick O???Leary- enables MQTT communication
SparkFun CCS811 Arduino Library ??? enables control of the sensor


OLED shield:
Adafruit BusIO ??? enables communication between OLED shield and a microcontroller
Adafruit_GFX.h (version 1.2.3) ??? enables e.g. different fonts / graphic library
Adafruit_SSD1306.h (version 1.1.2) ??? enables communication with the OLED Shield
