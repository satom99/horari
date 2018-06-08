#include <stdio.h>

int main() {
  char in;
  int com = 0;
  while((in = getchar()) != EOF) {
    // Control
    switch(in) {
      case '#':
        com = 1;
        break;
      case '\n':
        com = 0;
        break;
    }

    // Output
    if (!com)
      putchar(in);
  }

  return 0;
}
