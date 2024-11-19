#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "SSD1306Wire.h"


// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "JVAVM/Pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "JVAVM/Sub"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

void publishMessage();
void messageHandler(String &topic, String &payload);

SSD1306Wire display(0x3c, 5, 4);

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  client.subscribe("sala12/sensors");
  client.subscribe("energy/power");

  Serial.println("AWS IoT Connected!");
}

int pinButtonP = 32;
int pinButtonN = 33;
int pinButtonSend = 25;
int pinPont = 35;

int buttonStateP;
int buttonStateN;
int buttonSend;
int pontState;
int pTemp;
int atualTemp;
int tempState = -1;

int counter;

void drawProgressBarDemo() {
  display.drawProgressBar(0, 32, 120, 10, pontState);

  pTemp = map(pontState, 0, 100, 19, 30);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(atualTemp) + "ºC -> " + String(pTemp) + "ºC");
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  //doc["sensor_Agradabilidade"] = tempState;
  doc["temperature"] = pTemp;

  char jsonBuffer[400];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void publishDesligar()
{

  client.publish("energy/power", "0");
}

void publishLigar()
{
  client.publish("energy/power", "1");
}

void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  
  // Tente desserializar a mensagem JSON
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  // Extraia apenas os valores que você deseja
  if (doc.containsKey("temp")) {
    atualTemp = doc["temp"];
    Serial.print("Temperatura recebida: ");
    Serial.println(atualTemp);
  } else {
    Serial.println("Chave 'temp' não encontrada na mensagem.");
  }

  // Extraia outros dados, se necessário
  if (doc.containsKey("otherData")) {
    String otherData = doc["otherData"];
    Serial.println("Other Data: " + otherData);
  }

  if (doc.containsKey("status")) {
    String status = doc["status"];
    Serial.println("Status: " + status);
  }
}

void setup() {
  Serial.begin(9600);

  display.init();

  // display.flipScreenVertically();

  display.setContrast(255);
  drawProgressBarDemo();
  display.display();
  
  pinMode(pinButtonP, INPUT_PULLUP);
  pinMode(pinButtonN, INPUT_PULLUP);
  pinMode(pinButtonSend, INPUT_PULLUP);

  connectAWS();
}

void loop() {
  buttonStateP = digitalRead(pinButtonP);
  buttonStateN = digitalRead(pinButtonN);
  pontState = map(analogRead(pinPont), 0, 4095, 0, 100);

  if(buttonStateP){
    tempState = 1;
  }else if(buttonStateN){
    tempState = 0;
  }

  display.clear();
  drawProgressBarDemo();
  display.display();

  buttonSend = digitalRead(pinButtonSend);

  if(!buttonStateN){
    publishDesligar();
    delay(2000);
  }else if(!buttonStateP){
    publishLigar();
    delay(2000);
  }else if(!buttonSend){
    publishMessage();
    delay(2000);
  }

  client.loop();
  delay(1000);
}