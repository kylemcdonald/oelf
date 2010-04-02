import processing.pdf.*;

import java.math.*;

int side = 8;
int bits = 6;

int top = 1 << bits;

boolean useChord = false;
boolean useBinary = true;
boolean useChuHsi = false;

boolean useGray = true;

boolean usePdf = true;

color black = color(0);
color white = color(255);
color gray = color(128);

void setup() {
  int w = top * side;
  int h = bits * side;
  if(usePdf)
    size(w, h, PDF, "bitmap.pdf");
  else
    size(w, h, P2D);
  stroke(gray);
  noLoop();
}

void draw() {
  BigInteger counter = BigInteger.ZERO;
  for(int i = 0; i < top; i++) {
    if(useGray) {
      BigInteger shifted = counter.shiftRight(1);
      BigInteger gray = shifted.xor(counter);
      drawBitmap(gray);
    } else if(useChuHsi)
      drawBitmap(reverse(counter, bits));
    else
      drawBitmap(counter);
    translate(side, 0);
    if(useChord)
      counter = chordCount(counter, bits);
    else if(useBinary)
      counter = binaryCount(counter);
    else if(useChuHsi)
      counter = chuHsiCount(counter, bits);
  }
  
  if(usePdf)
    exit();
}

void drawBitmap(BigInteger x) {
  pushMatrix();
  for(int i = 0; i < bits; i++) {
    fill(x.testBit(i) ? white : black);
    rect(0, 0, side, side);
    translate(0, side);
  }
  popMatrix();
}
