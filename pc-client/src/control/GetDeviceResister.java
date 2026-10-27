package control;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;


public class GetDeviceResister {
String sendMsg, receiveMsg, str, id, pw;
	
	public GetDeviceResister(String id){
		this.id = id;
	}
	
	public String execute() {
		try {
		    // 접속할 서버 주소 
		    URL url = new URL(Config.SERVER_URL + "/java");
		
		    HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		    conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
		    conn.setDefaultUseCaches(false);
		    conn.setDoInput(true);                         // 서버에서 읽기 모드 지정
		    conn.setDoOutput(true);                       // 서버로 쓰기 모드 지정
		    conn.setRequestMethod("POST");                // 전송방식 POST
		    OutputStreamWriter osw = new OutputStreamWriter(conn.getOutputStream(), "UTF-8");
		
		    // 전송할 데이터. GET 방식으로 작성후 post 전송
		    sendMsg = "username=" + id;
		
		    osw.write(sendMsg);
		    osw.flush();
		    
		    // 통신 성공 시 수행
            if (conn.getResponseCode() == conn.HTTP_OK) {
                InputStreamReader tmp = new InputStreamReader(conn.getInputStream(), "UTF-8");
                BufferedReader reader = new BufferedReader(tmp);
                StringBuffer buffer = new StringBuffer();

                //  보낸 값을 받는 부분
                while ((str = reader.readLine()) != null) {
                    buffer.append(str);
                }
                receiveMsg = buffer.toString();
            } else {
                // 통신 실패
            }
			}catch(Throwable e) {
				e.printStackTrace();
			}
		return receiveMsg;
	}
}
