#include <WiFi.h>

const char* ssid     = "YOUR_WIFI_SSID";    // 연결할 SSID
const char* password = "YOUR_WIFI_PASSWORD";     // 연결할 SSID의 비밀번호

void setup()
{
    Serial.begin(115200);
    delay(10);
    
    WiFi.begin(ssid, password);

    // 와이파이망에 연결
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("WiFi access...");
    }
    // 연결 여부 로그 출력
    if(WiFi.status() == WL_CONNECTED){
        Serial.println( "WiFi connected!");
        Serial.print( "local ip: " );
        Serial.println( WiFi.localIP() );
    }
    else 
        Serial.println("WiFi not connected");
    // MAC출력
    Serial.println(WiFi.macAddress());
}

void loop()
{
  
}
