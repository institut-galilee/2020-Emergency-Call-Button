
#include <ESP8266WiFi.h>


const char* ssid = "";   // Your network SSID (name)
const char* password = "";  // Your network password (use for WPA, or use as key for WEP)
// Unique IFTTT URL resource
const char* resource = "/trigger/emergency_call_button/with/key/gXsg2b0dWW69zZjsRUd6M6IfNLqlhUGvtqDQw02TJDn";   //Important- your key goes here
int keyIndex = 0;   // your network key Index number (needed only for WEP)
int LED = D3;  //declaring pin for LED
int BUTTON = D2;  //declaring pin for button
// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

void setup(){
Serial.begin(9600);  //set rate of serial monitor
initWifi();
//makeIFTTTRequest();

pinMode(LED,OUTPUT);  //set LED pin as output
pinMode(BUTTON,INPUT_PULLUP);  //set button pin as input
}
// this loop looks for a button push and if happens 
//blinks LED and runs IFTTT request loop (otherwise LED off)
void loop(){
if(digitalRead(BUTTON) == LOW){
digitalWrite(LED,HIGH);
delay (1000);
digitalWrite(LED,LOW);
delay (1000);
digitalWrite(LED,HIGH);
delay (1000);
makeIFTTTRequest();

}else{

digitalWrite(LED,LOW);

}
}
// this loop helps for debugging Wifi connection 
//and initiates Wifi

void initWifi() {
  Serial.print("Connecting to: "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);  

  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() != WL_CONNECTED) {
     Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: "); 
  Serial.print(millis());
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
}

// this loops makes an HTTP request to the IFTTT web service
// and also allows the serial monitor to help with debugging
void makeIFTTTRequest() {
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
     Serial.println("Failed to connect, going back to sleep");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);
  client.print(String("GET ") + resource + 
                  " HTTP/1.1\r\n" +
                  "Host: " + server + "\r\n" + 
                  "Connection: close\r\n\r\n");
                  
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
     Serial.println("No response, going back to sleep");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop();

}
