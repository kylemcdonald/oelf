package mp3.sideinfo;

import mp3.*;

public class Granule {
	Channel channel;
	private int position;
	
	public BitString part23Length; // bubbles, turning into chopper
	public BitString bigValues; // whammy/wahwah effect, only for 0-255 really
	public BitString globalGain; // gain of the entire granule
	public BitString scalefacCompress; // bubbles
	public Window window;
	public BitString preflag; // induces local compression artifacts
	public BitString scalefacScale; // no obvious contribution
	public BitString count1TableSelect; // no obvious contribution

	public Granule(Channel channel, BitStringStack bits, int position) {
		this.channel = channel;
		this.position = position;

		part23Length = bits.read(12);
		bigValues = bits.read(9);
		globalGain = bits.read(8);
		scalefacCompress = bits.read(4);

		window = bits.read(1).isTrue() ? new Switched(this) : new Normal(this);
		window.readWindow(bits);

		preflag = bits.read(1);
		scalefacScale = bits.read(1);
		count1TableSelect = bits.read(1);
	}
	
	public BitString write() {
		BitString out = new BitString();
		
		out.add(part23Length);
		out.add(bigValues);
		out.add(globalGain);		
		out.add(scalefacCompress);
		out.add(window.write());
		out.add(preflag);
		out.add(scalefacScale);
		out.add(count1TableSelect);
		
		return out;
	}
	
	// second granule only valid if scfsi = 0 for all bands
	public int part2Length() {
		if(window.blockType() == 2) { // iso p23
			if(((Switched) window).mixedBlockFlag.isTrue())
				return 17 * slen1() + 18 * slen2();
			else
				return 18 * slen1() + 18 * slen2();
		} else {
			return 11 * slen1() + 10 * slen2();
		}
	}
	
	public int part3Length() {
		return part23Length() - part2Length();
	}
	
	public int part23Length() {
		return part23Length.asInt();
	}
	
	private static final int[] slen1Values = {0,0,0,0,3,1,1,1,2,2,2,3,3,3,4,4};
	int slen1() {
		return slen1Values[scalefacCompress.asInt()];
	}

	private static final int[] slen2Values = {0,1,2,3,0,1,2,3,1,2,3,1,2,3,2,3};
	int slen2() {
		return slen2Values[scalefacCompress.asInt()];
	}
	
	public String toString() {
		return "g " + part23Length + " " + bigValues + " " + globalGain + "...";
	}
	
	boolean isFirst() {
		return position == 0;
	}

	public void readMainData(BitStringStack reservoir) {
		int part2Length = window.readMainData(reservoir);
		window.readHuffman(reservoir, part2Length);
		BitString out = writeMainData();
		if(Mp3.debug) {
			System.out.println("Granule output:\n" + out.toHex());
			System.out.println(
					"[part23Length: " + part23Length.asInt() +
					", mainData.bitLength(): " + out.bitLength() +
					", part2Length: " + part2Length + "]");
		}
	}

	public BitString writeMainData() {
		return window.writeMainData();
	}
}
