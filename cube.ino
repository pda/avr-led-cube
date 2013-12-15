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

uint16_t loopCount = 0;
uint32_t tickCount = 0;

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

void setLed(uint8_t x, uint8_t y, uint8_t z, byte on) {

  // flatten x{0...4},y{0...4} to i{0...16}
  uint8_t i = y * CATHODE_COUNT + x;

  if (i < 2) {
    if (on) anodesPortB[z] |= (1 << i);
    else anodesPortB[z] &= ~(1 << i);
  } else if (i < 8) {
    if (on) anodesPortC[z] |= (1 << (i - 2));
    else anodesPortC[z] &= ~(1 << (i - 2));
  } else {
    if (on) anodesPortD[z] |= (1 << (i - 8));
    else anodesPortD[z] &= ~(1 << (i - 8));
  }
}

// Called once per perceivable frame, to update animation.
void tick() {
  uint8_t x, y, z, on;
  x = (tickCount % CATHODE_COUNT);
  y = (tickCount % ANODE_COUNT) / CATHODE_COUNT;
  z = (tickCount % LED_COUNT) / ANODE_COUNT;
  on = (tickCount % (LED_COUNT * 2) < LED_COUNT);
  setLed(x, y, z, on);
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

// Arduino initialization.
void setup() {
  setupPorts();
  clearCathodes();
}

// Arduino loop.
void loop() {
  if (loopCount++ == 5) {
    loopCount = 0;
    tick();
  }

  display();
}
