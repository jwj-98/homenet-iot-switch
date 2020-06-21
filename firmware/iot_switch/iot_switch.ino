#include <FS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>


// pin var
int rly[] = {5, 17, 16};
int led[] = {13, 21, 19, 18};
int btn[] = {3, 23, 22, 1};
bool preBtn[] = {false, false, false, false};
unsigned long times;

//접속할 ap 정보, 등록할 유저 정보 변수
String acSsid_str="";
String acPw_str="";
String type;
String mac;
boolean trig= false;
int count= 1;
String username;
String userpw;

//기기등록 시 생성할 ap 정보 변수
const char* APSsid = "switchAP";
const char* APPw = "YOUR_AP_PASSWORD";
AsyncWebServer server(80);


//client가 잘못된 주소로 접속시 표시합니다.
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

//ap웹 서버에서 보여줄 html입니다. WiFi스캔을 위해 header와 footer를 구분합니다. (header + 'WiFi스캔' + footer)
String htmlHeader = R"qoooo(<!DOCTYPE html><html lang=\"en\"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0" ><style>div, input {padding: 5px;font-size: 1em;}input{width: 95%;}select {padding: 5px;font-size: 1em;width: 100%;}body {text-align: center;font-family: verdana;}button {border: 0;border-radius: 0.3rem;background-color: #1fa3ec;color: #fff;line-height: 2.4rem;font-size: 1.2rem;width: 100%}h3 {border: 0;border-radius: 0.3rem;background-color: #3b3b3b;color: #fff;line-height: 2.4rem;font-size: 1.2rem;width: 100%}</style></head><body><div style="text-align:center;display:inline-block;min-width:260px;"><h3>IOT 기기등록</h3>공유기 정보를 등록하고<BR>기기등록을 완료해주세요.<BR><br/><form method='post' action='save'><div><input name='username' type='text' placeholder='사이트 아이디'></div><div> <input name='userpw' type='password' placeholder='사이트 비밀번호'></div><div><select name="ssid"><option selected>SSID 선택...</option>)qoooo";
String htmlFooter = R"qoooo(</select></div><div><input name='pw' type='password' placeholder='비밀번호'></div><div><select name="type"><option selected>기기종류 선택...</option><option value="switch 1">스위치 1구</option><option value="switch2">스위치 2구</option><option value="switch3">스위치 3구</option><option value="moodLight">무드등</option></select></div><br/><button type='submit'>완&nbsp;료</button></form><p>&copy; 2020-홈네트워크</p><br/></div></body></html>)qoooo";
String viewHtml;
String htmlSave = R"hellohaha(<!DOCTYPE html><html lang=\"en\"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1.0" ><style>div, input {padding: 5px;font-size: 1em;}input {width: 95%;}select {padding: 5px;font-size: 1em;width: 100%;}body {text-align: center;font-family: verdana;}button {border: 0;border-radius: 0.3rem;background-color: #1fa3ec;color: #fff;line-height: 2.4rem;font-size: 1.2rem;width: 100%}h3 {border: 0;border-radius: 0.3rem;background-color: #3b3b3b;color: #fff;line-height: 2.4rem;font-size: 1.2rem;width: 100%}.w {border: 0;border-radius: 0.3rem;background-color: rgba(255, 0, 0, 0.78);color: #fff;line-height: 1.8rem;width: 100%}</style></head><body><div style="text-align:center;display:inline-block;min-width:260px;"><h3>IOT 기기등록</h3>기기등록 버튼의 불빛을 확인해주세요!<BR>기기등록이 완료되면 불이 꺼집니다.<BR><BR><div class="w">10초이상기기등록이진행되지않을경우<BR>메뉴얼2.부터다시진행해주세요.</div><br/><p>&copy;2020-홈네트워크</p><br/></div></body></html>)hellohaha";


void setup() {
    Serial.begin(115200);
    for(int i=0; i<4; i++){
        pinMode(led[i], OUTPUT);
        pinMode(btn[i], OUTPUT);
    }
    for(int i=0; i<3; i++){
        pinMode(rly[i], OUTPUT);
        digitalWrite(rly[i], LOW);
        digitalWrite(led[i+1], HIGH);
    }
}


void loop() {
  //버튼 스캔
  bool val[] = {digitalRead(btn[0]), digitalRead(btn[1]), digitalRead(btn[2]), digitalRead(btn[3])};

  //전등 버튼 동작
  for(int i=1; i<4; i++){
    if(val[i]){
        if(!preBtn[i]){
            changeRlySt(i);
            sendRlyCon(i);
            preBtn[i] = true;
        }
    }else if(!val[i]){
        if(preBtn[i]){
            preBtn[i] = false;
        }
    }
  }

  //기기등록 버튼 동작 (2초이상 누를때 동작합니다.)
  String scanSsid;
  int scanCount;

  if(val[0]){ //버튼을 누를때
    if(!preBtn[0]){ //버튼 off 에서 on 으로 바뀔때 1회 실행 cuz.preBtn val
      times= millis();
      preBtn[0]= true;
    }
    //버튼을 누르는 중이라면 실행
    if (  (millis() - times) > 2000  ){ //2초이상 버튼을 눌렀다면
      digitalWrite(led[0], HIGH);
      trig = true;
      acSsid_str = "";
      acPw_str= "";
      WiFi.disconnect();
    }
  }
  else if(!val[0]) { //버튼을 누르지않았을때
    if(preBtn[0]){//버튼 on 에서 off 로 바뀔때 1회 실행
      preBtn[0]= false;
    }
  }


  //기기등록 이벤트 처리
  while (trig) { //현재 ap에 접속돼있지 않다면 반복 10번까지 접속 시도
    delay(500);
    WiFi.softAPdisconnect(true);

    //형변환 cuz; wifi.begin(char[],char[])
    char* acSsid = (char*)acSsid_str.c_str();
    char* acPw = (char*)acPw_str.c_str();

    WiFi.begin(acSsid, acPw);
    Serial.print("AP에 접속 시도...(");
    Serial.print(count);
    Serial.println(")");
    if(WiFi.status() != WL_CONNECTED){
      count++;
    }
    if(WiFi.status() == WL_CONNECTED){ //AP접속 성공시 실행, 웹 요청
      Serial.println("!...AP에 접속 성공");
      int httpCount = 0;
      boolean httpTrig = true;

      while(httpTrig){
        mac = WiFi.macAddress();
        String response;
        String post = "&username=" + username + "&password=" + userpw + "&mac=" + mac + "&type=" + type;

        Serial.print("send post: ");
        Serial.println(post);

        delay(1000);
        HTTPClient http;

        http.begin("http://YOUR_SERVER_IP:5000/auth/registerSwitch");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpResponseCode = http.POST(post);

        if(httpResponseCode>0){
         response = http.getString();

         Serial.println(httpResponseCode);
         Serial.println(response);
        }else{
         Serial.print("Error on sending POST: ");
         Serial.println(httpResponseCode);
        }
        http.end();  //Free resources

        if(response.equals("ok")){
          trig = false;
          httpTrig = false;
          httpCount = 0;
          digitalWrite(led[0], LOW);
        }

        if(httpCount > 5) {
          WiFi.disconnect();
          acSsid_str = "";
          acPw_str = "";
          httpTrig = false;
        }
        httpCount++;
      }
    }


    if(count > 10){// 접속시도 10회초과로 ap모드를 활성화해 접속할ap 정보를 받아옵니다.
      WiFi.disconnect();
      trig= false;//post정보를 받을때까지 trig=false; ap접속을 시도하지 않습니다.

      IPAddress ip(192, 168, 4, 1);
      IPAddress gateway(192, 168, 4, 2);
      IPAddress subnet(255, 255, 255, 0);
      WiFi.softAPConfig(ip, gateway, subnet);
      WiFi.softAP(APSsid, APPw);//ap생성

      Serial.print("접속실패...AP모드를 활성화합니다. AP IP Address: ");
      Serial.println(WiFi.softAPIP());

      // softAPIP로 get 요청시 실행
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
          String ssids; //스캔된 WiFi의 ssid
          int netcount = WiFi.scanNetworks();
          viewHtml = htmlHeader; //스캔된 ap ssid를 html코드 사이에 추가시킵니다.
          for(int i= 0; i< netcount; i++){
            ssids= WiFi.SSID(i);
            viewHtml += "<option value=" + ssids + ">";
            viewHtml += ssids;
            viewHtml += "</option>";
          }
          viewHtml += htmlFooter;
          request->send(200, "text/html", viewHtml);
      });
      // /save로 post 요청시 실행
      server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
          if (request->hasParam("username", true)) {
              username = request->getParam("username", true)->value();
          }
          if (request->hasParam("userpw", true)) {
              userpw = request->getParam("userpw", true)->value();
          }
          if (request->hasParam("ssid", true)) {
              acSsid_str = request->getParam("ssid", true)->value();
          }
          if (request->hasParam("pw", true)) {
              acPw_str = request->getParam("pw", true)->value();
          }
          if (request->hasParam("type", true)) {
              type = request->getParam("type", true)->value();
          }
          trig = true;
          count = 1;

          request->send(200, "text/html", htmlSave);

      });
      server.onNotFound(notFound);
      server.begin();
    }

  }


  //mqtt에 접속 후 구독합니다.


  //mqtt broker에서 보낸 메세지가 있다면 헤더를 확인하고 릴레이를 제어합니다.


}


//함수.릴레이 상태 변경
void changeRlySt(int num){
    if( digitalRead(rly[num]) ){
        digitalWrite(rly[num], LOW);
        digitalWrite(led[num], HIGH);
    }else{
        digitalWrite(rly[num], HIGH);
        digitalWrite(led[num], LOW);
    }
}

//함수.기기 수동제어 전송
void sendRlyCon(int num){

}
