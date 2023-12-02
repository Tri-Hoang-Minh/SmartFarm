#include <DHT.h>
#include <ArduinoMqttClient.h>
#include <BH1750.h>
//#include <Wire.h>
//#include <WiFiNINA.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

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
  //
  lcd.init();         // initialize the lcd
  lcd.backlight();    // Turn on the LCD screen backlight
}
float getHumiAir(){
  //return dht.readHumidity();
  for(int i=0;i<=9;i++)   
  {
    humiAirValue += random(50,90);     
  }
  meanhumiAir = humiAirValue/10;
  humiAirValue = 0;
  return meanhumiAir;
}
float getTempAir(){
  //return dht.readTemperature();
  for(int i=0;i<=9;i++)   
  {
    tempAirValue += random(25,40);     
  }
  meantempAir = tempAirValue/10;
  tempAirValue = 0;
  return meantempAir;
}
int getSoilSensor(){
  for(int i=0;i<=9;i++)   
  {
    soilValue += random(30,100);     
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
    lightValue += random(0,1000);  
  }
  meanLight =lightValue/10;     //Tính giá trị trung bình
  lightValue=0;
  return meanLight;
}
void sendData(float doam,float doC, float soilHumi, float lux){
  String datasent =String ("Garden2{ ") + String ("airHumi: ") + String (doam)+","+ String ("airTemp: ")+ String (doC)+","+String ("soilHumi: ")+String (soilHumi)+"," +String ("Light: ")+ String (lux)+ "}*";
  mySerial.println(datasent);
  Serial.println(datasent);
  Serial.println("========================================");
}
void controlDevice(String gardenID, String deviceID, int state){
  if(gardenID=="garden_0002"&& deviceID == "device_0005" && state == 1){
    digitalWrite(relayDevice1, HIGH);
  }
  else if(gardenID=="garden_0002"&& deviceID == "device_0005" && state == 0){
    digitalWrite(relayDevice1, LOW);
  }
  else if(gardenID=="garden_0002"&& deviceID == "device_0006" && state == 1){
    digitalWrite(relayDevice2, HIGH);
  }
  else if(gardenID=="garden_0002"&& deviceID == "device_0006" && state == 0){
    digitalWrite(relayDevice2, LOW);
  }
  else if(gardenID=="garden_0002"&& deviceID == "device_0007" && state == 1){
    digitalWrite(relayDevice3, HIGH);
  }
  else if(gardenID=="garden_0002"&& deviceID == "device_0007" && state == 0){
    digitalWrite(relayDevice3, LOW);
  }
  else if(gardenID=="garden_0002"&& deviceID == "device_0008" && state == 1){
    digitalWrite(relayDevice4, HIGH);
  }
  else if(gardenID=="garden_0002"&& deviceID == "device_0008" && state == 0){
    digitalWrite(relayDevice4, LOW);
  }
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
  String incomingString = mySerial.readStringUntil('#');
  // Serial.println(incomingString);
  int index = incomingString.indexOf('{');
  // Serial.println(index);
  // if (index != -1){
    Serial.println("========================================");
    int index1 = incomingString.indexOf('{');
    String garden = incomingString.substring(0, index1);
    // Serial.println(garden);
    if (garden == "Garden1"){         
    }else{
      Serial.println(incomingString);
      HandelControl(incomingString);
    }
  // }
  // Serial.println("========================================");
}
void ScreenLCD(float doam, float doC, int soilHumi, float lux, int timer) {
  
  if(timer < 3){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hello");
    lcd.setCursor(0, 1);
    lcd.print("Garden 2");
    // lcd.clear();
  }
  if(timer == 3){
    lcd.clear();
  }
  if(timer> 3 && timer <10){
    lcd.setCursor(0, 0);
    lcd.print("T: ");
    lcd.print(doC);
    lcd.print("H: ");
    lcd.print(doam);
    lcd.setCursor(0, 1);
    lcd.print("S: ");
    lcd.print(soilHumi);
    lcd.print("L: ");
    lcd.print(lux);
  }
  if(timer == 10){
    lcd.clear();
  }
}

void loop() 
{
  timer++;
  float doam ;
  float doC ;
  int soilHumi;
  float lux;
  if(timer == 10){
    doam = getHumiAir(); //Đọc độ ẩm

    doC = getTempAir(); //Đọc nhiệt độ C

    soilHumi = getSoilSensor();

    lux = getLightsensor();

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
  ScreenLCD(doam, doC, soilHumi,lux,timer);
  receiveData(); 
  //delay(1000);
}
