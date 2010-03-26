PImage img;

void setup() {
  img = loadImage("50.png");
  size(img.width, img.height);
  img.loadPixels();
  
  boolean[][] vals = new boolean[width][height];
  Vector columns = new Vector();
  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      int cur = j * width + i;
      vals[i][j] = img.pixels[cur] == 0xffffffff;
    }
    columns.add(new Column(vals[i]));
  }
  
  Collections.sort(columns);
  
  for(int i = 0; i < width; i++) {
    Column co = (Column) columns.get(i);
    for(int j = 0; j < height; j++) {
      int cur = j * width + i;
      img.pixels[cur] = color(co.column[j] ? 255 : 0);
    }
  }
  
  img.updatePixels();
}

void draw() {
  background(0);
  image(img, 0, 0);
}
