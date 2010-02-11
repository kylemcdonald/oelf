package bend;

import processing.core.*;
import mp3.*;

public class NoiseBend implements Bend {	
	PApplet applet;
	int n, mode;
	float min, max, frequency;
	public NoiseBend(PApplet applet, float min, float max, int n, float frequency) {
		this(applet, min, max, n, frequency, absolute);
	}
	public NoiseBend(PApplet applet, float min, float max, int n, float frequency, int mode) {
		this.applet = applet;
		this.min = min;
		this.max = max;
		this.n = n;
		this.frequency = frequency;
		this.mode = mode;
	}
	
	public void bend(BitString bits, float time) {
		int value = (int) PApplet.map(applet.noise(time * frequency), 0, 1, min, max);
		BitString cur;
		if(mode == absolute) {
			cur = BitString.fromInt(value, n);
		} else {
			cur = BitString.fromInt(value + bits.asInt(), bits.bitLength());
		}
		bits.set(cur);
	}
}
