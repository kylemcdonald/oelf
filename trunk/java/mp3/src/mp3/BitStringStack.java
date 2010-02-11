package mp3;

import java.util.*;

public class BitStringStack extends BitString {
	private Stack<Integer> stack = new Stack<Integer>();
	private int position = 0;
	
	public BitStringStack() {
		super();
	}
	
	public BitStringStack(byte[] bytes) {
		super(bytes);
	}
	
	public BitStringStack(String string){
		super(string);
	}

	public void push() {
		stack.push(position);
	}
	
	public int pop() {
		position = stack.pop();
		return position;
	}
	
	public void set(int rposition, boolean newBit) {
		super.set(position + rposition, newBit);
	}
	
	public boolean get(int i) {
		return super.get(position + i);
	}
	
	public BitString read(int length) {
		BitString out = substring(0, length);
		seek(length);
		return out;
	}
	
	public BitString substring(int start, int length) {
		return super.substring(position + start, length);
	}
	
	public void jumpTo(BitString find) throws Exception {
		position = indexOf(0, find);
	}
	
	public int indexOf(int start, BitString find) throws Exception {
		return super.indexOf(position + start, find);
	}
	
	public void seek(int i) {
		position += i;
	}
	
	public int position() {
		return position;
	}

	public void seekEnd() {
		position = size;
	}
}
