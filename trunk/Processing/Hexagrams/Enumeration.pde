BigInteger binaryCount(BigInteger x) {
  return x.add(BigInteger.ONE);
}

BigInteger chordCount(BigInteger x, int len) {
  int low = getLowestMovable(x, len);
  if(low + 1 < len) {
    x = x.clearBit(low);
    x = x.setBit(low + 1);
    int lower = 0;
    for(int j = 0; j < low; j++) {
      if(x.testBit(j)) {
        x = x.clearBit(j);
        x = x.setBit(lower++);
      }
    }
  } else {
    int count = x.bitCount() + 1;
    x = BigInteger.ZERO;
    for(int j = 0; j < count; j++)
      x = x.setBit(j);
  }
  return x;
}

int getLowestMovable(BigInteger x, int len) {
  for(int i = 0; i < len; i++)
    if(x.testBit(i) && !x.testBit(i + 1))
      return i;
  return len;
}

BigInteger chuHsiCount(BigInteger x, int len) {
  int high = getHighestMoveable(x, len);
  if(high >= 0) {
    x = x.clearBit(high++);
    x = x.setBit(high++);
    for(int j = high; j < len; j++)
      if(x.testBit(j)) {
        x = x.clearBit(j);
        x = x.setBit(high++);
      }
  } else {
    int count = x.bitCount() + 1;
    x = new BigInteger("0");
    for(int j = 0; j < count; j++)
      x = x.setBit(j);
  }
  return x;
}

int getHighestMoveable(BigInteger x, int len) {
  for(int i = len - 2; i >= 0; i--)
    if(x.testBit(i) && !x.testBit(i + 1))
      return i;
  return -1;
}

BigInteger reverse(BigInteger x, int len) {
  for(int i = 0; i < len / 2; i++) {
    int left = len - i - 1;
    boolean right = x.testBit(i);
    if(x.testBit(left))
      x = x.setBit(i);
    else
      x = x.clearBit(i);
    if(right)
      x = x.setBit(left);
    else
      x = x.clearBit(left);
  }
  return x;
}
