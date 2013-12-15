#include "stubs.h"
#include "../cube.ino"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void doLed(int layer, int x, int y) {
  if (buffer[layer][(y * 4) + x])
    putc('@', stdout);
  else
    putc('.', stdout);
}

void doLayer(int layer) {
  for (int y = 0; y <= 3; y++) {
    for (int i = 0; i < y * 4; i++) putc(' ', stdout);
    for (int x = 0; x <= 3; x++) {
      doLed(layer, x, y);
      printf("     ");
    }
    printf("\n");
  }
  printf("\n");
}

void render() {
  printf("\033[25A"); // up 25 lines

  for (int layer = 0; layer <= 3; layer++) {
    doLayer(layer);
  }
  printf("\n\n");
}

int main() {

  printf("\033[25A"); // up 25 lines
  printf("\033[2J"); // Clear the screen, move to (0,0)

  for (int i = 0; i <= 100; i++) {
    tick();
    render();
    usleep(50000);
  }
}
