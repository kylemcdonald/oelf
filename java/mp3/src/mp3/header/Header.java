package mp3.header;

import mp3.*;

public class Header {
	// 12 bit syncword + 1 id bit (MPEG-1) + 2 layer bits (layer 3)
	final static public BitString sync = new BitString("111111111111101");
	final static BitString protectionBit = new BitString("1");
	Bitrate bitrate;
	Frequency frequency;
	public boolean padding;
	static final BitString privateBit = new BitString("0");
	public BitString mode;
	BitString modeExtension; // only used in layer 1 (und mp3 p48)
	static final BitString copyrightBit = new BitString("0"); // uncopyrighted
	static final BitString originalBit = new BitString("1"); // original
	static final BitString emphasisBit = new BitString("00"); // no emphasis
	
	public Header(BitStringStack bits) throws Exception {
		try {
			bits.jumpTo(sync);
		} catch(Exception e) {
			throw new Exception("Can't find sync.");
		}
		if(!bits.get(15)) // raissi disagrees with iso, i trust iso
			throw new Exception("Contains CRC.");
		bits.push();
		if(Mp3.debug)
			System.out.println("Input header:  " + bits.substring(0, 32).toHex());
		bits.seek(12);
		bitrate = new Bitrate(bits.substring(4, 4));
		frequency = new Frequency(bits.substring(8, 2));
		padding = bits.get(10);
		mode = bits.substring(12, 2);
		modeExtension = bits.substring(14, 2);
		bits.pop();
		if(Mp3.debug)
			System.out.println("Output header: " + write().toHex());
	}
	
	public boolean mono() {
		return mode.equals("11");
	}
	
	public int bitRate() {
		return bitrate.getBitrate();
	}

	public float sampleRate() {
		return frequency.getFrequency() / 1000f;
	}
	
	public BitString write() {
		BitString out = new BitString();
		out.add(sync);
		out.add(protectionBit);
		out.add(bitrate.bits);
		out.add(frequency.bits);
		out.add(padding);
		out.add(privateBit);
		out.add(mode);
		out.add(modeExtension);
		out.add(copyrightBit);
		out.add(originalBit);
		out.add(emphasisBit);
		return out;
	}
	
	public int length() {
		return 32;
	}
	
	public String toString() {
		return "[" + bitRate() + " kbps " + sampleRate() + " KHz " + padding + " " + mode + " " + modeExtension + "]";
	}
}
