# HomeNet IoT Switch

ESP32로 만든 **3구 스마트 전등 스위치**와 기기 등록을 받는 **Flask 서버**입니다.
스마트폰으로 스위치의 임시 Wi-Fi(AP)에 접속해 집 공유기 정보와 계정을 입력하면, 스위치가 공유기에 붙은 뒤 서버에 자기 자신을 등록합니다.

> 2020년 1학기 홈네트워크 수업 팀 프로젝트(1조)입니다. 학교 GitLab에서 진행한 작업을 정리해 옮겼습니다.

## 맡은 역할

| 담당 | 내용 |
|---|---|
| **본인** | ESP32 펌웨어 전체 (AP 프로비저닝, Wi-Fi 스캔 페이지, 기기 등록, 릴레이/버튼/LED 제어), 서버 인증·DB 스키마·기기 등록 API |
| 팀원 | 로그인/회원가입 화면 초안, 게시판 기능 (최종본에서는 제외) |

## 동작 흐름

```
 [스위치 등록 버튼]
        │
        ▼
 ESP32가 AP 모드로 전환 ("switchAP")
        │  스마트폰으로 접속 → 192.168.4.1
        ▼
 주변 Wi-Fi 스캔 결과가 담긴 등록 페이지 표시
 (사이트 아이디/비밀번호 + 공유기 SSID/비밀번호 입력)
        │
        ▼
 AP 종료 → 집 공유기에 접속
        │
        ▼
 POST /auth/registerSwitch  (username, password, mac, type)
        │
        ▼
 Flask 서버: 계정 확인 → switch 테이블에 MAC 등록 (같은 MAC이면 갱신)
```

## 폴더 구조

```
firmware/
  iot_switch/            최종 펌웨어 (릴레이 3채널 + 등록 버튼)
  ard_ap_server_test/    AP 모드 + 웹 폼 테스트
  wifi_access_test/      공유기 접속 테스트
  http_post_test/        서버 POST 테스트
  device_regist_test/    AP 프로비저닝 → 등록까지 통합 테스트
  device_regist_btn_test/ 등록 버튼 트리거 테스트
server/
  flaskr/                Flask 앱 (auth 블루프린트, SQLite)
  run.bat
```

## 하드웨어

- ESP32 개발보드
- 릴레이 3채널 (GPIO 5, 17, 16)
- 버튼 4개: 등록 버튼 1 + 전등 버튼 3 (GPIO 3, 23, 22, 1)
- 상태 LED 4개 (GPIO 13, 21, 19, 18)

## 서버 API

| 메서드 | 경로 | 파라미터 | 설명 |
|---|---|---|---|
| POST | `/auth/register` | username, password | 회원가입 (비밀번호는 werkzeug 해시 저장) |
| POST | `/auth/login` | username, password | 로그인 (세션) |
| POST | `/auth/registerSwitch` | username, password, mac, type | 스위치 기기 등록 |
| POST | `/auth/registerMoodLight` | username, password, mac, r1 | 무드등 기기 등록 |

DB는 SQLite이며 `user`, `switch`(채널별 on/off 상태 r1~r3), `moodLight` 테이블을 씁니다. (`server/flaskr/schema.sql`)

## 실행

**서버**
```bash
cd server
pip install flask
set FLASK_APP=flaskr
flask init-db        # 최초 1회
flask run --host=0.0.0.0
```

**펌웨어** (Arduino IDE, ESP32 보드 매니저)
- 라이브러리: `ESPAsyncWebServer`, `AsyncTCP`
- 펌웨어 안의 `YOUR_SERVER_IP`, `YOUR_AP_PASSWORD`, 테스트 스케치의 `YOUR_WIFI_SSID`/`YOUR_WIFI_PASSWORD`를 본인 환경에 맞게 바꿔서 업로드하세요.

## 알려진 한계 (당시 그대로 보존)

- 학기 중 MQTT로 원격 제어까지 붙이려 했으나 완성하지 못해 `iot_switch.ino`의 MQTT 부분과 `sendRlyCon()`이 비어 있습니다. 이 부분은 이후 프로젝트 [lithophane-moodlight](https://github.com/jwj-98/lithophane-moodlight)에서 MQTT 구독으로 구현했습니다.
- `registerMoodLight`가 `moodLight`가 아닌 `user` 테이블에 INSERT 합니다.
- 로그인 성공 시 게시판(`board.lists`)으로 리다이렉트하는데, 게시판은 최종본에서 빠져 있습니다.
- `SECRET_KEY='dev'`는 Flask 튜토리얼 기본값입니다. 실제 배포 시에는 `instance/config.py`로 분리해야 합니다.
- 서버와 기기 사이 통신이 HTTP 평문입니다.

## 기술 스택

ESP32 (Arduino) · ESPAsyncWebServer · Python Flask · SQLite · Bootstrap
