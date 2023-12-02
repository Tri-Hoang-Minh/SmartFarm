#include <SoftwareSerial.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
SoftwareSerial mySerial(16, 17);
// Định nghĩa struct garden chứa các biến float và int
struct GardenData {
  String airHumi;
  String airTemp; 
  String soilHumi;
  String light;  
};
struct ControlGarden {
  String ID;
  String Device;
  String Status;
};
GardenData garden1;  
GardenData garden2;  
ControlGarden controlDevice1;
ControlGarden controlDevice2;
//
const unsigned long sensorInterval = 10000; // Đọc cảm biến mỗi 10 giây
// Update these with values suitable for your network.
// const char* ssid = "Galaxy A31D804";
// const char* password = "20122002";
const char* ssid = "Realme";
const char* password = "13042002";
// const char* ssid = "MANG DAY KTX G4-1006";
// const char* password = "kkkkkkkk";
//const char* mqtt_server = "io.adafruit.com";
// const char* mqtt_server = "192.168.31.190";
//Khair
// const char* mqtt_server = "192.168.252.114";
const char* mqtt_server = "192.168.194.62";
// const char* mqtt_server = "test.mosquitto.org";
// String jsonString;
String jsonAirHumi1;
String jsonAirTemp1;
String jsonSoilHumi1;
String jsonLight1;
//
String jsonAirHumi2;
String jsonAirTemp2;
String jsonSoilHumi2;
String jsonLight2;

int timer = 0;
#define mqtt_port 1883
#define MQTT_USER "cdanh2012"
#define MQTT_PASSWORD "aio_MLyn47jyUrd36O01rb9KHIiJdr60"

// #define MQTT_SERIAL_PUBLISH_CH "cdanh2012/feeds/mqtt-publich"
// #define MQTT_SERIAL_PUBLISH_CH_AIRTEMP  "cdanh2012/feeds/airtemp"
// #define MQTT_SERIAL_PUBLISH_CH_AIRHUMI  "cdanh2012/feeds/airhumi"
// #define MQTT_SERIAL_PUBLISH_CH_SOIL     "cdanh2012/feeds/soilhumi"
// #define MQTT_SERIAL_PUBLISH_CH_LIGHT    "cdanh2012/feeds/light"
#define MQTT_SERIAL_PUBLISH_CH_AIRTEMP  "feeds/Temp"
#define MQTT_SERIAL_PUBLISH_CH_AIRHUMI  "feeds/Humi"
#define MQTT_SERIAL_PUBLISH_CH_SOIL     "feeds/Mois"
#define MQTT_SERIAL_PUBLISH_CH_LIGHT    "feeds/Light"
// #define MQTT_SERIAL_RECEIVER_CH "feed5"
#define MQTT_SERIAL_RECEIVER_CH1 "feeds/DeviceGarden1"
#define MQTT_SERIAL_RECEIVER_CH2 "feeds/DeviceGarden2"
// #define MQTT_SERIAL_RECEIVER_CH "cdanh2012/feeds/mqtt-publich"

WiFiClient wifiClient;
PubSubClient client(wifiClient);

//
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

//
void reconnectGarden1() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      // client.publish(MQTT_SERIAL_PUBLISH_CH_AIRHUMI,  jsonAirHumi1.c_str());  // Chuyển đổi sang const char *
      // client.publish(MQTT_SERIAL_PUBLISH_CH_AIRTEMP,  jsonAirTemp1.c_str());  // Chuyển đổi sang const char *
      // client.publish(MQTT_SERIAL_PUBLISH_CH_SOIL,     jsonSoilHumi1.c_str());  // Chuyển đổi sang const char *
      // client.publish(MQTT_SERIAL_PUBLISH_CH_LIGHT,    jsonLight1.c_str());  // Chuyển đổi sang const char *
      // ... and resubscribe
      client.subscribe(MQTT_SERIAL_RECEIVER_CH1);
      // client.subscribe(MQTT_SERIAL_RECEIVER_CH2);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void reconnectGarden2() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      // client.publish(MQTT_SERIAL_PUBLISH_CH_AIRHUMI, jsonAirHumi2.c_str());  // Chuyển đổi sang const char *
      // client.publish(MQTT_SERIAL_PUBLISH_CH_AIRTEMP, jsonAirTemp2.c_str());  // Chuyển đổi sang const char *
      // client.publish(MQTT_SERIAL_PUBLISH_CH_SOIL, jsonSoilHumi2.c_str());  // Chuyển đổi sang const char *
      // client.publish(MQTT_SERIAL_PUBLISH_CH_LIGHT, jsonLight2.c_str());  // Chuyển đổi sang const char *
      //client.publish(MQTT_SERIAL_PUBLISH_CH, "Helloooo"); 
      // ... and resubscribe
      // client.subscribe(MQTT_SERIAL_RECEIVER_CH);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void callback(char* topic, byte *payload, unsigned int length) {
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);
    Serial.print("data:");     
    Serial.write(payload, length);
    Serial.println();
    String ControlDevice ="";
    ControlDevice = String((char*)payload, length);
    HandelControl(ControlDevice);

}
void preJsonGarden1(){
  // Createjson();
  CreatejsonAirTemp(1);
  Serial.println("");
  CreatejsonAirHumi(1);
  Serial.println("");
  CreatejsonLight(1);
  Serial.println("");
  CreatejsonSoilHumi(1);
  Serial.println("");
}
void preJsonGarden2(){
  // Createjson();
  CreatejsonAirTemp(2);
  CreatejsonAirHumi(2);
  CreatejsonLight(2);
  CreatejsonSoilHumi(2);
}
void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  Serial.setTimeout(500);  // Set time out for 
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnectGarden1();
  reconnectGarden2();
}
      
