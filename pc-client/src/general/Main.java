package general;

import view.swing.screens.LogInForm;
import view.swing.utils.AlterFonts;

public class Main {
	
	public static void main(String[] x){
		AlterFonts.alterFonts();
		
		LogInForm cpGeneral = new LogInForm();
		cpGeneral.setUndecorated(true);
		cpGeneral.setVisible(true);
	}

}
