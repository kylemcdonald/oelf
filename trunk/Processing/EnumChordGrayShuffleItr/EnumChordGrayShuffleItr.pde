import java.math.*;

int len = 512;
int rows = len;
int cols = 512;
int order = 1;

int top = cols;

void setup() {
  size(cols, rows, P2D);
}

void draw() {
  background(0);
  int col = 0;
  int row = 0;
  
  color white = color(255);
  
  order+=2;
  
  loadPixels();
  BigInteger x = new BigInteger("0");
  //x = new BigInteger(len, new Random());
  for(int i = 0; i < top; i++) {
    BigInteger g = x;
    for(int j = 0; j < order; j++) {
      BigInteger last = g;
      g = last.shiftRight(1).xor(last);
      g = shuffle(g, len);
    }
    for(int j = 0; j < len; j++)
      if(g.testBit(j))
        pixels[cols * ((len * row) + j) + col] = white;
      
    col++;
    if(col == cols) {
      col = 0;
      row++;
    }
    
    int low = getLowestMovable(x, len);
    if(low + 1 < len) {
      x = x.clearBit(low);
      x = x.setBit(low + 1);
      int lower = 0;
      for(int j = 0; j < low; j++) {
        if(x.testBit(j)) {
          x = x.clearBit(j);
          x = x.setBit(lower++);
        }
      }
    } else {
      int count = x.bitCount() + 1;
      x = new BigInteger("0");
      for(int j = 0; j < count; j++)
        x = x.setBit(j);
    }
  }
  updatePixels();
  
  saveFrame(order + "-order Shuffled Gray Code.png");
}

BigInteger flip(BigInteger x, int len) {
  for(int i = 0; i < len / 2; i++) {
    int left = len - i - 1;
    boolean right = x.testBit(i);
    if(x.testBit(left))
      x = x.setBit(i);
    else
      x = x.clearBit(i);
    if(right)
      x = x.setBit(left);
    else
      x = x.clearBit(left);
  }
  return x;
}

int getLowestMovable(BigInteger x, int len) {
  for(int i = 0; i < len; i++)
    if(x.testBit(i) && !x.testBit(i + 1))
      return i;
  return len;
}

BigInteger shuffle(BigInteger x, int len) {
  BigInteger y = BigInteger.ZERO;
  for(int i = 0; i < len; i++) {
    if(x.testBit(i)) {
      BigInteger pos = new BigInteger(binary(i), 2);
      pos = flip(pos, 9);
      y = y.setBit(pos.intValue());
    }
  }
  return y;
}

