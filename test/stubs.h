#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

typedef uint8_t byte;

void delayMicroseconds(int us) {
  usleep(us);
}

byte PORTB;
byte PORTC;
byte PORTD;

byte DDRB;
byte DDRC;
byte DDRD;
