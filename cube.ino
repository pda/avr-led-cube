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

void setup() {

  // See README.md for port/pin/LED maps.
  DDRB |= MASK_PORTB; // anode 0..1, cathode 0..3
  DDRC |= MASK_PORTC; // anode 2..7
  DDRD |= MASK_PORTD; // anode 8..15

  // Deselect all cathodes.
  PORTB &= ~MASK_CATHODE;

  // Select all anodes.
  PORTB |= (MASK_PORTB & ~MASK_CATHODE);
  PORTC |= MASK_PORTC;
  PORTD |= MASK_PORTD;

}

void loop() {

  // cathode 0
  PORTB = (PORTB & ~MASK_CATHODE) | MASK_CATHODE_0;

  // cathode 1
  PORTB = (PORTB & ~MASK_CATHODE) | MASK_CATHODE_1;

  // cathode 2
  PORTB = (PORTB & ~MASK_CATHODE) | MASK_CATHODE_2;

  // cathode 3
  PORTB = (PORTB & ~MASK_CATHODE) | MASK_CATHODE_3;

}
