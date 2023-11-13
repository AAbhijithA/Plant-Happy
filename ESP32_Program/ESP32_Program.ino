#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
#include <DHT.h>
#include "ThingSpeak.h"

#define DHTPIN 14
#define DHTTYPE DHT11   

#define CHANNEL_ID Enter_Your_ThingSpeak_Channel_ID
#define CHANNEL_API_KEY "Enter your ThingSpeak Write API Key"

WiFiClient client;

int moistureSensor = 12;
int threshold = 500;

DHT dht(DHTPIN, DHTTYPE);
 
const char* WIFI_SSID = "Enter your Wifi Name";
const char* WIFI_PASS = "Enter your Wifi Password";
 
WebServer server(80);
 
 
static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);
void serveJpg(){
  auto frame = esp32cam::capture();
  if(frame == nullptr){
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()      ));
 
  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient localclient = server.client();
  frame->writeTo(localclient);
}
 
void handleJpgLo(){
  if(!esp32cam::Camera.changeResolution(loRes)){
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}
 
void handleJpgHi(){
  if(!esp32cam::Camera.changeResolution(hiRes)){
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}
 
void handleJpgMid(){
  if(!esp32cam::Camera.changeResolution(midRes)){
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}
 
 
void  setup(){
  Serial.begin(115200);

  dht.begin();
  
  Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);
 
    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-lo.jpg");
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /cam-mid.jpg");
 
  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam-mid.jpg", handleJpgMid);
  
  server.begin();
  ThingSpeak.begin(client);
}
 
void loop(){
  server.handleClient();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if(isnan(temperature) || isnan(humidity)){
    Serial.println("Failed to read from DHT sensor!");
  } 
  else{
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  }
  int moistureValue = analogRead(moistureSensor);
  Serial.print("Moisture Level: ");
  Serial.print(moistureValue);
  if(moistureValue < threshold) {
    Serial.println(" ,Soil is too dry!");
  } 
  else{
    Serial.println(" ,Soil moisture is okay.");
  }
  ThingSpeak.setField(1,temperature);
  ThingSpeak.setField(2,humidity);
  ThingSpeak.setField(3,moistureValue);
  ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);
  delay(4000);
}
