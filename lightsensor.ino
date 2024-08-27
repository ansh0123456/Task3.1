#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>

char ssid [] = "realme" ;
char pass [] = "Realme24";

WiFiClient client;

BH1750 lightMeter; // Create an instance of the BH1750 light sensor library const 

char HOST_NAME[] = "maker.ifttt.com"; // Website
String PATH_NAME = "https://maker.ifttt.com/trigger/Taneja/with/key/fYukWM6_qaii6oHQkfHQXeA7h0EUQRV5cIFjTuI9j7A"; // Key

void setup() 
{
  WiFi.begin(ssid, pass);
  Serial.begin(9600); 

  Wire.begin();
  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));

  // Attempt to connect to WiFi
  connectWiFi();
}

void loop() 
{
  float light_intensity = lightMeter.readLightLevel();
  Serial.print("light_intensity: ");
  Serial.println(light_intensity);
  
  if (light_intensity >= 360) 
  {
    triggerWebhook("Daylight");
  }
   else if (light_intensity < 80) 
  {
    triggerWebhook("Dawnlight");
  }
  delay(3000); // Wait for 30 second before reading light  
}

void connectWiFi() 
{
  Serial.println("Attempting to connect to WiFi...");
  WiFi.begin(ssid, pass);  // Start WiFi connection
do 
  {
    delay(500);  // Check every 0.5 seconds for connection status
    Serial.print(".");
  } 
    while (WiFi.status() != WL_CONNECTED);
    Serial.println("WiFi connected!");
 }

void triggerWebhook(String eventName) 
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    connectWiFi();
  }
  
  if (client.connect("maker.ifttt.com", 80)) 
  {
    client.println("GET " + PATH_NAME + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();
    delay(500);
    client.stop();
    Serial.println(" Data send successfully");
  } 
  else 
  {
    Serial.println("Unable to connect to webhook server");
  }
}
