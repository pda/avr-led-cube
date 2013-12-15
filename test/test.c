#include "stubs.h"
#include "../cube.ino"

void doLed(int x, int y, int z) {

  // flatten x{0...4},y{0...4} to i{0...16}
  uint8_t i = y * CATHODE_COUNT + x;
  uint8_t on = 0;

  if (i < 2) {
    on = anodesPortB[z] >> i;
  } else if (i < 8) {
    on = anodesPortC[z] >> (i - 2);
  } else {
    on = anodesPortD[z] >> (i - 8);
  }

  putchar(on ? '@' : '.');
}

void doLayer(int z) {
  for (int y = 0; y <= 3; y++) {
    for (int i = 0; i < y * 4; i++) putchar(' ');
    for (int x = 0; x <= 3; x++) {
      doLed(x, y, z);
      printf("     ");
    }
    printf("\n");
  }
  printf("\n");
}

void render() {
  printf("\033[25A"); // up 25 lines
  for (int z = 0; z <= 3; z++) {
    doLayer(z);
  }
  printf("\n\n");
}

int main() {

  printf("\033[25A"); // up 25 lines
  printf("\033[2J"); // Clear the screen, move to (0,0)

  for (int i = 0; i <= 1000; i++) {
    tick();
    render();
    usleep(40000);
  }
}
