int sc = 4;
boolean[] swapped;
int[] positions;

void setup() {
  size(512, 256);
  ellipseMode(CORNER);
  noFill();
  smooth();
}

void draw() {
  int n = mouseX / sc + 1;

  int skipAmount = (mouseY / 8) + 1;
  boolean[] swapped = new boolean[n];
  int[] positions = new int[n];
  for(int lowest = 0; lowest < n; lowest++) {
    if(!swapped[lowest]) {
      int start = lowest;
      int skip = 1;
      while(start < n) {
        skip += skipAmount;
        int end = start + skip;
        while(end < n && swapped[end]) {
          skip += skipAmount;
          end += skipAmount;
        }
        if(end < n) {
          swapped[start] = true;
          positions[start] = end;
          swapped[end] = true;
          positions[end] = start;
        }
        start = end;
        while(start < n && swapped[start])
          start++;
      }
    }
  }

  background(255);
  for(int i = 0; i < n; i++) {
    stroke(swapped[i] ? color(200) : color(255, 0, 0));
    line(i * sc, 0, i * sc, height);
  }
  stroke(0);
  for(int i = 0; i < n; i++) {
    if(positions[i] > i) {
      int diff = positions[i] - i;
      diff *= sc;
      ellipse(i * sc, height - diff / 2, diff, diff);
    }
  }
}


