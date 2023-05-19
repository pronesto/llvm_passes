int main(int argc, char** argv) {
  int sum = 0;
  for (int i = 0; i < argc; i++) {
    for (int j = 0; j < 2; j++) {
      sum += argv[i][j];
    }
  }
  for (int i = 0; i < argc; i += 2) {
    for (int j = 0; j < 2; j *= 3) {
      sum += argv[i][j];
    }
  }
  for (int i = 0; i < 10; i += 5) {
    for (int j0 = 0; j0 < 2; j0 += 7) {
      sum--;
    }
    for (int j1 = 20; j1 > 3; j1 -= 11) {
      sum++;
    }
  }
  return sum;
}
