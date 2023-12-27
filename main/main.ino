// C++ code
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WifiLocation.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

/* WiFi Connection Method for name-password type*/
const char* ssid = "**********";
const char* password = "****";


/* WiFi Connection Method for name-username-password type
const char* ssid = "WCBYOD";
#define EAP_IDENTITY "kiwirafe@gmail.com"
#define EAP_USERNAME "*****"
#define EAP_PASSWORD "******"
*/


const int DIT = 2;
const int DASH = 0;
const int ADD = 4;
const int PUSH = 16;
const int BUZZER = 5;


int last_dit = LOW;
int last_dash = LOW;

String MORSE[37] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", "------"};
char CHARS[37] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'M', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ' '};

String input;
String inputs;

int column1 = 6;
int column2 = 6;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.noCursor();
  lcd.setCursor(0,0);
  lcd.print("Connecting");
  lcd.setCursor(0,1);
  lcd.print("to wifi...");
  delay(1000);

  WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);

  while (WiFi.status() != WL_CONNECTED){
      Serial.println("Connecting");
      delay(2000);
  }

  Serial.println("Connected to the WiFi network");
  Serial.println("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("WELCOME TO");
  lcd.setCursor(4,1);
  lcd.print("MORSENET");
  delay(1500);
  lcd.clear();
  
  lcd.setCursor(1,0);
  lcd.print("INPUT IN MORSE");
  lcd.setCursor(1,1);
  lcd.print("AND UPLOAD IT!");
  delay(1500);
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("MORSE:");
  lcd.setCursor(0,1);
  lcd.print("TEXTS:");
  
  lcd.setCursor(0,1);
  pinMode(DIT, INPUT_PULLUP);
  pinMode(DASH, INPUT_PULLUP);
  pinMode(ADD, INPUT_PULLUP);
  pinMode(PUSH, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
}

int post(String morse) {
  Serial.println("Posting JSON data to server...");
  Serial.println(morse);
  // Block until we are able to connect to the WiFi access point
  if (WiFi.status() == WL_CONNECTED) {
     
    HTTPClient http;   
     
    http.begin("https://20130.pythonanywhere.com/");  
    http.addHeader("Content-Type", "application/json");         
     
    StaticJsonDocument<200> doc;
    // Add values in the document
  
    doc["password"] = "****";
    doc["username"] = "ESP32 #1";
    doc["morse"] = morse;
     
    String RequestBody;
    serializeJson(doc, RequestBody);
     
    int ReponseCode = http.POST(RequestBody);
    if(ReponseCode > 0){
      String response = http.getString();                       
       
      Serial.println(response);
      Serial.println(ReponseCode);   
      return 0;
    }
    else {
      Serial.println("Error occurred while sending HTTP POST.");
      Serial.println(ReponseCode);
      return 1;
    }
     
  }
}

void loop() {  
  int dit_state = digitalRead(DIT);
  int dash_state = digitalRead(DASH);
  int add_state = digitalRead(ADD);
  int push_state = digitalRead(PUSH);

  if (column2 == 16) {
  	lcd.setCursor(6,1);
  	lcd.print(inputs.substring(inputs.length() - 11, inputs.length()));
    column2 -= 1;
  }
  
  if (last_dit != dit_state) {
    if (dit_state == LOW) {
      tone(BUZZER, 2000, 50);
      lcd.setCursor(column1, 0);
      lcd.print('.');
      input += '.';
      column1 += 1;
    }
    last_dit = dit_state;
  } else if (last_dash != dash_state) {
    if (dash_state == LOW) {
      tone(BUZZER, 1000, 50);
      lcd.setCursor(column1, 0);
      lcd.print('-');
      input += '-';
      column1 += 1;
    }
    last_dash = dash_state;
  }
  Serial.println(input);

  if (add_state == LOW) {
    lcd.setCursor(6, 0);
    lcd.print("           ");
    column1 = 6;
    if (input == "----") {
      inputs.remove(inputs.length() - 1);
      column2 -= 1;
      lcd.setCursor(column2, 1);
      lcd.print(' ');
      lcd.setCursor(column2, 1);
    } else {
      for (int i = 0; i < 37; i++) {
        if (input == MORSE[i]) {
          char output = CHARS[i];
          inputs += output;
          lcd.setCursor(column2, 1);
          lcd.print(output);
          column2 += 1;
        }
       }
    }
    input = "";
  }
  if (push_state == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Uploading...");
    lcd.setCursor(0, 0);
    
    int result = post(inputs);
    if (result == 0) {
      lcd.print("Success!");
      delay(1500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MORSE:");
      lcd.setCursor(0, 1);
      lcd.print("TEXTS:");
      column1 = 6;
      column2 = 6;
      inputs = "";
    } else {
      lcd.print("Error!");
    }
  }
  
}
