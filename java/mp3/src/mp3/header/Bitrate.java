package mp3.header;

import mp3.*;

public class Bitrate {
	BitString bits;
	
	public Bitrate(BitString bits) throws Exception {
		if(bits.equals("0000") ||
				bits.equals("1111"))
			throw new Exception("Invalid bitrate bits " + bits);
		this.bits = bits;
		if(Mp3.debug)
			System.out.println("Bit rate: " + bits + " (" + getBitrate() + ")");
	}	

	final static int[] bitrates = new int[]{32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320};

	public int getBitrate() {
		return bitrates[bits.asInt() - 1];
	}
	
	public String toString() {
		return bits.toString();
	}
}
