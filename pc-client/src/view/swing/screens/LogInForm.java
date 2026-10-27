package view.swing.screens;
import java.awt.Color;
import java.awt.Font;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowListener;

import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;

import control.GetDeviceResister;
import control.LogInResister;
import view.swing.custom.rectlook.RectlookButton;
import view.swing.custom.rectlook.RectlookCheckBox;
import view.swing.custom.rectlook.RectlookFormField;
import view.swing.other.BottomPanel;
import view.swing.other.RainbowPanel;
import view.swing.other.TitlePanel;


interface LogInFormInter{
	//웹서버 통신 사용자 확인
	public String logInCheck(String id, String pw);
	//웹서버의 DB에서 기기정보를 가져옵니다
	public String[]device(String username);
	
}


public class LogInForm extends CPGeneral implements ActionListener, WindowListener, LogInFormInter{
	String strUserid = "";
	String strUserpw = "";
	
	@Override
	public String logInCheck(String id, String pw) {
		LogInResister l = new LogInResister(id, pw);
		return l.login();
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
		
		RectlookFormField userid = new RectlookFormField("/view/images/mail.png", "USER e-mail");
		userid.setBounds(150,20,480,60);
		content.add(userid);
		
		RectlookFormField userpw = new RectlookFormField("/view/images/profile-icon.png", "USER password");
		userpw.setBounds(150,100,480,60);
		content.add(userpw);
		
		RectlookCheckBox jcb = new RectlookCheckBox("로그인 유지", new Color(138, 147, 179), new Color(114, 132, 193));
		jcb.setBounds(150,180,700,60);
		content.add(jcb);
		
		RectlookButton jb2 = new RectlookButton("회원가입", RectlookButton.PRIMARY_TYPE);
		jb2.setBounds(480,265,150,50);
		content.add(jb2);
		
		RectlookButton jb1 = new RectlookButton("로그인", RectlookButton.SUCCESS_TYPE);
		jb1.setBounds(310,265,150,50);
		content.add(jb1);
		
		BottomPanel bp = new BottomPanel(new Rectangle(0,475,800,100));
		C.add(bp);

		ActionListener listener = new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				strUserid = userid.getText();
				strUserpw = userpw.getText();
				
				String request = logInCheck(strUserid, strUserpw);
				System.out.println(request);
				if(request.equals("ok")) {
					
					GetDeviceResister g = new GetDeviceResister(strUserid);
					String mac = g.execute();
					System.out.println(mac);
					MainForm m = new MainForm(mac);
					m.setUndecorated(true);
					m.setVisible(true);
					dispose();
				}
			}
		};
		
		jb1.addActionListener(listener);
	}

	@Override
	public String[] device(String username) {
		
		return null;
	}
	
}
