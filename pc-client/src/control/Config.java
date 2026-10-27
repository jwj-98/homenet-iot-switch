package control;

// 서버 주소 설정. 실행 시 -Dserver.url=... -Dmqtt.broker=... 로 바꿀 수 있습니다.
public class Config {
	public static final String SERVER_URL = System.getProperty("server.url", "http://127.0.0.1:5000");
	public static final String MQTT_BROKER = System.getProperty("mqtt.broker", "tcp://127.0.0.1:1883");
}
