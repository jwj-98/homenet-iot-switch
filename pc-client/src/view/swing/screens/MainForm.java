package view.swing.screens;
import java.awt.Color;
import java.awt.Font;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.SwingConstants;

import control.MqttResister;
import view.swing.custom.rectlook.RectlookButton;
import view.swing.other.BottomPanel;
import view.swing.other.RainbowPanel;
import view.swing.other.TitlePanel;
import view.swing.utils.bo.CustomImageIcon;


interface MainFormInter{
	//MQTT 제어 정보를 PUBLISH 합니다
	public void pubMqtt(String content);
	
}


public class MainForm extends CPGeneral implements ActionListener, WindowListener, MainFormInter{
	String mac = "";
	
	MainForm(String mac){
		this.mac = mac;
	}
	
	@Override
	public void initComponents(){
		
		jmBarraMenu = new JMenuBar();
		jmBarraMenu.setBounds(777, 2, 800, 18);
		C.add(jmBarraMenu);
		
		//BEGIN - ARQUIVO

		jmiArquivoSair = new JMenuItem("X");
		Font font = new Font("HY견고딕", Font.TRUETYPE_FONT, 15);
		jmiArquivoSair.setFont(font);
		jmiArquivoSair.addActionListener(this);
		jmBarraMenu.add(jmiArquivoSair);
		//END - ARQUIVO
		
		RainbowPanel rbp = new RainbowPanel(new Rectangle(0, 20, 800, 15));
		C.add(rbp);
		
		TitlePanel tp = new TitlePanel(new Rectangle(0, 35, 800, 100));
		C.add(tp);
		
		JPanel content = new JPanel();
		content.setBounds(new Rectangle(0,135,800,340));
		content.setLayout(null);
		C.add(content);
		
		BottomPanel bp = new BottomPanel(new Rectangle(0,475,800,100));
		C.add(bp);
		
		JSeparator separator;
		separator = new JSeparator(SwingConstants.VERTICAL);
		separator.setBounds(175,0,1,getBounds().height);
		separator.setBackground(Color.LIGHT_GRAY);
		content.add(separator);
		
		RectlookButton btnDeviceResist = new RectlookButton("기기 등록", RectlookButton.INFO_TYPE);
		btnDeviceResist.setBounds(650,25,120,40);
		content.add(btnDeviceResist);
		
		RectlookButton btnNext = new RectlookButton(">", RectlookButton.DEFAULT_TYPE);
		btnNext.setBounds(575,150,50,50);
		content.add(btnNext);
		
		RectlookButton btnPre = new RectlookButton("<", RectlookButton.DEFAULT_TYPE);
		btnPre.setBounds(225,150,50,50);
		content.add(btnPre);
		
		JLabel notice = new JLabel("공지사항");
		notice.setFont(font);
		notice.setBounds(20,25,120,40);
		content.add(notice);
		
		JLabel con = new JLabel("IOT 제어");
		con.setFont(font);
		con.setBounds(20,65,120,40);
		content.add(con);
		
		JLabel device = new JLabel("기기 등록");
		device.setFont(font);
		device.setBounds(20,105,120,40);
		content.add(device);
		
		JButton btn1 = new JButton();
		btn1.setIcon(new CustomImageIcon("/view/images/switch_on.png", "switch", 40, 40));
		btn1.setBounds(405,95,40,40);
		btn1.setBackground(new Color(255, 255, 255));
		content.add(btn1);
		
		ActionListener listenerBtn1 = new ActionListener() {
					
				@Override
				public void actionPerformed(ActionEvent e) {
					pubMqtt("mac=" + mac + "&btnNum=" + 1);
				}
			};
			btn1.addActionListener(listenerBtn1);
		
		JButton btn2 = new JButton();
		btn2.setIcon(new CustomImageIcon("/view/images/switch_on.png", "switch", 40, 40));
		btn2.setBounds(405,150,40,40);
		btn2.setBackground(new Color(255, 255, 255));
		content.add(btn2);
		
		ActionListener listenerBtn2 = new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				pubMqtt("mac=" + mac + "&btnNum=" + 2);
			}
		};
		btn2.addActionListener(listenerBtn2);
		
		JButton btn3 = new JButton();
		btn3.setIcon(new CustomImageIcon("/view/images/switch_on.png", "switch", 40, 40));
		btn3.setBounds(405,205,40,40);
		btn3.setBackground(new Color(255, 255, 255));
		content.add(btn3);
		
		ActionListener listenerBtn3 = new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				pubMqtt("mac=" + mac + "&btnNum=" + 3);
			}
		};
		btn3.addActionListener(listenerBtn3);
		
		JLabel frame = new JLabel();
		frame.setIcon(new CustomImageIcon("/view/images/switch_frame.png", "switch", 200, 300));
		frame.setBounds(325,20,200,300);
		content.add(frame);
		
		
		
	}



	



	@Override
	public void pubMqtt(String content) {
		MqttResister mqtt = new MqttResister(content);
		Thread t = new Thread(mqtt);
		t.start();
	}
	
}
