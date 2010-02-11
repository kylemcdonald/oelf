package mp3.sideinfo;

import java.util.*;
import mp3.*;

public class SideInfo extends ArrayList<Channel> {
	public BitString mainDataBegin;
	Frame frame;
	
	public SideInfo(BitStringStack bits, Frame frame) {
		this.frame = frame;
		bits.push();
		int startPosition = bits.position();
		for(int ch = 0; ch < (frame.header.mono() ? 1 : 2); ch++)
			add(new Channel());
		mainDataBegin = bits.read(9);
		bits.seek(privateBits().bitLength()); // skip private bits
		for(int ch = 0; ch < size(); ch++)
			get(ch).readScfsi(bits);			
		for(int gr = 0; gr < 2; gr++)
			for(int ch = 0; ch < size(); ch++)
				get(ch).readGranule(bits);
		int stopPosition = bits.position();
		bits.pop();
		if(Mp3.debug) {
			System.out.println("Input side info:  " + bits.substring(0, stopPosition - startPosition).toHex() + " (" + (stopPosition - startPosition) + ")");
			System.out.println("Output side info: " + write().toHex() + " (" + write().bitLength() + ")");
		}
	}
	
	public int mainDataBegin() {
		return mainDataBegin.asInt() * 8;
	}

	final static BitString monoPrivateBits = new BitString(5);
	final static BitString stereoPrivateBits = new BitString(3);
	public BitString privateBits() {
		if(frame.header.mono())
			return monoPrivateBits;
		return stereoPrivateBits;
	}
	
	public BitString write() {
		BitString out = new BitString();

		out.add(mainDataBegin);
		out.add(privateBits());
		for(int ch = 0; ch < size(); ch++)
			out.add(get(ch).scfsi);
		for(int gr = 0; gr < 2; gr++)
			for(int ch = 0; ch < size(); ch++)
				out.add(get(ch).get(gr).write());
		return out;
	}
	
	public BitString writeMainData() {
		BitString mainData = new BitString();
		for(int gr = 0; gr < 2; gr++)
			for(int ch = 0; ch < size(); ch++)
				mainData.add(get(ch).get(gr).writeMainData());
		return mainData;
	}
	
	public int length() {
		return size() == 1 ? 136 : 256; // "17 bytes in mode single_channel and 32 bytes in the other modes"
	}
	
	public String toString() {
		return "[" + write().toString() + "]";
	}

	public void readMainData(BitStringStack reservoir) {
		if(Mp3.debug)
			System.out.print("Moving back " + mainDataBegin() + " bits from " + reservoir.position() + " to mainDataBegin... ");
		reservoir.seek(-mainDataBegin());
		if(Mp3.debug)
			System.out.println("now at " + reservoir.position());
		for(int gr = 0; gr < 2; gr++)
			for(int ch = 0; ch < size(); ch++)
				get(ch).get(gr).readMainData(reservoir);
	}
}
