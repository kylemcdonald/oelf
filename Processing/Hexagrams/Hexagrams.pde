import processing.pdf.*;

import java.math.*;

int hw = 9;
int broken = 3;
int padding = 4;
int bits = 6;

int top = 1 << bits;
int side = (int) sqrt(top);

boolean useChord = false;
boolean useBinary = false;
boolean useChuHsi = true;

boolean useGray = false;

boolean usePdf = true;
int sf = 4;

void setup() {
  int w = (hw + padding) * side - padding;
  int h = (bits * 2 + padding) * side - padding;
  if(usePdf)
    size(w, h, PDF, "hexagrams.pdf");
  else
    size(w * sf, h * sf, P2D);
  noLoop();
}

void draw() {
  background(255);
  
  if(!usePdf) {
    scale(sf, sf);
    strokeWeight(sf);
    translate(0, 1);
  }
  
  BigInteger counter = BigInteger.ZERO;
  for(int i = 0; i < side; i++) {
    pushMatrix();
    for(int j = 0; j < side; j++) {
      if(useGray) {
        BigInteger shifted = counter.shiftRight(1);
        BigInteger gray = shifted.xor(counter);
        drawHexagram(gray);
      } else if(useChuHsi)
        drawHexagram(reverse(counter, bits));
      else
        drawHexagram(counter);
      translate(hw + padding, 0);
      if(useChord)
        counter = chordCount(counter, bits);
      else if(useBinary)
        counter = binaryCount(counter);
      else if(useChuHsi)
        counter = chuHsiCount(counter, bits);
    }
    popMatrix();
    translate(0, bits * 2 + padding);
  }
  
  if(usePdf)
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
