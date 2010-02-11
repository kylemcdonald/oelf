package bend;

import java.util.*;
import mp3.*;

public class BendTimeline {
	static final float secondsPerFrame = 0.026f;
	
	ArrayList<Bend> bends;
	ArrayList<Float> times;
	int frame;
	public BendTimeline() {
		bends = new ArrayList<Bend>();
		times = new ArrayList<Float>();
		frame = 0;
	}
	public void addBend(Bend bend, float time) {
		bends.add(bend);
		times.add(time);
	}
	public void bend(BitString bits) {
		float curTime = curTime();
		int i = 0;
		while(true) {
			if(times.size() == 0 ||
				i + 1 == times.size() ||
				curTime < times.get(i))
				break;
			i++;
		}
		if(i < bends.size())
			bends.get(i).bend(bits, curTime);
	}
	public void nextFrame() {
		frame++;
	}
	public float curTime() {
		return frame * secondsPerFrame;
	}
}
