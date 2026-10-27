package view.swing.other;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.RenderingHints;

import javax.swing.JPanel;

public class RainbowPanel extends JPanel{
	
	private static final long serialVersionUID = 1L;

	private Color colors = new Color(51, 153, 204);
	
	public RainbowPanel(Rectangle bounds){
		setLayout(null);
		setBounds(bounds);
		
	}
	
	public RainbowPanel(){
		setLayout(null);
		
	}
	
    @Override
    protected void paintComponent(final Graphics pG) {
        super.paintComponent(pG);

        final Graphics2D g = (Graphics2D) pG;
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
        
        int posX = 0;

			g.setColor(colors);
			g.fillRect(posX, 0, 800, 15);


    }
}
