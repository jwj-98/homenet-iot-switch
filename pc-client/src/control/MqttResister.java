package control;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
 
public class MqttResister implements Runnable{
	String content;
	public MqttResister(String content) {
        this.content = content;
	}

	@Override
	public void run() {		
		final String MQTT_BROKER_IP = Config.MQTT_BROKER;
        
        try
        {
            MqttClient client = new MqttClient( 
                    MQTT_BROKER_IP, //URI 
                    MqttClient.generateClientId(), //ClientId 
                    new MemoryPersistence());
            MqttMessage message = new MqttMessage(content.getBytes());
            
            client.connect();
            client.publish("topic", message);
            
            client.disconnect();
        } 
        
        catch (MqttException e) {
            e.printStackTrace();
        } //Persistence
	}
}