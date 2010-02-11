import java.io.*;
import bend.*;
import processing.core.*;
import mp3.*;
import mp3.sideinfo.*;

public class BendMp3 extends PApplet {
	public static void main(String args	[]) {
		PApplet.main(new String[]{"BendMp3"});
	}
	
	public void setup() {
		size(128, 128);
		
		Mp3 mp3 = new Mp3(this);
		mp3.readFile("433.mp3");
				
		float[] t = {
				8.28f, 21.72f, 30f,
				74.1f, 101.4f, 145.1f, 173f,
				193.6f, 206.33f, 219.06f, 239.66f, 260.26f, 273f};
		
		//

		BendTimeline gainTrack = new BendTimeline();	
		
		gainTrack.addBend(new FixedBend(160, 8), t[0]);
		gainTrack.addBend(new NoiseBend(this, 128, 190, 8, 1f), t[1]);
		gainTrack.addBend(new NoiseBend(this, 138, 190, 8, 4f), t[2]);
		
		gainTrack.addBend(new NoiseBend(this, 138, 200, 8, .5f), t[3]);
		gainTrack.addBend(new RandomBend(128, 180, 8, 0f), t[4]);
		gainTrack.addBend(new NoiseBend(this, 138, 190, 8, 3f), t[5]);
		gainTrack.addBend(new RandomBend(138, 240, 8, .01f), t[6]);
		
		gainTrack.addBend(new ToggleBend(160, 170, 8, 2f), t[7]);
		gainTrack.addBend(new RandomBend(130, 256, 8, .1f), t[8]);
		gainTrack.addBend(new NoiseBend(this, 140, 160, 8, .1f), t[9]);
		gainTrack.addBend(new NoiseBend(this, 140, 180, 8, 1f), t[10]);
		gainTrack.addBend(new RandomBend(138, 256, 8, 0f), t[11]);
		gainTrack.addBend(new FixedBend(160, 8), t[12]);
		
		
		//
		
		BendTimeline bigTrack = new BendTimeline();
		
		bigTrack.addBend(new FixedBend(128, 9), t[0]);
		bigTrack.addBend(new NoiseBend(this, 0, 320, 9, 1f), t[1]);
		bigTrack.addBend(new ToggleBend(128, 256, 9, .01f), t[2]);
	
		bigTrack.addBend(new SineBend(0, 256, 9, 10f), t[3]);
		bigTrack.addBend(new SineBend(0, 256, 9, .1f), t[4]);
		bigTrack.addBend(new NoiseBend(this, 0, 320, 9, 8f), t[5]);
		bigTrack.addBend(new RandomBend(0, 320, 9, .33f), t[6]);
		
		bigTrack.addBend(new RandomBend(0, 511, 9, 0f), t[7]);
		bigTrack.addBend(new RandomBend(128, 320, 9, 1.66f), t[8]);
		bigTrack.addBend(new RandomBend(128, 160, 9, 2.33f), t[9]);
		bigTrack.addBend(new SineBend(128, 320, 9, 10f), t[10]);
		bigTrack.addBend(new FixedBend(320, 9), t[11]);
		bigTrack.addBend(new FixedBend(128, 9), t[12]);
		
		//
		
		BendTimeline tableTrack = new BendTimeline();
		tableTrack.addBend(new FixedBend(0, 5, Bend.relative), t[0]);
		tableTrack.addBend(new FixedBend(1, 5, Bend.relative), t[1]);
		tableTrack.addBend(new ToggleBend(-12, 4, 5, .33f, Bend.relative), t[2]);
		
		tableTrack.addBend(new SineBend(18, 23, 5, 16.18f), t[3]);
		tableTrack.addBend(new ToggleBend(20, 23, 5, 1.618f), t[4]);
		tableTrack.addBend(new NoiseBend(this, -12, 12, 5, 16.18f), t[5]);
		tableTrack.addBend(new SineBend(20, 25, 5, 16.18f), t[6]);

		tableTrack.addBend(new SineBend(0, 32, 5, .0618f), t[7]);
		tableTrack.addBend(new SineBend(0, 32, 5, .0161f), t[8]);
		tableTrack.addBend(new SineBend(0, 32, 5, .618f), t[9]);
		tableTrack.addBend(new SineBend(0, 32, 5, .161f), t[10]);
		tableTrack.addBend(new SineBend(0, 32, 5, 1.61f), t[11]);
		tableTrack.addBend(new FixedBend(0, 5, Bend.relative), t[12]);
		
		float startTime = t[0];
		for(Frame frame : mp3.frames) {
			float curTime = gainTrack.curTime();
			SideInfo sideInfo = frame.sideInfo;
			//frame.mainData.randomize();
			if(curTime > startTime) {
				for(int i = 0; i < 8; i++) {
					int cur = (int) random(0, frame.mainData.bitLength());
					frame.mainData.flip(cur);
				}
			}
			//frame.sideInfo.mainDataBegin.randomize();
			for(Channel channel : sideInfo) {
				for(Granule granule : channel) {
					gainTrack.bend(granule.globalGain);
					bigTrack.bend(granule.bigValues);
					/*
					granule.part23Length.randomize();
					for(ScaleFactorBand sfb : window) {
						sfb.value.randomize(); // no significant effect?
					}
					*/
					Window window = granule.window;
					if(curTime > startTime) {
						if(random(1) < .5) {
							granule.preflag.randomize();
							granule.scalefacCompress.randomize();
						}
						for(BitString bits : window.tableSelect) { 
							tableTrack.bend(bits);
						}
					}
					/*
					if(window instanceof Switched) {
						((Switched) window).blockType.randomize(); // a little ring mod
						((Switched) window).mixedBlockFlag.randomize(); // a little... something?
					}
					*/
				}
			}
			if(curTime > startTime && 
					(noise(curTime) > .618 || curTime < startTime + 1))
				frame.unique = false;
			gainTrack.nextFrame();
			bigTrack.nextFrame();
			tableTrack.nextFrame();
		}
		String filename = System.currentTimeMillis() + ".mp3";
		mp3.saveFile(filename);
		
		try {
			Runtime.getRuntime().exec("\"C:\\Program Files\\Winamp\\winamp.exe\" \"C:\\Users\\mcdonk\\workspace\\bendmp3\\" + filename + "\"");
		} catch(IOException e) {
			e.printStackTrace();
		}
	}
}
