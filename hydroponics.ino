#include "credentials.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = CRED_SSID;
const char* password = CRED_PASSWORD;
const char* mqtt_server = CRED_MQTT_SERVER;
const int mqtt_port = CRED_MQTT_PORT;
const char* mqtt_user = CRED_MQTT_USER;
const char* mqtt_pwd = CRED_MQTT_PASSWD;
const char* clientId = CRED_CLIENT_ID;
const char* pub_topic = CRED_PUB_TOPIC;

float sleepTimeInMin = 0.117; 

WiFiClient espClient;

#define DHTPIN D1
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

float temp = 0.0;
float hum = 0.0;

#define PHOTORESISTORPIN A0

int lightIntensity = 0;

#define PUMPPIN D7

void connectWifi(){
  Serial.println();  
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  int count = 0 ;
  while (WiFi.status() != WL_CONNECTED && count <= 15) {
    delay(1000);
    Serial.print(".");
    count++;
  }

  if(WiFi.status() != WL_CONNECTED){
    handleError("Failed to connect to Wifi.");
  } else{
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();  
  }
}

void readDhtSensor (){
  float newTemp = dht.readTemperature();
  float newHum = dht.readHumidity();

  if(isnan(newTemp)||isnan(newHum)){
    handleError("Failed to read from DHT sensor.");
  }else{
    temp = newTemp;
    hum = newHum;

    Serial.println();
    Serial.println("Temperature: ");
    Serial.println(temp);
    Serial.println();
    Serial.println("Humidity: ");
    Serial.println(hum);
  }
}

void readPhotoResistor (){
 lightIntensity =  constrain(map(analogRead(PHOTORESISTORPIN), 0, 920, 0, 100), 0, 100);
 Serial.println();
 Serial.println("Light intensity: ");
 Serial.println(lightIntensity);
}


void handleError(char* errorText){
  Serial.println();
  Serial.println("Error: ");
  Serial.print(errorText);
}

void pumpWater(){
  digitalWrite(PUMPPIN, HIGH);
  Serial.println("pumping");
  delay(5000);
  digitalWrite(PUMPPIN,LOW);
  Serial.println("stopped pumping");
}

void setup() {
  pinMode(PUMPPIN,OUTPUT);
  Serial.begin(9600);
  delay(200);

  
  connectWifi();
  
  readDhtSensor();
  readPhotoResistor();
  pumpWater();

  ESP.deepSleep(sleepTimeInMin * 6e7);
}

void loop() {}
