import java.math.*;

int len = 16;
int rows = 1024;
int cols = 1024;

void setup() {
  size(cols, rows);
  
  background(0);
  int col = 0;
  int row = 0;
  
  color white = color(255);
  
  loadPixels();
  BigInteger x = new BigInteger("0");
  for(int i = 0; i < 1 << len; i++) {
    for(int j = 0; j < len; j++)
      if(x.testBit(j))
        pixels[cols * ((len * row) + j) + col] = white;
      
    col++;
    if(col == cols) {
      col = 0;
      row++;
    }
    
    int high = getHighestMoveable(x, len);
    if(high >= 0) {
      x = x.clearBit(high++);
      x = x.setBit(high++);
      for(int j = high; j < len; j++)
        if(x.testBit(j)) {
          x = x.clearBit(j);
          x = x.setBit(high++);
        }
    } else {
      int count = x.bitCount() + 1;
      x = new BigInteger("0");
      for(int j = 0; j < count; j++)
        x = x.setBit(j);
    }
  }
  updatePixels();
  noLoop();
}

void draw() {
  saveFrame("basicEnumeration.png");
}

int getHighestMoveable(BigInteger x, int len) {
  for(int i = len - 2; i >= 0; i--)
    if(x.testBit(i) && !x.testBit(i + 1))
      return i;
  return -1;
}
