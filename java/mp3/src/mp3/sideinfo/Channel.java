package mp3.sideinfo;

import java.util.*;
import mp3.*;

public class Channel extends ArrayList<Granule> {
	BitString scfsi;

	public Channel() {
	}
	
	public void readScfsi(BitStringStack bits) {
		scfsi = bits.read(4);
	}
	
	public void readGranule(BitStringStack bits) {
		add(new Granule(this, bits, size()));
	}

	public String toString() {
		return "(c " + scfsi + " " + super.toString() + ")";
	}
}