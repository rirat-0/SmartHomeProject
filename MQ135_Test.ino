#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define READPIN D4

//folgende vier Parameter anpassen.
//--------------------------------
const char* ssid = "XXXX";
const char* password = "XXXX";

const char* mqtt_topic_publish1 = "sensor/MQ135";

//--------------------------------

//folgende vier Parameter nicht ändern
//--------------------------------
const char* mqtt_server = "XXXX";
const int mqtt_port = XXXX;
const char* mqtt_user = "XXXX";
const char* mqtt_pw = "XXXX";
//--------------------------------

StaticJsonDocument<512> doc_config;
StaticJsonDocument<512> doc_state;

char mqtt_config_data[512];
char mqtt_state_data[512];

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
int val = 0;

uint8_t _pin = 0;
int pinValue;
int Sensorwert;

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

void publishData(int pinValue) {
  doc_state["state"] = round(pinValue);
  serializeJson(doc_state, mqtt_state_data);
  client.publish(mqtt_topic_publish1, mqtt_state_data);
  Serial.println(mqtt_state_data);
}

//Diese Methode wird aufgerufen, sobald es neue Nachrichten gibt, die über das Topic "wetterID/2" versendet wurden.
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
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(READPIN, INPUT);
  Serial.begin(115200);
  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Sensorwert = digitalRead(READPIN);
  Serial.println(Sensorwert);
  pinValue = digitalRead(_pin);

  publishData(pinValue);

  delay(2000);
    //String val_str1 = String(feuchtigkeit1);
    //char val_buff1[val_str1.length() + 1];
    //val_str1.toCharArray(val_buff1, val_str1.length() + 1);
    //client.publish(mqtt_topic_publish1, val_buff1);





    // Publish Char Array (einfachste Methode)
    //client.publish(mqtt_topic_publish, "Hallo Welt");

    //Im folgenden sind Codebeispiele für verschiedene Datentypen, die versendet werden sollen.

    /*
      // Publish String (String muss in ein Char-Array umgewandelt werden)
      String payload = "Hallo Welt";
      //Erzeuge Char Array der Länge der Strings + 1 für den Nullterminator am Ende
      char payload_buff[payload.length() + 1];
      // Wandelt String mit dem Befehl toCharArray(buf, len) in Array um
      payload.toCharArray(payload_buff, payload.length() + 1);
      //uund abschicken
      client.publish(mqtt_topic_publish, payload_buff);

      // Publish JSON (JSON String muss ebenfalls in Char Array umgewandelt werden. Verfahren ist vollkommen analog zu oben)
      ++val;
      String json_payload = "{\"message\":\"Hallo Welt\",\"number\":" + String(val) + "}";
      char json_payload_buff[json_payload.length() + 1];
      json_payload.toCharArray(json_payload_buff, json_payload.length() + 1);
      client.publish(mqtt_topic_publish, json_payload_buff);

      // Publish int (auch hier muss die Zahl erst in einen String umgewandelt werden, damit wir daraus ein Array machen können)
      String val_str = String(val);
      char val_buff[val_str.length() + 1];
      val_str.toCharArray(val_buff, val_str.length() + 1);
      client.publish(mqtt_topic_publish, val_buff);
}
