package mp3.sideinfo;

import java.util.*;
import mp3.*;
import mp3.maindata.*;

public class Switched extends Window {
	static final BitString windowSwitchingFlag = new BitString("1");
	public BitString blockType;
	public BitString mixedBlockFlag;
	ArrayList<BitString> subblockGain = new ArrayList<BitString>();
	
	Switched(Granule granule) {
		this.granule = granule;
	}
	
	/*
	 * When this is a switching window (i.e., blocksplit_flag is true
	 * and the block type is 2, read the main data. If the block type
	 * is not 2, go to the default readMainData defined in Window.
	 * (iso p 21)
	 */
	public int readMainData(BitStringStack reservoir) {
		if(blockType() == 2) {
			int start = reservoir.position();
			if(mixedBlockFlag.isTrue()) {
				for(int sfb = 0; sfb < 8; sfb++)
					add(new ScaleFactorBand(reservoir, granule.slen1(), sfb));
				for(int sfb = 3; sfb < 12; sfb++) {
					int length = sfb < 6 ? granule.slen1() : granule.slen2();
					for(int win = 0; win < 3; win++)
						add(new ShortScaleFactorBand(reservoir, length, sfb, win));
				}
			} else {
				for(int sfb = 0; sfb < 12; sfb++) {
					int length = sfb < 6 ? granule.slen1() : granule.slen2();
					for(int win = 0; win < 3; win++)
						add(new ShortScaleFactorBand(reservoir, length, sfb, win));
				}
			}
			int end = reservoir.position();
			int length = end - start;
			if(Mp3.debug)
				System.out.println("Read Switched Window: '" + reservoir.substring(-length, length).toHex() + "'");
			return length;
		} else {
			return super.readMainData(reservoir);
		}
	}
	
	/*
	 * Read 22 bits of window data from side info.
	 */
	void readWindow(BitStringStack bits) {
		blockType = bits.read(2);
		mixedBlockFlag = bits.read(1); // aka switchPoint
		for(int i = 0; i < 2; i++)
			tableSelect.add(bits.read(5));
		for(int i = 0; i < 3; i++)
			subblockGain.add(bits.read(3));
	}
	
	public BitString write() {
		BitString out = new BitString();
		out.add(windowSwitchingFlag);
		out.add(blockType);
		out.add(mixedBlockFlag);
		for(int i = 0; i < tableSelect.size(); i++)
			out.add(tableSelect.get(i));
		for(int i = 0; i < subblockGain.size(); i++)
			out.add(subblockGain.get(i));
		return out;
	}
	
	public int blockType() {
		return blockType.asInt();
	}
	
	// iso p29
	public int switchPointL() {
		return mixedBlockFlag.isTrue() ? 8 : 0;
	}
	
	// iso p29
	public int switchPointS() {
		return mixedBlockFlag.isTrue() ? 3 : 0;
	}
}
