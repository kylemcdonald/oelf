package bend;

import mp3.*;

public interface Bend {
	public static final int absolute = 0;
	public static final int relative = 1;
	void bend(BitString bits, float time);
}
