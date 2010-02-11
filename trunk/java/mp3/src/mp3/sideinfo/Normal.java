package mp3.sideinfo;

import java.util.*;
import mp3.*;

public class Normal extends Window {
	static final BitString windowSwitchingFlag = new BitString("0");
	ArrayList<BitString> regionAddress = new ArrayList<BitString>();
	
	Normal(Granule granule) {
		this.granule = granule;
	}
	
	/*
	 * Read 22 bits of window data from side info.
	 */
	void readWindow(BitStringStack bits) {
		for(int i = 0; i < 3; i++)
			tableSelect.add(bits.read(5));
		regionAddress.add(bits.read(4));
		regionAddress.add(bits.read(3));
	}

	public BitString write() {
		BitString out = new BitString();
		out.add(windowSwitchingFlag);
		for(int i = 0; i < tableSelect.size(); i++)
			out.add(tableSelect.get(i));
		for(int i = 0; i < regionAddress.size(); i++)
			out.add(regionAddress.get(i));
		return out;
	}
	
	public int blockType() {
		return 0; // "If blocksplit_flag is not set, then the value of block_type is zero." iso p28
	}
}
