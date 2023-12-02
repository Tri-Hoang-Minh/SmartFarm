#include <DHT.h>
#include <ArduinoMqttClient.h>
#include <BH1750.h>
//#include <Wire.h>
//#include <WiFiNINA.h>
#include <SoftwareSerial.h>

// SoftwareSerial mySerial(16, 17);
SoftwareSerial mySerial(10, 11);
int timer = 0;
const int relayDevice1= 2;
const int relayDevice2= 3;
const int relayDevice3= 4;
const int relayDevice4= 5;
const int DHTPIN = 6;//Chân Out của cảm biến nối chân số 5 Arduino
const int soilPin = 7;
const int DHTTYPE = DHT11;   // Khai báo kiểu cảm biến là DHT11
//const int DHTTYPE = DHT22;   // DHT 22 
//const int DHTTYPE = DHT21;   // DHT 21 

int soilValue, meanSoilValue;
float tempAirValue, meantempAir;
float humiAirValue, meanhumiAir;
float lightValue, meanLight;
BH1750 lightMeter;

int digital = A0;
DHT dht(DHTPIN, DHTTYPE); //Khai báo thư viện chân cảm biến và kiểu cảm biến
//int count = 0;
void setup() 
{
  Serial.begin(9600);
  mySerial.begin(9600);
  dht.begin(); //Khởi động cảm biến
  pinMode (soilPin, INPUT);
  Wire.begin();
  lightMeter.begin();
  pinMode(relayDevice1,OUTPUT);
  pinMode(relayDevice2,OUTPUT);
  pinMode(relayDevice3,OUTPUT);
  pinMode(relayDevice4,OUTPUT);
  digitalWrite(relayDevice1, LOW);
  digitalWrite(relayDevice2, LOW);
  digitalWrite(relayDevice3, LOW);
  digitalWrite(relayDevice4, LOW);
}
float getHumiAir(){
  //return dht.readHumidity();
  for(int i=0;i<=9;i++)   
  {
    humiAirValue += dht.readHumidity();     
  }
  meanhumiAir = humiAirValue/10;
  humiAirValue = 0;
  return meanhumiAir;
}
float getTempAir(){
  //return dht.readTemperature();
  for(int i=0;i<=9;i++)   
  {
    tempAirValue += dht.readTemperature();     
  }
  meantempAir = tempAirValue/10;
  tempAirValue = 0;
  return meantempAir;
}
int getSoilSensor(){
  for(int i=0;i<=9;i++)   
  {
    soilValue += analogRead(soilPin);     
  }
  meanSoilValue=soilValue/10;     //Tính giá trị trung bình
  int fakeHumi = map(meanSoilValue, 0, 1023, 0, 100);    //Chuyển giá trị Analog thành giá trị %
  int realHumi = 100 - fakeHumi;              //Tính giá trị phần trăm thực, chuyển điện thế khô thành ẩm
  soilValue=0;
  return realHumi;
}
float getLightsensor(){
  for(int i=0;i<=9;i++)   
  {
    lightValue += lightMeter.readLightLevel();     
  }
  meanLight =lightValue/10;     //Tính giá trị trung bình
  lightValue=0;
  return meanLight;
}
void sendData(float doam,float doC, float soilHumi, float lux){
  String datasent =String ("Garden1{ ") + String ("airHumi: ") + String (doam)+","+ String ("airTemp: ")+ String (doC)+","+String ("soilHumi: ")+String (soilHumi)+"," +String ("Light: ")+ String (lux)+ "}*";
  mySerial.println(datasent);
  Serial.println("========================================");
}
void controlDevice(String gardenID, String deviceID, int state){
  if(gardenID=="garden_0001"&& deviceID == "device_0001" && state == 1){
    digitalWrite(relayDevice1, HIGH);
  }
  else if(gardenID=="garden_0001"&& deviceID == "device_0001" && state == 0){
    digitalWrite(relayDevice1, LOW);
  }
  else if(gardenID=="garden_0001"&& deviceID == "device_0002" && state == 1){
    digitalWrite(relayDevice2, HIGH);
  }
  else if(gardenID=="garden_0001"&& deviceID == "device_0002" && state == 0){
    digitalWrite(relayDevice2, LOW);
  }
  else if(gardenID=="garden_0001"&& deviceID == "device_0003" && state == 1){
    digitalWrite(relayDevice3, HIGH);
  }
  else if(gardenID=="garden_0001"&& deviceID == "device_0003" && state == 0){
    digitalWrite(relayDevice3, LOW);
  }
  else if(gardenID=="garden_0001"&& deviceID == "device_0004" && state == 1){
    digitalWrite(relayDevice4, HIGH);
  }
  else if(gardenID=="garden_0001"&& deviceID == "device_0004" && state == 0){
    digitalWrite(relayDevice4, LOW);
  }
  // if(gardenID=="garden_0001"&& deviceID == "device_0001" ){
  //   bool relayState = digitalRead(relayDevice1);
  //   digitalWrite(relayDevice1, !relayState);
  // }
  // else if(gardenID=="garden_0001"&& deviceID == "device_0002"){
  //   bool relayState = digitalRead(relayDevice2);
  //   digitalWrite(relayDevice2, !relayState);
  // }
  // else if(gardenID=="garden_0001"&& deviceID == "device_0003"){
  //   bool relayState = digitalRead(relayDevice3);
  //   digitalWrite(relayDevice3, !relayState);
  // }
  // else if(gardenID=="garden_0001"&& deviceID == "device_0004"){
  //   bool relayState = digitalRead(relayDevice4);
  //   digitalWrite(relayDevice4, !relayState);
  // }
}
void HandelControl(String incomingString){
    int commaIndex1 = incomingString.indexOf(',');
    int commaIndex2 = incomingString.indexOf(',', commaIndex1+1);
    int commaIndex3 = incomingString.indexOf('}', commaIndex2+1);
    String Garden =  incomingString.substring(1, commaIndex1);
    Serial.println(Garden);
    String device = incomingString.substring(commaIndex1+1, commaIndex2); 
    Serial.println(device);
    int value = incomingString.substring(commaIndex2+1, commaIndex3).toInt(); 
    Serial.println(value);
    controlDevice(Garden, device, value);
}
void receiveData(){
  // receive  data
  // String incomingString = mySerial.readStringUntil('#');
  // Serial.println(incomingString);
  // HandelControl(incomingString);
  // Serial.println("========================================");
  String incomingString = mySerial.readStringUntil('#');
  Serial.println(incomingString);
  int index = incomingString.indexOf('{');
  // Serial.println(index);
  if (index != -1){
    Serial.println("========================================");
    int index1 = incomingString.indexOf('{');
    String garden = incomingString.substring(0, index1);
    //Serial.println(garden);
    if (garden == "garden2"){         
    }else{
      HandelControl(incomingString);
    }
  }
}
void loop() 
{
  timer++;
  receiveData();
  if(timer == 5){
    float doam = getHumiAir(); //Đọc độ ẩm

    float doC = getTempAir(); //Đọc nhiệt độ C

    int soilHumi = getSoilSensor();

    float lux = getLightsensor();

    // Kiểm tra cảm biến có hoạt động hay không
    if (isnan(doam) || isnan(doC)) 
    {
      Serial.println("Không có giá trị trả về từ cảm biến DHT");
      return;
    }

    Serial.print("Độ ẩm: "); 
    Serial.print(doam);
    Serial.print("%  Nhiệt độ: ");
    Serial.print(doC);
    Serial.println("°C");

    Serial. print("analog: "); Serial. print(meanSoilValue); Serial.print(" "); Serial. print("~"); Serial.print(" "); 
    Serial.print(soilHumi); Serial.println('%');

    Serial.print("Light:");
    Serial.print(lux);
    Serial.println("lx");

    //SendJson
    sendData(doam, doC, soilHumi,lux);
    timer = 0;
  }
  //Serial.println(timer);
  //delay(2000);
}
