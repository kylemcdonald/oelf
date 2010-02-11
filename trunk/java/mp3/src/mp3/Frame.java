package mp3;

import mp3.header.*;
import mp3.sideinfo.*;

public class Frame {
	public Header header;
	public SideInfo sideInfo;
	
	public BitString mainData;
	public boolean unique = true;
	
	public Frame(BitStringStack bits, BitStringStack reservoir) throws Exception {
		header = new Header(bits); // load header & jump to sync
		
		bits.push(); // pause at the header
		
		bits.seek(header.length());
		
		// load side info
		sideInfo = new SideInfo(bits, this);
		bits.seek(sideInfo.length());
		
		// store bits in reservoir
		//reservoir.seekEnd();
		//reservoir.add(bits.read(nextSyncword() - length()));
		mainData = bits.read(nextSyncword() - length()); // really, inter-frame data
		
		if(Mp3.debug) {
			System.out.println("Reservoir @" + reservoir.position() + "/" + reservoir.bitLength() + " is currently:\n" + reservoir.substring(0, 32).toHex() + "...");
			System.out.println("Entire reservoir is:\n" + reservoir.toHex());
		}
		
		//sideInfo.readMainData(reservoir);
		
		bits.pop(); // return to the header
		
		if(Mp3.debug) {	
			System.out.println("Output main data (" + mainData.bitLength() + "):\n" + mainData.toHex());
			System.out.println("Frame size vs next syncword " + nextSyncword());
			System.out.println("Starting at the header, here are the next 8192 bits:\n" + bits.substring(0, 8192).toHex());
		}
	}
	
	/*
	 * Number of bits between start of this and the next syncword.
	 * Also called "frame size" when referring to the number of bytes.
	 */
	public int nextSyncword() {
		int paddingByte = header.padding ? 1 : 0;
		int mainBytes = (int) ((1152 * header.bitRate()) / (8 * header.sampleRate()));
		return 8 * (paddingByte + mainBytes);
	}
	
	public BitString write() {
		BitString out = new BitString();
		out.add(header.write());
		out.add(sideInfo.write());
		out.add(mainData);
		return out;
	}
	
	public int length() {
		return header.length() + sideInfo.length();
	}
	
	public String toString() {
		return "{" + header + " " + sideInfo + "}";
	}
}
