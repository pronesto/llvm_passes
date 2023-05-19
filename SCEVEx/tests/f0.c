int main(int argc, char** argv) {
  int sum = 0;
  for (int i = 0; i < argc; i++) {
    for (int j = 0; j < 2; j++) {
      sum += argv[i][j];
    }
  }
  return sum;
}
