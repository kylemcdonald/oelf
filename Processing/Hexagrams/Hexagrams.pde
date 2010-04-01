import processing.pdf.*;

import java.math.*;

int hw = 9;
int broken = 3;
int padding = 4;
int bits = 6;

int top = 1 << bits;
int side = (int) sqrt(top);

void setup() {
  size((hw + padding) * side - padding,
    (bits * 2 + padding) * side - padding, PDF, "binary-hexagrams.pdf");
  noLoop();
}

void draw() {
  background(255);
  
  BigInteger counter = BigInteger.ZERO;
  for(int i = 0; i < side; i++) {
    pushMatrix();
    for(int j = 0; j < side; j++) {
      drawHexagram(counter);
      translate(hw + padding, 0);
      counter = counter.add(BigInteger.ONE);
    }
    popMatrix();
    translate(0, bits * 2 + padding);
  }
  
  exit();
}

void drawHexagram(BigInteger x) {
  pushMatrix();
  for(int i = 0; i < bits; i++) {
    if(x.testBit(i)) { // unbroken, yang, 1
      line(0, 0, hw, 0);
    } else { // broken, yin, 0
      float middle = (float) hw / 2;
      float bw = (float) broken / 2;
      line(0, 0, middle - bw, 0);
      line(middle + bw, 0, hw, 0);
    }
    translate(0, 2);
  }
  popMatrix();
}
