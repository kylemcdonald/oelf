package bend;

import mp3.*;

public class FixedBend implements Bend {
	int mode, n, value;
	
	public FixedBend(int value, int n) {
		this(value, n, absolute);
	}
	
	public FixedBend(int value, int n, int mode) {
		this.value = value;
		this.n = n;
		this.mode = mode;
	}

	public void bend(BitString bits, float time) {
		if(mode == absolute)
			bits.set(BitString.fromInt(value, n));
		else
			bits.set(BitString.fromInt(value + bits.asInt(), n));
	}

}
