#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WifiLocation.h>


const char* ssid = "**********";
const char* password = "********";
const char* BingKey = "********";


void setup(){
    Serial.begin(9600);
    delay(1000);

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED){
        Serial.println("Connecting");
        delay(2000);
    }

    Serial.println("Connected to the WiFi network");
    Serial.println("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    locate();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void locate() {
 Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (WiFi.status() == WL_CONNECTED) {
    WifiLocation location(BingKey);

    location_t loc = location.getGeoFromWiFi();
    Serial.println("Location request data");
    Serial.println(location.getSurroundingWiFiJson());
    Serial.println("Latitude: " + String(loc.lat, 7));
    Serial.println("Longitude: " + String(loc.lon, 7));
    Serial.println("Accuracy: " + String(loc.accuracy));
    
     
  }
}
