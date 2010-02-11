package bend;

import processing.core.*;
import mp3.*;

public class SineBend implements Bend {
	int min, max, n;
	float period;
	
	public SineBend(int min, int max, int n, float period) {
		this.min = min;
		this.max = max;
		this.n = n;
		this.period = period;
	}

	public void bend(BitString bits, float time) {
		int value = (int) PApplet.map(
				(float) Math.sin(time * Math.PI * 2 / period), -1, +1, min, max);
		bits.set(BitString.fromInt(value, n));
	}

}
