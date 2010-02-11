package bend;

import mp3.*;

public class RandomBend implements Bend {
	float period, lastTime;
	BitString value;
	int min, max;
	public RandomBend(int min, int max, int n, float period) {
		this.min = min;
		this.max = max;
		this.period = period;
		value = new BitString(n);
		value.randomize();
		lastTime = 0;
	}
	
	public void bend(BitString bits, float time) {
		if(time % period < lastTime % period || period == 0)
			value.randomize(min, max);
		bits.set(value);
		lastTime = time;
	}

}
