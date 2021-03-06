import java.math.*;

int len = 16;
int rows = 1024;
int cols = 1024;
int order = 1;

void setup() {
  size(cols, rows);
  
  background(0);
  int col = 0;
  int row = 0;
  
  color white = color(255);
  
  loadPixels();
  BigInteger counter = new BigInteger("0");
  for(int i = 0; i < 1 << len; i++) {
    BigInteger x = counter;
    for(int j = 0; j < order; j++) {
      BigInteger last = x;
      x = last.shiftRight(1).xor(last);
    }
    for(int j = 0; j < len; j++)
      if(x.testBit(j))
        pixels[cols * ((len * row) + j) + col] = white;
      
    col++;
    if(col == cols) {
      col = 0;
      row++;
    }
    
    counter = counter.add(BigInteger.ONE);
  }
  updatePixels();
  noLoop();
}

void draw() {
  saveFrame(order + "-order Gray Code.png");
}

