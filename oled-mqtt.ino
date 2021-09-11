#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

const char* mqtt_topic_subscribe = "G05/airsensor/+";
const char* mqtt_topic_publish = "G05/OLEDshield";
const char* ssid = "XXXXX";
const char* password = "XXXXX";
const char* mqtt_server = "XXXXX";
const int mqtt_port = 2883;
const char* mqtt_user = "XXXXX";
const char* mqtt_pw = "XXXXX";


  int co2 = 0;
  String co2_ausgabe = "";

  int tvoc = 0;
  String tvoc_ausgabe = "";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
int val = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
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


void callback(char* topic, byte* payload, unsigned int length) {
  char receivedPayload[length];
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    receivedPayload[i] = (char) payload[i];
  }
  Serial.println();


  if (strcmp(topic, "G05/airsensor/co2") == 0) {
    Serial.println("Topic erkannt");
    co2 = atoi(receivedPayload);
    
    if(co2 <= 800) {
      co2_ausgabe = ":-)";
    } else if (co2 <= 1000) {
      co2_ausgabe = ":-|";
    } else if (co2 <= 1400) {
      co2_ausgabe = ":-/";
    } else if (co2 > 1400) {
      co2_ausgabe = ":-(";
    }
  }

  
  if (strcmp(topic, "G05/airsensor/tvoc") == 0) {
    Serial.println("Topic erkannt");
    tvoc = atoi(receivedPayload);
  }

      
      

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


void setup()   {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay(); 
}

void loop() {
    if (!client.connected()) {
    reconnect();
  }
  client.loop();

      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(32,8);
      display.println(co2_ausgabe);
      display.setCursor(32,17);
      String tmp_co2 = String(co2);
      display.println("Co2: " + tmp_co2);
      display.setCursor(32,25);
      String tmp_tvoc = String(tvoc);
      display.println("TVOC: " + tmp_tvoc);
      display.display();
      delay(2000);

if (millis() - lastMsg > 5000) {
    lastMsg = millis();
      if (co2 >= 4000) {
        client.publish(mqtt_topic_publish, "FEUER!");
      }
}

}
