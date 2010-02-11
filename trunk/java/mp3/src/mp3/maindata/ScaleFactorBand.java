package mp3.maindata;

import mp3.*;

/*
 * A scale factor band controls the gain within one of the bands.
 */
public class ScaleFactorBand {
	public BitString value;
	int position;
	public ScaleFactorBand(BitStringStack bits, int length, int position) {
		this.value = bits.read(length);
		this.position = position;
	}
	
	public BitString write() {
		return value;
	}
}
