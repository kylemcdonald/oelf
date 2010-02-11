package bend;

import mp3.*;

public class ToggleBend implements Bend {
	int on, off, n;
	int mode;
	float frequency;
	public ToggleBend(int on, int off, int n, float frequency) {
		this(on, off, n, frequency, absolute);
	}
	public ToggleBend(int on, int off, int n, float frequency, int mode) {
		this.on = on;
		this.off = off;
		this.n = n;
		this.frequency = frequency;
		this.mode = mode;
	}
	
	public void bend(BitString bits, float time) {
		boolean state = (time % frequency) / frequency < .5f;
		BitString cur;
		if(mode == absolute) {
			if(state)
				cur = BitString.fromInt(on, n);
			else
				cur = BitString.fromInt(off, n);
		} else {
			if(state)
				cur = BitString.fromInt(bits.asInt() + on, n);
			else
				cur = BitString.fromInt(bits.asInt() + off, n);
		}
		bits.set(cur);
	}

}
