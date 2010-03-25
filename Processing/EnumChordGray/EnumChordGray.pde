import java.math.*;

// The lowest voice that can rise a half-tone does so
// and any lower voices descend to their points of departure.

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
    BigInteger g = x.shiftRight(1).xor(x);
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
          x = x.setBit(lower);
          lower++;
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
  noLoop();
}

void draw() {
  saveFrame("grayChordCatalogue.png");
}

int getLowestMovable(BigInteger x, int len) {
  for(int i = 0; i < len; i++)
    if(x.testBit(i) && !x.testBit(i + 1))
      return i;
  return len;
}
