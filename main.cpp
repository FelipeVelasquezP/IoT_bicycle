#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient esp32Client;
PubSubClient mqttClient(esp32Client);

const char* ssid     = "RED";
const char* password = "password";

char *server = "server";
int port = 1883;

int ledpin= 13; 
int sensorPin=33;

int var = 0;
int ledval = 0;
int outSensor = 0;
char datos[400];
String resultS = "";
float temp=0.0;

void wifiInit() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    
  }


void callback(char* topic, byte* payload, unsigned int length) {


  char payload_string[length + 1];

  int resultI;
  memcpy(payload_string, payload, length);
  payload_string[length] = '\0';
  resultI = atoi(payload_string);
  
  var = resultI;

  resultS = "";

 
  for (int i=0;i<length;i++) {
    resultS= resultS + (char)payload[i];
  }
  Serial.println();
}



void reconnect() {
  
  while (!mqttClient.connected()) {

    if (mqttClient.connect("arduinoClient")) {
 
      mqttClient.subscribe("Entrada/01");
    } else {
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(ledpin,OUTPUT);
  Serial.begin(115200);
  delay(10);
  
  wifiInit();
  mqttClient.setServer(server, port);
  mqttClient.setCallback(callback);
  digitalWrite(ledpin,HIGH);

}

void loop()
{

   if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  outSensor = analogRead(sensorPin);
  temp=((outSensor * 3.3 )/1024)+6.14;
  sprintf(datos, "%f", temp);
  mqttClient.publish("temp",datos);
  delay(5000);
  
}
