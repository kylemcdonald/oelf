int sc = 8;
boolean[] swapped;
int[] positions;

void setup() {
  size(512, 64);
  ellipseMode(CORNER);
  noFill();
  smooth();
}

void draw() {
  int n = mouseX / sc + 1;

  int lowest = 0;
  boolean[] swapped = new boolean[n];
  int[] positions = new int[n];
  while(lowest < n) {
    if(!swapped[lowest]) {
      swapped[lowest] = true;
      int start = lowest;
      int skip = 0;
      while(start < n) {
        skip++;
        while(start + skip < n && swapped[start + skip])
          skip++;
        int end = start + skip;
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
    lowest++;
  }
  
  background(255);
  stroke(200);
  for(int i = 0; i < n; i++)
    line(i * sc, 0, i * sc, height);
  stroke(0);
  for(int i = 0; i < n; i++) {
    if(positions[i] > i) {
      int diff = positions[i] - i;
      diff *= sc;
      ellipse(i * sc, height - diff / 2, diff, diff);
    }
  }
}

