class Column implements Comparable {
  boolean[] column;
  float count, avg, transitions;
  float[] sortOrder = new float[2];
  Column(boolean[] column) {
    this.column = column;
    
    avg = 0;
    count = 0;
    transitions = 0;
    boolean cur = column[0];
    for(int i = 0; i < column.length; i++) {
      if(column[i]) {
        avg += i;
        count++;
      }
      if(column[i] != cur) {
        transitions++;
        cur = column[i];
      }
    }
    avg /= count;
    
    sortOrder[0] = transitions;
    sortOrder[1] = avg;
  }
  int compareTo(Object o) {
    Column co = (Column) o;
    
    if(sortOrder[0] < co.sortOrder[0])
      return -1;
    if(sortOrder[0] > co.sortOrder[0])
      return +1;
    
    if(sortOrder[1] < co.sortOrder[1])
      return -1;
    if(sortOrder[1] > co.sortOrder[1])
      return +1;
      
    return 0;
  }
}
