package mp3.maindata;

import mp3.*;

public class ShortScaleFactorBand extends ScaleFactorBand {
	int window;
	public ShortScaleFactorBand(BitStringStack bits, int length, int position, int window) {
		super(bits, length, position);
		this.window = window;
	}
}
