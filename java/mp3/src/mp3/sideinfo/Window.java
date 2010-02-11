package mp3.sideinfo;

import java.util.*;
import mp3.*;
import mp3.maindata.*;

public abstract class Window extends ArrayList<ScaleFactorBand> {
	protected Granule granule;
	protected BitString huffman;
	public ArrayList<BitString> tableSelect = new ArrayList<BitString>();
	abstract void readWindow(BitStringStack bits);
	abstract public BitString write();
	abstract public int blockType();
	
	/*
	 * Reads the main data (part 2) for the general case. The bit lengths
	 * of the various scalefactor bands are defined by slen1 and slen2
	 * which, in turn, are defined by scalefac_compress.
	 * (iso p 21, 28)
	 */
	public int readMainData(BitStringStack reservoir) {
		int start = reservoir.position();
		BitString scfsi = granule.channel.scfsi;
		// TODO: tighten the connection between scfsi and sfb existence
		if(!scfsi.get(0) || granule.isFirst())
			for(int sfb = 0; sfb < 6; sfb++)
				add(new ScaleFactorBand(reservoir, granule.slen1(), sfb));
		if(!scfsi.get(1) || granule.isFirst())
			for(int sfb = 6; sfb < 11; sfb++)
				add(new ScaleFactorBand(reservoir, granule.slen1(), sfb));
		if(!scfsi.get(2) || granule.isFirst())
			for(int sfb = 11; sfb < 16; sfb++)
				add(new ScaleFactorBand(reservoir, granule.slen2(), sfb));
		if(!scfsi.get(3) || granule.isFirst())
			for(int sfb = 16; sfb < 21; sfb++)
				add(new ScaleFactorBand(reservoir, granule.slen2(), sfb));
		int end = reservoir.position();
		int length = end - start;
		if(Mp3.debug)
			System.out.println("Read Normal Window: '" + reservoir.substring(-length, length).toHex() + "'");
		return length;
	}
	
	public BitString writeMainData() {
		BitString out = new BitString();
		for(ScaleFactorBand sfb : this)
			out.add(sfb.write());
		out.add(huffman);
		return out;
	}
	
	public void readHuffman(BitStringStack reservoir, int part2Length) {
		huffman = reservoir.read(granule.part23Length() - part2Length);
	}
}
