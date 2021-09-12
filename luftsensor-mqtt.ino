#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "SparkFunCCS811.h" 

#define CCS811_ADDR 0x5B //Default I2C Address

CCS811 airSensor(CCS811_ADDR);

//--------------------------------
const char* mqtt_topic_publish1 = "G05/airsensor/co2";
const char* mqtt_topic_publish2 = "G05/airsensor/tvoc";
const char* mqtt_topic_subscribe = "G05/OLEDshield";
//WLAN Zugangsdaten eintragen:
const char* ssid = "XXXXX";
const char* password = "XXXXX";
//MQTT Zugangsdaten eintragen:
const char* mqtt_server = "XXXXX";
const int mqtt_port = XXXXX;
const char* mqtt_user = "XXXXX";
const char* mqtt_pw = "XXXXX";
//--------------------------------

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
int val = 0;

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID: Client ID MUSS inviduell sein, da der MQTT Broker nicht mehrere Clients mit derselben ID bedienen kann
    String clientId = "Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pw)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe(mqtt_topic_subscribe);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("CCS811 Semesterprojekt G05");

  Wire.begin(); //Inialize I2C Hardware

  if (airSensor.begin() == false)
  {
    Serial.print("CCS811 error. Please check wiring. Freezing...");
    while (1)
      ;
  }

  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
}

void loop()
{
if (!client.connected()) {
    reconnect();
  }
  client.loop();

if (millis() - lastMsg > 5000) {
    lastMsg = millis();

    
  
  //Check to see if data is ready with .dataAvailable()
  if (airSensor.dataAvailable())
  {
    //If so, have the sensor read and calculate the results.
    //Get them later
    airSensor.readAlgorithmResults();

    Serial.print("CO2[");
    //Returns calculated CO2 reading
    Serial.print(airSensor.getCO2());
    Serial.print("] tVOC[");
    //Returns calculated TVOC reading
    Serial.print(airSensor.getTVOC());
    Serial.print("] millis[");
    //Display the time since program start
    Serial.print(millis());
    Serial.print("]");
    Serial.println();
  }
  String val_str = String(airSensor.getCO2());
    char val_buff[val_str.length() + 1];
    val_str.toCharArray(val_buff, val_str.length() + 1);
    client.publish(mqtt_topic_publish1, val_buff);

   String val_str2 = String(airSensor.getTVOC());
    char val_buff2[val_str2.length() + 1];
    val_str2.toCharArray(val_buff2, val_str2.length() + 1);
    client.publish(mqtt_topic_publish2, val_buff2);

  delay(100); //Don't spam the I2C bus
}
}
