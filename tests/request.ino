#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WifiLocation.h>

/* WiFi Connection Method for name-password type*/
const char* ssid = "**********";
const char* password = "****";


/* WiFi Connection Method for name-username-password type
const char* ssid = "WCBYOD";
#define EAP_IDENTITY "kiwirafe@gmail.com"
#define EAP_USERNAME "*****"
#define EAP_PASSWORD "******"
*/

void setup(){
    Serial.begin(9600);
    delay(1000);

    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, password);
    //WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);

    while (WiFi.status() != WL_CONNECTED){
        Serial.println("Connecting");
        delay(2000);
    }

    Serial.println("Connected to the WiFi network");
    Serial.println("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    post();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void post() {
 Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (WiFi.status() == WL_CONNECTED) {
     
    HTTPClient http;   
     
    http.begin("http://192.168.68.58:5000/");  
    http.addHeader("Content-Type", "application/json");         
     
    StaticJsonDocument<200> doc;
    // Add values in the document
  
    doc["password"] = "****";
    doc["username"] = "kiwirafe";
    doc["morse"] = "QWERTY";
    
     
    String RequestBody;
    serializeJson(doc, RequestBody);
     
    int ReponseCode = http.POST(RequestBody);
 
    if(ReponseCode > 0){
      String response = http.getString();                       
       
      Serial.println(ReponseCode);   
      Serial.println(response);
    }
    else {
      Serial.println("Error occurred while sending HTTP POST.");
      Serial.println(ReponseCode);
    }
     
  }
}