// }
//AirTemp
DynamicJsonDocument jsonData(1024);
void CreatejsonAirTemp(int jsonGarden){
   jsonData.clear();
  if(jsonGarden == 1) {
    jsonData["station_id"] = "garden_0001";
    jsonData["station_name"] = "Garden 0001";
    jsonData["gps_longitude"] = 106.89;
    jsonData["gps_latitude"] = 10.5;
    JsonArray sensors = jsonData.createNestedArray("sensors");

    // Add sensor objects to the array
    JsonObject tempSensor = sensors.createNestedObject();
    tempSensor["sensor_id"] = "temp_0001";
    tempSensor["sensor_name"] = "Temperature";
    tempSensor["sensor_value"] = garden1.airTemp;
    tempSensor["sensor_unit"] = "C";
    serializeJson(jsonData, jsonAirTemp1);
    // Print the JSON string to Serial for debugging
    serializeJson(jsonData, Serial);
  }
   if(jsonGarden == 2) {
    jsonData["station_id"] = "garden_0002";
    jsonData["station_name"] = "Garden 0002";
    jsonData["gps_longitude"] = 107.89;
    jsonData["gps_latitude"] = 11.5;
    JsonArray sensors = jsonData.createNestedArray("sensors");

    // Add sensor objects to the array
    JsonObject tempSensor = sensors.createNestedObject();
    tempSensor["sensor_id"] = "temp_0002";
    tempSensor["sensor_name"] = "Temperature";
    tempSensor["sensor_value"] = garden2.airTemp;
    tempSensor["sensor_unit"] = "C";
    serializeJson(jsonData, jsonAirTemp2);
    // Print the JSON string to Serial for debugging
    serializeJson(jsonData, Serial);
  }
}
//AirHumi
void CreatejsonAirHumi(int jsonGarden){
   jsonData.clear();
  if(jsonGarden == 1) {
    jsonData["station_id"] = "garden_0001";
    jsonData["station_name"] = "Garden 0001";
    jsonData["gps_longitude"] = 106.89;
    jsonData["gps_latitude"] = 10.5;
    JsonArray sensors = jsonData.createNestedArray("sensors");

    // Add sensor objects to the array
    JsonObject humiSensor = sensors.createNestedObject();
    humiSensor["sensor_id"] = "humi_0001";
    humiSensor["sensor_name"] = "Humidity";
    humiSensor["sensor_value"] = garden1.airHumi;
    humiSensor["sensor_unit"] = "%";

    serializeJson(jsonData, jsonAirHumi1);
    // Print the JSON string to Serial for debugging
    serializeJson(jsonData, Serial);
  }
  if(jsonGarden == 2) {
    jsonData["station_id"] = "garden_0002";
    jsonData["station_name"] = "Garden 0002";
    jsonData["gps_longitude"] = 107.89;
    jsonData["gps_latitude"] = 11.5;
    JsonArray sensors = jsonData.createNestedArray("sensors");

    // Add sensor objects to the array
    JsonObject humiSensor = sensors.createNestedObject();
    humiSensor["sensor_id"] = "humi_0002";
    humiSensor["sensor_name"] = "Humidity";
    humiSensor["sensor_value"] = garden2.airHumi;
    humiSensor["sensor_unit"] = "%";

    serializeJson(jsonData, jsonAirHumi2);
    // Print the JSON string to Serial for debugging
    serializeJson(jsonData, Serial);
  }
}
void CreatejsonSoilHumi(int jsonGarden){
   jsonData.clear();
  if(jsonGarden == 1) {
    jsonData["station_id"] = "garden_0001";
    jsonData["station_name"] = "Garden 0001";
    jsonData["gps_longitude"] = 106.89;
    jsonData["gps_latitude"] = 10.5;
    JsonArray sensors = jsonData.createNestedArray("sensors");

    // Add sensor objects to the array
    JsonObject soilSensor = sensors.createNestedObject();
    soilSensor["sensor_id"] = "mois_0001";
    soilSensor["sensor_name"] = "Soil";
    soilSensor["sensor_value"] = garden1.soilHumi;
    soilSensor["sensor_unit"] = "%";
    serializeJson(jsonData, jsonSoilHumi1);
    // Print the JSON string to Serial for debugging
    serializeJson(jsonData, Serial);
  }
  if(jsonGarden == 2) {
    jsonData["station_id"] = "garden_0002";
    jsonData["station_name"] = "Garden 0002";
    jsonData["gps_longitude"] = 107.89;
    jsonData["gps_latitude"] = 11.5;
    JsonArray sensors = jsonData.createNestedArray("sensors");

    // Add sensor objects to the array
    JsonObject soilSensor = sensors.createNestedObject();
    soilSensor["sensor_id"] = "mois_0002";
    soilSensor["sensor_name"] = "Soil";
    soilSensor["sensor_value"] = garden2.soilHumi;
    soilSensor["sensor_unit"] = "%";
    serializeJson(jsonData, jsonSoilHumi2);
    // Print the JSON string to Serial for debugging
    serializeJson(jsonData, Serial);
  }
}
void CreatejsonLight(int jsonGarden){
   jsonData.clear();
  if(jsonGarden == 1) {
    jsonData["station_id"] = "garden_0001";
    jsonData["station_name"] = "Garden 0001";
    jsonData["gps_longitude"] = 106.89;
    jsonData["gps_latitude"] = 10.5;
    JsonArray sensors = jsonData.createNestedArray("sensors");
    // Add sensor objects to the array
    JsonObject illumSensor = sensors.createNestedObject();
    illumSensor["sensor_id"] = "light_0001";
    illumSensor["sensor_name"] = "Illuminance";
    illumSensor["sensor_value"] = garden1.light;
    illumSensor["sensor_unit"] = "lux";
    serializeJson(jsonData, jsonLight1);
    // Print the JSON string to Serial for debugging
    serializeJson(jsonData, Serial);
  }
  if(jsonGarden == 2) {
    jsonData["station_id"] = "garden_0002";
    jsonData["station_name"] = "Garden 0002";
    jsonData["gps_longitude"] = 107.89;
    jsonData["gps_latitude"] = 11.5;
    JsonArray sensors = jsonData.createNestedArray("sensors");
    // Add sensor objects to the array
    JsonObject illumSensor = sensors.createNestedObject();
    illumSensor["sensor_id"] = "light_0002";
    illumSensor["sensor_name"] = "Illuminance";
    illumSensor["sensor_value"] = garden2.light;
    illumSensor["sensor_unit"] = "lux";
    serializeJson(jsonData, jsonLight2);
    // Print the JSON string to Serial for debugging
    serializeJson(jsonData, Serial);
  }
}

