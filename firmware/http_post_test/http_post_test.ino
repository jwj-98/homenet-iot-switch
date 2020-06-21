#include <WiFi.h>
#include <HTTPClient.h>
  
const char* ssid = "YOUR_WIFI_SSID";
const char* password =  "YOUR_WIFI_PASSWORD";
  
void setup() {
  
  Serial.begin(115200);
  delay(4000);   //Delay needed before calling the WiFi.begin
  
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
   if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   delay(1000);
   HTTPClient http;   
  
   http.begin("http://YOUR_SERVER_IP:5000/auth/registerSwitch");  //Specify destination for HTTP request
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");             //Specify content-type header
   String post = "&username=test&password=test&mac=AA:BB:CC:DD:EE:FF&type=s2";
   
   int httpResponseCode = http.POST(post);   //Send the actual POST request
  
   if(httpResponseCode>0){
  
    String response = http.getString();                       //Get the response to the request
  
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  
   }else{
  
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  
   }
  
   http.end();  //Free resources
  
 }else{
  
    Serial.println("Error in WiFi connection");   
  
 }
  
  delay(10000);  //Send a request every 10 seconds
  
}
  
void loop() {
  

  
}
