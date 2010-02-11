package mp3.header;

import mp3.*;

public class Frequency {
	BitString bits;
	
	public Frequency(BitString bits) throws Exception {
		if(bits.asInt() == 3)
			throw new Exception("Invalid frequency bits " + bits);
		this.bits = bits;
	}

	public static final int[] frequencies = new int[]{44100, 48000, 32000};
	
	public int getFrequency() {
		return frequencies[bits.asInt()];
	}
}
