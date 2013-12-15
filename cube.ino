#define MASK_CATHODE 0b00111100

#define MASK_CATHODE_0 0b00000100
#define MASK_CATHODE_1 0b00001000
#define MASK_CATHODE_2 0b00010000
#define MASK_CATHODE_3 0b00100000

#define MASK_ANODE_PORTB 0b00000011
#define MASK_ANODE_PORTC 0b00111111
#define MASK_ANODE_PORTD 0b11111111

#define MASK_PORTB 0b00111111
#define MASK_PORTC 0b00111111
#define MASK_PORTD 0b11111111

uint16_t loopCount = 0;
uint32_t tickCount = 0;

byte buffer[4][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

void clearCathodes() {
  PORTB &= ~MASK_CATHODE;
}

void setAnodes(int i) {
  PORTB = (PORTB & ~MASK_ANODE_PORTB) |
    (buffer[i][0] << 0) |
    (buffer[i][1] << 1);

  PORTC = (PORTC & ~MASK_ANODE_PORTC) |
    (buffer[i][2] << 0) |
    (buffer[i][3] << 1) |
    (buffer[i][4] << 2) |
    (buffer[i][5] << 3) |
    (buffer[i][6] << 4) |
    (buffer[i][7] << 5);

  PORTD = (PORTD & ~MASK_ANODE_PORTD) |
    (buffer[i][8] << 0) |
    (buffer[i][9] << 1) |
    (buffer[i][10] << 2) |
    (buffer[i][11] << 3) |
    (buffer[i][12] << 4) |
    (buffer[i][13] << 5) |
    (buffer[i][14] << 6) |
    (buffer[i][15] << 7);
}

// Called once per perceivable frame, to update animation.
void tick() {
  byte *b = ((byte *)buffer) + (tickCount % 64);
  if (*b) *b = 0;
  else *b = 1;
  tickCount++;
}

// Called as frequently as possible to multiplex the display.
void display() {
  // cathode 0
  clearCathodes();
  setAnodes(0);
  PORTB = (PORTB & ~MASK_CATHODE) | MASK_CATHODE_0;
  delayMicroseconds(10);

  // cathode 1
  clearCathodes();
  setAnodes(1);
  PORTB = (PORTB & ~MASK_CATHODE) | MASK_CATHODE_1;
  delayMicroseconds(10);

  // cathode 2
  clearCathodes();
  setAnodes(2);
  PORTB = (PORTB & ~MASK_CATHODE) | MASK_CATHODE_2;
  delayMicroseconds(10);

  // cathode 3
  clearCathodes();
  setAnodes(3);
  PORTB = (PORTB & ~MASK_CATHODE) | MASK_CATHODE_3;
  delayMicroseconds(10);
  clearCathodes();
}

void setup() {

  // See README.md for port/pin/LED maps.
  DDRB |= MASK_PORTB; // anode 0..1, cathode 0..3
  DDRC |= MASK_PORTC; // anode 2..7
  DDRD |= MASK_PORTD; // anode 8..15

  clearCathodes();
}

void loop() {
  if (loopCount++ == 1000) {
    loopCount = 0;
    tick();
  }

  display();
}
