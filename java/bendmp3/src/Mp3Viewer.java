import processing.core.*;
import controlP5.*;
import mp3.*;

public class Mp3Viewer extends PApplet {
	public static void main(String args	[]) {
		PApplet.main(new String[]{"Mp3Viewer"});
	}

	ControlP5 controlP5;
	boolean characterDisplay;

	PFont font;
	Mp3 mp3;

	float filePosition = 0;
	int bytesOffset = 0;
	int byteLeft = 1;
	int byteTop = 2;
	int byteWidth = 12;
	int byteHeight = 12;
	int bytesWide, bytesHigh;
	
	public void setup() {
		size(1280 / 2, 720 / 2, P2D);

		font = loadFont("TrebuchetMS-9.vlw");
		textMode(SCREEN);
		textFont(font);
		textAlign(LEFT, TOP);

		bytesWide = (int) (width / byteWidth);
		bytesHigh = (int) (height / byteHeight);

		mp3 = new Mp3(this);
		mp3.readFile("433.mp3");
		
		controlP5 = new ControlP5(this);
		controlP5.addToggle("characterDisplay", false, 10, 10, 10, 10);
		controlP5.addSlider("filePosition", 0, 1, 0, 25, 10, 100, 10);
	}

	public void draw() {
		background(192);

		int curOffset = bytesOffset + (int) (filePosition * mp3.bytesLength());
		
		for(int i = 0; i < bytesHigh; i++) {
			for(int j = 0; j < bytesWide; j++) {
				int position = i * bytesWide + j + curOffset;
				if(position >= 0 && position < mp3.bytesLength()) {
					char cur = (char) mp3.getByte(position);
					int x = j * byteWidth;
					int y = i * byteHeight;
					fill(cur);
					noStroke();
					rect(x, y, byteWidth, byteHeight);
					fill(cur < 128 ? 255 : 0);
					x += byteLeft;
					y += byteTop;
					if(characterDisplay) {
						text(cur, x, y);
					} else {
						text(hex(cur, 2), x, y);
					}
				}
			}
		}

		if(controlP5.isVisible()) {
			fill(128);
			noStroke();
			rect(5, 5, 180, 30);
			controlP5.draw();
		}
	}

	public void keyPressed() {
		if(keyCode == DOWN) bytesOffset += bytesHigh * bytesWide / 2;
		if(keyCode == UP) bytesOffset -=  bytesHigh * bytesWide / 2;
		if(key == 's') saveFrame("####-mp3viewer.png");
	}
}