int count = 0;
void HandelControl(String incomingString ){
  int pos1 = incomingString.indexOf(':');
  int pos2 = incomingString.indexOf(','); 
  controlDevice1.ID = incomingString.substring(pos1+2, pos2-1);
  pos1 = incomingString.indexOf(':', pos2);
  pos2 = incomingString.indexOf(',', pos1);  
  controlDevice1.Device = incomingString.substring(pos1+2, pos2-1);
  pos1 = incomingString.indexOf(':', pos2);
  pos2 = incomingString.indexOf('}'); 
  controlDevice1.Status = incomingString.substring(pos1+1, pos2);
  ControlDeviceAllGarden(controlDevice1.ID, controlDevice1.Device,controlDevice1.Status);  
}
void ControlDeviceAllGarden(String ID, String Device, String value){    
    String datasent = "{"+ ID  +","+Device +","+ value+"}"+ "#"; 
    mySerial.println(datasent);
    Serial.println(datasent);
}
void HandelGarden1(String incomingString ){
  // Tách chuỗi thành các phần tử
  int pos1 = incomingString.indexOf(':');
  int pos2 = incomingString.indexOf(',');  
  garden1.airHumi = incomingString.substring(pos1+1, pos2);
  pos1 = incomingString.indexOf(':', pos2);
  pos2 = incomingString.indexOf(',', pos1);  
  garden1.airTemp = incomingString.substring(pos1+1, pos2);  
  pos1 = incomingString.indexOf(':', pos2);
  pos2 = incomingString.indexOf(',', pos1);
  garden1.soilHumi = incomingString.substring(pos1+1, pos2);
  pos1 = incomingString.indexOf(':', pos2);
  pos2 = incomingString.indexOf('}', pos1);
  garden1.light = incomingString.substring(pos1+1, pos2);
  // Serial.println(garden1.airHumi);
  // Serial.println(garden1.airTemp);
  // Serial.println(garden1.soilHumi);
  // Serial.println(garden1.light);
}
void publishGarden1(){
  client.publish(MQTT_SERIAL_PUBLISH_CH_AIRHUMI, jsonAirHumi1.c_str());  // Chuyển đổi sang const char *
  delay(10);
  client.publish(MQTT_SERIAL_PUBLISH_CH_AIRTEMP, jsonAirTemp1.c_str());  // Chuyển đổi sang const char *
  delay(10);
  client.publish(MQTT_SERIAL_PUBLISH_CH_SOIL, jsonSoilHumi1.c_str());  // Chuyển đổi sang const char *
  delay(10);
  client.publish(MQTT_SERIAL_PUBLISH_CH_LIGHT, jsonLight1.c_str());  // Chuyển đổi sang const char *
  delay(10);
}
void publishGarden2(){
  client.publish(MQTT_SERIAL_PUBLISH_CH_AIRHUMI, jsonAirHumi2.c_str());  // Chuyển đổi sang const char *
  client.publish(MQTT_SERIAL_PUBLISH_CH_AIRTEMP, jsonAirTemp2.c_str());  // Chuyển đổi sang const char *
  client.publish(MQTT_SERIAL_PUBLISH_CH_SOIL, jsonSoilHumi2.c_str());  // Chuyển đổi sang const char *
  client.publish(MQTT_SERIAL_PUBLISH_CH_LIGHT, jsonLight2.c_str());  // Chuyển đổi sang const char *
}
void HandelGarden2(String incomingString ){
  // Tách chuỗi thành các phần tử
  int pos1 = incomingString.indexOf(':');
  int pos2 = incomingString.indexOf(',');  
  garden2.airHumi = incomingString.substring(pos1+1, pos2).toFloat();
  pos1 = incomingString.indexOf(':', pos2);
  pos2 = incomingString.indexOf(',', pos1);  
  garden2.airTemp = incomingString.substring(pos1+1, pos2).toFloat();  
  pos1 = incomingString.indexOf(':', pos2);
  pos2 = incomingString.indexOf(',', pos1);
  garden2.soilHumi = incomingString.substring(pos1+1, pos2).toFloat();
  pos1 = incomingString.indexOf(':', pos2);
  pos2 = incomingString.indexOf('}', pos1);
  garden2.light = incomingString.substring(pos1+1, pos2).toFloat();
  // Serial.println(garden2.airHumi);
  // Serial.println(garden2.airTemp);
  // Serial.println(garden2.soilHumi);
  // Serial.println(garden2.light);
}
void receiveData(){
  //if(mySerial.available() >0){
    String incomingString = mySerial.readStringUntil('*');
    // String incomingString2 = mySerial.readStringUntil('#');

    int index = incomingString.indexOf('}');
    // Serial.println(index);
    if (index != -1){
      Serial.println("========================================");
      int index1 = incomingString.indexOf('{');
      String gadern = incomingString.substring(0, index1);
      if (gadern == "Garden1"){
        HandelGarden1(incomingString);
        preJsonGarden1();
      }
      else if (gadern == "Garden2"){
        HandelGarden2(incomingString);
        preJsonGarden2();
      }
      Serial.print("Data: ");
      Serial.println(incomingString);  
      Serial.println("========================================");
    }
  //}
}

void loop() {
  // sendData();
  client.loop();
  if (!client.connected()) {
    reconnectGarden1();
    reconnectGarden2();
  }
  receiveData();
  client.subscribe(MQTT_SERIAL_RECEIVER_CH1);
  // client.subscribe(MQTT_SERIAL_RECEIVER_CH2);
  timer ++;
  // if(timer == 10){
  //   publishGarden1();
  // }
  // client.publish(MQTT_SERIAL_PUBLISH_CH_AIRHUMI, jsonAirHumi1.c_str());  // Chuyển đổi sang const char *
  // client.publish(MQTT_SERIAL_PUBLISH_CH_AIRTEMP, jsonAirTemp1.c_str());  // Chuyển đổi sang const char *
  // client.publish(MQTT_SERIAL_PUBLISH_CH_SOIL, jsonSoilHumi1.c_str());  // Chuyển đổi sang const char *
  // client.publish(MQTT_SERIAL_PUBLISH_CH_LIGHT, jsonLight1.c_str());  // Chuyển đổi sang const char *
  publishGarden1();
  // Su lys ngu
  ControlDeviceAllGarden(controlDevice1.ID, controlDevice1.Device,controlDevice1.Status);  

  // publishGarden2();
  Serial.println(timer);
}