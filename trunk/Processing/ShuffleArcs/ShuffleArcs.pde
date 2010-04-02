import processing.pdf.*;

boolean recording = false;

int sc = 4;
boolean[] swapped;
int[] positions;

boolean useProper = true;

void setup() {
  size(512, 180);
  smooth();
}

void draw() {  
  //int n = mouseX / sc + 1;
  int n = width / sc + 1;

  int skipAmount = (mouseY / 8) + 1;
  swapped = new boolean[n];
  positions = new int[n];
    
  if(mousePressed) {
    for(int lowest = 0; lowest < mouseX / sc; lowest++) {
      if(!swapped[lowest]) {
        int start = lowest;
        int end = lowest;
        int skip = 1;
        while(end < n) {
          skip += skipAmount;
          end += skip;
          while(end < n && swapped[end]) {
            end++;
            skip++;
          }
          if(end < n) {
            swapped[start] = true;
            swapped[end] = true;
            positions[start] = end;
            positions[end] = start;
            start = end + 1;
            while(start < n && swapped[start])
              start++;
          }
        }
      }
    }
  } else {
    for(int lowest = 0; lowest < mouseX / sc; lowest++) {
      if(!swapped[lowest]) {
        int start = lowest;
        int skip = 1;
        while(start < n) {
          skip += skipAmount;
          int end = start + skip;
          while(end < n && swapped[end]) {
            skip += skip;
            end += skip;
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
  }
  
  if(recording)
    beginRecord(PDF, "out.pdf");
    
  ellipseMode(CORNER);
  noFill();
  background(255);
  for(int i = 0; i < n; i++) {
    if(!swapped[i])
      stroke(color(255));
    else
      stroke(color(200));
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
  
  if(recording) {
    endRecord();
    recording = false;
  }
}

void keyPressed() {
  if(key == ' ')
    recording = true;
}

