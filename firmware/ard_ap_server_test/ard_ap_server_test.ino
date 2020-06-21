#include <FS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

//esp가 생성할 ap정보입니다.
const char* ssid = "switchAP";
const char* password = "YOUR_AP_PASSWORD";

//client가 잘못된 주소로 접속시 표시합니다.
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

//esp가 접속할 공유기 정보입니다. client로 부터 초기화합니다.
String acssid;
String acpw;
String devicetype;

//ap웹서버에서 보여줄 html입니다. WiFi스캔을 위해 header와 footer를 구분합니다. (header + 'WiFi스캔' + footer)
String htmlHeader = R"qoooo(<!DOCTYPE html><html lang=\"en\"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0" ><style>div, input {padding: 5px;font-size: 1em;}input{width: 95%;}select {padding: 5px;font-size: 1em;width: 100%;}body {text-align: center;font-family: verdana;}button {border: 0;border-radius: 0.3rem;background-color: #1fa3ec;color: #fff;line-height: 2.4rem;font-size: 1.2rem;width: 100%}h3 {border: 0;border-radius: 0.3rem;background-color: #3b3b3b;color: #fff;line-height: 2.4rem;font-size: 1.2rem;width: 100%}</style></head><body><div style="text-align:center;display:inline-block;min-width:260px;"><h3>IOT 기기등록</h3>공유기 정보를 등록하고<BR>기기등록을 완료해주세요.<BR><br/><form method='post' action='save'><div><select name="ssid"><option selected>SSID 선택...</option>)qoooo";
String htmlFooter = R"qoooo(</select></div><div><input name='pw' type='password' placeholder='비밀번호'></div><div><select name="type"><option selected>기기종류 선택...</option><option value="switch 1">스위치 1구</option><option value="switch2">스위치 2구</option><option value="switch3">스위치 3구</option><option value="moodLight">무드등</option></select></div><br/><button type='submit'>완&nbsp;료</button></form><p>&copy; 2020-</p><br/></div></body></html>)qoooo";
String viewHtml = htmlHeader;


void setup() {

    Serial.begin(115200);
    WiFi.softAP(ssid, password); //esp가 ap생성 시작

    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    String ssids; //스캔된 WiFi의 ssid
    int netcount = WiFi.scanNetworks();    
    
    for(int i= 0; i< netcount; i++){
      ssids= WiFi.SSID(i);
      viewHtml += "<option value=" + ssids + ">";
      viewHtml += ssids;
      viewHtml += "</option>";
    }
    viewHtml += htmlFooter;

    // softAPIP로 요청시 실행
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", viewHtml);
        });
        
    // /save로 post 요청시 실행
    server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
        String message = "정보를 입력해주세요.";
        if (request->hasParam("ssid", true)) {
            acssid = request->getParam("ssid", true)->value();
            message = acssid;
        }
        if (request->hasParam("pw", true)) {
            acpw = request->getParam("pw", true)->value();
            message = message + ", " + acpw;
        }
        if (request->hasParam("type", true)) {
            devicetype = request->getParam("type", true)->value();
            message = message + ", " + devicetype;
        }
        request->send(200, "text/plain", message);
        WiFi.softAPdisconnect(true);
    });


//    get 파라미터 요청시 실행
//    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
//        String message;
//        if (request->hasParam(PARAM_MESSAGE)) {
//            message = request->getParam(PARAM_MESSAGE)->value();
//        } else {
//            message = "No message sent";
//        }
//        request->send(200, "text/plain", "Hello, GET: " + message);
//    });

    
    server.onNotFound(notFound);

    server.begin();
}

void loop() {
  Serial.println(acssid);
}
