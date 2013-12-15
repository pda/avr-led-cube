#define CATHODE_COUNT 4
#define ANODE_COUNT 16
#define LED_COUNT (CATHODE_COUNT * ANODE_COUNT)

#define PORTB_CATHODE_OFFSET 2

// The bits of AVR ports B, C, D being used for output.
#define MASK_PORTB 0b00111111
#define MASK_PORTC 0b00111111
#define MASK_PORTD 0b11111111

// Port B is dual purpose:
#define MASK_PORTB_CATHODE 0b00111100
#define MASK_PORTB_ANODE 0b00000011

typedef struct vector3 {
  uint8_t x, y, z;
} vector3_t;

typedef struct drop {
  vector3_t pos;
  uint8_t speed;
  uint8_t z; // extended z; divide by 8 for pos->z
} drop_t;

#define SNAKE_LENGTH 4
typedef struct snake {
  vector3_t segments[SNAKE_LENGTH];
  uint8_t head;
} snake_t;

uint8_t loopCount = 0;
uint16_t tickCount = 0;

drop_t drops[6];
snake_t snake;

// buffers for per-port anode data.
byte anodesPortB[CATHODE_COUNT];
byte anodesPortC[CATHODE_COUNT];
byte anodesPortD[CATHODE_COUNT];

// Disable all cathodes (layers).
void clearCathodes() {
  PORTB &= ~MASK_PORTB_CATHODE;
}

// Enable a cathode (layer). Does not clear other cathodes.
void enableCathode(uint8_t layer) {
  PORTB |= (1 << PORTB_CATHODE_OFFSET) << layer;
}

// Set the anodes for the specified layer, based on the
// anodesPort{B,C,D} buffers.
void setAnodes(int layer) {
  PORTB = (PORTB & ~MASK_PORTB_ANODE) | anodesPortB[layer];
  PORTC = anodesPortC[layer];
  PORTD = anodesPortD[layer];
}

void ledAddress(uint8_t x, uint8_t y, uint8_t z,
  byte ** portBuffer, uint8_t * bitIndex) {

  // flatten x{0...4},y{0...4} to i{0...16}
  uint8_t i = y * CATHODE_COUNT + x;

  if (i >= 8) {
    *portBuffer = &anodesPortD[z];
    *bitIndex = i - 8;
  } else if (i >= 2) {
    *portBuffer = &anodesPortC[z];
    *bitIndex = i - 2;
  } else {
    *portBuffer = &anodesPortB[z];
    *bitIndex = i;
  }
}

void setLed(uint8_t x, uint8_t y, uint8_t z, byte on) {

  byte * portBuffer;
  uint8_t bitIndex;
  ledAddress(x, y, z, &portBuffer, &bitIndex);

  if (on) *portBuffer |= (1 << bitIndex);
  else *portBuffer &= ~(1 << bitIndex);

}

void clearLeds() {
  for (int z = 0; z < CATHODE_COUNT; z++) {
    anodesPortB[z] = 0x00;
    anodesPortC[z] = 0x00;
    anodesPortD[z] = 0x00;
  }
}

void setupDrops() {
  for (int i = 0; i < sizeof(drops) / sizeof(vector3_t); i++) {
    randomDrop(&drops[i]);
  }
}

void randomDrop(struct drop * d) {
  vector3_t * pos = &d->pos;
  d->speed = rand() % 2 + 1;
  d->z = 0;
  pos->x = rand() % 4;
  pos->y = rand() % 4;
  pos->z = 0;
}

void tickDrops() {
  for (int i = 0; i < sizeof(drops) / sizeof(drop_t); i++) {
    drop_t * d = &drops[i];
    vector3_t * pos = &d->pos;

    d->z += d->speed;

    if (d->z < 32) {
      pos->z = d->z / 8;
    } else {
      pos->z = 3;
    }

    if (d->z >= 36) {
      randomDrop(d);
    }

    setLed(pos->x, pos->y, pos->z, 1);
  }
}

void setupSnake() {
  snake.head = 0;
  for (int i = 0; i < SNAKE_LENGTH; i++) {
    snake.segments[i].x = 0;
    snake.segments[i].y = 0;
    snake.segments[i].z = 0;
  }
}

void drawSnake() {
  for (int i = 0; i < SNAKE_LENGTH; i++) {
    vector3_t * pos = &snake.segments[i];
    setLed(pos->x, pos->y, pos->z, 1);
  }
}

void tickSnake() {
  if (tickCount % 4 == 0) {
    uint8_t nextHead = (snake.head + 1) % SNAKE_LENGTH;
    vector3_t * pHead = &snake.segments[snake.head];
    vector3_t * pNext = &snake.segments[nextHead];

    uint8_t x, y, z;
    x = pHead->x;
    y = pHead->y;
    z = pHead->z;

    pNext->x = x;
    pNext->y = y;
    pNext->z = z;

    uint8_t chance = rand() % 6;
    switch (chance) {
      case 0: if (x > 0) pNext->x--; break;
      case 1: if (x < 3) pNext->x++; break;
      case 2: if (y > 0) pNext->y--; break;
      case 3: if (y < 3) pNext->y++; break;
      case 4: if (z > 0) pNext->z--; break;
      case 5: if (z < 3) pNext->z++; break;
    }

    snake.head = nextHead;
  }

  drawSnake();
}

// Called once per perceivable frame, to update animation.
void tick() {

  clearLeds();

  //tickDrops();
  tickSnake();

  tickCount++;
}

// Called as frequently as possible to multiplex the display.
void display() {
  for (uint8_t i = 0; i <= 3; i++) {
    setAnodes(i);
    enableCathode(i);
    delayMicroseconds(2000);
    clearCathodes();
  }
}

void setupPorts() {
  // See README.md for port/pin/LED maps.
  DDRB |= MASK_PORTB; // anode 0..1, cathode 0..3
  DDRC |= MASK_PORTC; // anode 2..7
  DDRD |= MASK_PORTD; // anode 8..15
}

void setupAnimation() {
  //setupDrops();
  setupSnake();
}

// Arduino initialization.
void setup() {
  setupPorts();
  clearCathodes();
  setupAnimation();
}

// Arduino loop.
void loop() {
  if (loopCount++ == 4) {
    loopCount = 0;
    tick();
  }

  display();
}
