#include <stdio.h>

int sumFromFile(const char *filename) {
  
  int sum = 0; // Initialize sum counter
  int num; // Tmp variable
  FILE *fp = fopen(filename, "r"); //Opens file and reads only 

  if (fp == NULL) {
    printf("\nFile \"%s\" not found or cannot be accessed.\n", filename);
    return 0;
  } //Guard statement in case file cannot be accessed

  while (fscanf(fp, "%d", &num) != EOF) { // For each line: stores int in 'num' until EOF
    sum += num;
  }
  fclose(fp);
  return sum;
}

int main() {
  const char *file_name = "numbers.txt";
  int result = sumFromFile(file_name);
  printf("Sum of numbers: %d\n", result);
  return 0;
}

