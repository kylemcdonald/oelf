package mp3;

import java.util.*;
import processing.core.*;

public class Mp3 {
	
	public final static boolean debug = false;
	
	PApplet applet;
	BitStringStack bytes;
	public ArrayList<Frame> frames;
	
	public Mp3(PApplet applet) {
		this.applet = applet;
		frames = new ArrayList<Frame>();
	}
	
	public int bytesLength() {
		return bytes.byteLength();
	}
	
	public void readFile(String filename) {
		bytes = new BitStringStack(applet.loadBytes(filename));
		updateFrames();
	}
	
	public void saveFile(String filename) {
		updateBits();
		applet.saveBytes(filename, bytes.getBytes());
	}

	private void updateFrames() {
		BitStringStack reservoir = new BitStringStack();
		while(true) { // import all available frames
			try {
				Frame frame = new Frame(bytes, reservoir);
				frames.add(frame);

				if(Mp3.debug) {
					BitString reservoirOut = new BitString();
					for(Frame curFrame : frames)
						reservoirOut.add(curFrame.sideInfo.writeMainData());
					PApplet.println("Current reservoir output:\n" + reservoirOut.toHex());
				}
					
				bytes.seek(frame.nextSyncword());
				if(Mp3.debug || (frames.size() & 1023) == 0)
					PApplet.println(bytes.position() + "/" + bytes.bitLength()+ ", @" + frames.size() + " moved forward by " + frame.nextSyncword() + " bits.");
			} catch(Exception e) {
				System.out.println("Finished at: " + bytes.position() + "/" + bytes.bitLength());
				if(Mp3.debug)
					e.printStackTrace();
				break;
			}
		}
	}
	
	private void updateBits() {
		ArrayList<BitString> frameBits = new ArrayList<BitString>();
		int totalBytes = 0;
		Frame curFrame = frames.get(0);
		for(int i = 0; i < frames.size(); i++){
			Frame frame = frames.get(i);
			if(frame.unique)
				curFrame = frame;
			BitString cur = curFrame.write();
			frameBits.add(cur);
			totalBytes += cur.byteLength();
			if(Mp3.debug || (i & 1023) == 0)
				PApplet.println(bytes.bitLength()+ ", @" + i + "/" + frames.size() + " written.");
		}
		byte[] byteBuffer = new byte[totalBytes];
		int offset = 0;
		for(BitString bits : frameBits) {
			System.arraycopy(bits.getBytes(), 0, byteBuffer, offset, bits.byteLength());
			offset += bits.byteLength();
		}
		bytes = new BitStringStack(byteBuffer);
	}

	public byte getByte(int position) {
		return bytes.getByte(position);
	}
}
