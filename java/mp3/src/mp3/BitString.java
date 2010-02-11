package mp3;

import processing.core.*;

/*
 * If BitString were instead implemented with ints instead of bytes
 * a bunch of operations could be sped up significantly.
 */
public class BitString {
	protected byte[] bytes = {};
	protected int size = 0;

	public BitString() {
	}

	public BitString(String string) {
		resize(string.length());
		for(int i = 0; i < string.length(); i++)
			setAbs(i, string.charAt(i) != '0');
	}

	public BitString(int length) {
		resize(length);
	}
	
	public BitString(byte[] bytes) {
		this.bytes = bytes;
		this.size = bytes.length * 8;
	}

	/*
	 * Add a bit to the end of the BitString.
	 */
	public void add(boolean newBit) {
		resize(size + 1);
		setAbs(size - 1, newBit);
	}
	
	/*
	 * Add a BitString to the end of this BitString.
	 */
	public void add(BitString bitstring) {
		int bitOffset = size & 7; // i.e., %8
		if(bitOffset == 0) {
			int aLength = bytes.length;
			int bLength = bitstring.bytes.length;
			byte[] newBytes = new byte[aLength + bLength];
			System.arraycopy(bytes, 0, newBytes, 0, aLength);
			System.arraycopy(bitstring.bytes, 0, newBytes, aLength, bLength);
			bytes = newBytes;
			size += bitstring.size;
		} else {
			for(int i = 0; i < bitstring.size; i++)
				add(bitstring.get(i));
		}
	}
	
	public byte getByte(int position) {
		return bytes[position];
	}
	
	public boolean get(int position) {
		return getAbs(position);
	}
	
	public void set(byte value) {
		if(bytes.length > 0)
			bytes[0] = value;
	}
	
	public void set(BitString bits) {
		bytes = new byte[bits.bytes.length];
		System.arraycopy(bits.bytes, 0, bytes, 0, bits.bytes.length);
		size = bits.size;
	}
	
	public void set(int position, boolean newBit) {
		setAbs(position, newBit);
	}
	
	private boolean getAbs(int position) {
		int bytePosition = position >> 3; // i.e., /8
		int bitOffset = position & 7; // i.e., %8
		return (bytes[bytePosition] & (128 >> bitOffset)) != 0;
	}
	
	public void setAbs(int position, boolean newBit) {
		int bytePosition = position >> 3;
		int bitOffset = position & 7;
		if(newBit)
			bytes[bytePosition] |= 128 >> bitOffset;
		else
			bytes[bytePosition] &= ~(128 >> bitOffset);
	}
	
	public int bitLength() {
		return size;
	}
	
	public int byteLength() {
		return bytes.length;
	}
	
	public byte[] getBytes() {
		return bytes;
	}
	
	public BitString substring(int start, int substringLength) {
		BitString substring = new BitString(substringLength);
		for(int i = 0; i < substringLength; i++)
			substring.setAbs(i, getAbs(start + i));
		return substring;
	}
	
	private void resize(int newLength) {
		size = newLength;
		int byteLength = (int) Math.ceil(size / 8f);
		if(byteLength != bytes.length) {
			byte[] newBytes = new byte[byteLength];
			System.arraycopy(bytes, 0, newBytes, 0, Math.min(bytes.length, newLength));
			this.bytes = newBytes;
		}
	}
	
	private final static char[] hex = 
	{	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E', 'F'};
	public String toHex() {
		String out = "";
		for(int i = 0; i < bytes.length; i++) {
			out += hex[(bytes[i] >> 4) & 0xF];
			out += hex[(bytes[i] & 0xF)];
			boolean last = i == bytes.length - 1;
			if((i + 1) % 32 == 0 && !last)
				out += '\n';
			else if((i + 1) % 4 == 0 && !last)
				out += ' ';
		}
		return out;
	}
	
	public String toString() {
		String out = "";
		for(int i = 0; i < size; i++)
			out += get(i) ? '1' : '0';
		return out;
	}
	
	public String asBytes() {
		String out = "";
		for(int i = 0; i < bytes.length; i++)
			out += (char) bytes[i];
		return out;
	}
	
	public boolean isTrue() {
		return get(0);
	}

	public int asInt() {
		int out = 0;
		int intLength = Math.min(size, 32);
		for(int i = 0; i < intLength; i++)
			out |= (getAbs(intLength - i - 1) ? 1 : 0) << i;
		return out;
	}
	
	public int indexOf(BitString find) throws Exception {
		return indexOf(0, find);
	}
	
	public int indexOf(int start, BitString find) throws Exception {
		int inputLength = find.size;
		int maxPosition = size - inputLength + 1;
		for(int i = start; i < maxPosition; i++) {
			boolean matches = true;
			for(int j = 0; j < inputLength; j++) {
				if(getAbs(i + j) != find.getAbs(j)) {
					matches = false;
					break;
				}
			}
			if(matches)
				return i;
		}
		throw new Exception("Bit string not found.");
	}
	
	public boolean equals(String bits) {
		return this.equals(new BitString(bits));
	}
	
	public boolean equals(BitString compare) {
		if(compare.size != size)
			return false;
		for(int i = 0; i < bytes.length; i++)
			if(bytes[i] != compare.bytes[i])
				return false;
		return true;
	}

	public static BitString random(int bits) {
		BitString random = new BitString(bits);
		for(int i = 0; i < bits; i++)
			random.set(i, Math.random() > .5);
		return random;
	}

	public static BitString fromInt(int i, int n) {
		return new BitString(PApplet.binary(i, n));
	}

	public void invert() {
		for(int i = 0; i < bytes.length; i++)
			bytes[i] = (byte) ~bytes[i];
	}

	public void randomize() {
		randomize(0, 256);
	}
	
	public void randomize(int min, int max) {
		for(int i = 0; i < bytes.length; i++) {
			byte randomByte = (byte) ((Math.random() * (max - min)) + min);
			bytes[i] = randomByte;
		}
	}

	public void flip(int cur) {
		set(cur, !get(cur));
	}
}
