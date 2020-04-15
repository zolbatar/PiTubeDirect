/* Mouse pointers */
#include <inttypes.h>


/* Black mask */
uint8_t mpblack[] = {
   0b11111111,
   0b10000010,
   0b10000100,
   0b10000100,
   0b10000010,
   0b10110001,
   0b11001010,
   0b10000100,

   0b11100000,
   0b10100000,
   0b11100000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,

   0b00111100,
   0b01010010,
   0b10010001,
   0b10010001,
   0b10011101,
   0b10000001,
   0b01000010,
   0b00111100,

   0b11111111,
   0b10000001,
   0b10111111,
   0b10100000,
   0b10100000,
   0b10100000,
   0b10100000,
   0b11100000,

   0b00010000,
   0b00101000,
   0b01000100,
   0b11000110,
   0b01000100,
   0b01000100,
   0b01000100,
   0b01111100,

   0b01111100,
   0b01000100,
   0b01000100,
   0b01000100,
   0b11000110,
   0b01000100,
   0b00101000,
   0b00010000,

   0b00001000,
   0b11111100,
   0b10000010,
   0b10000001,
   0b10000010,
   0b11111100,
   0b00001000,
   0b00000000,

   0b00010000,
   0b00111111,
   0b01000001,
   0b10000001,
   0b01000001,
   0b00111111,
   0b00010000,
   0b00000000,

   0b00111100,
   0b01000110,
   0b10001101,
   0b10001101,
   0b10011001,
   0b10011001,
   0b01110010,
   0b00111100,

   0b00111000,
   0b01000100,
   0b01010100,
   0b01110100,
   0b00101000,
   0b00111000,
   0b00101000,
   0b00111000,

   0b01111100,
   0b01000100,
   0b01101100,
   0b00101000,
   0b00101000,
   0b01101100,
   0b01000100,
   0b01111100,

   0b00111000,
   0b00101000,
   0b11101110,
   0b10000010,
   0b11101110,
   0b00101000,
   0b00111000,
   0b00000000,

   0b01000000,
   0b10100000,
   0b10100000,
   0b10111110,
   0b10101011,
   0b10000001,
   0b11111111,
   0b01111110,

   0b01111110,
   0b11111111,
   0b10000001,
   0b10101011,
   0b10111110,
   0b10100000,
   0b10100000,
   0b01000000,

   0b01111110,
   0b11000001,
   0b11011110,
   0b11001000,
   0b11011000,
   0b11001000,
   0b11011000,
   0b01110000,

   0b01111110,
   0b10000011,
   0b01111011,
   0b00010011,
   0b00011011,
   0b00010011,
   0b00011011,
   0b00001110,

   0b11111111,
   0b01000010,
   0b00100100,
   0b00011000,
   0b00011000,
   0b00100100,
   0b01000010,
   0b11111111,

   0b11111111,
   0b01000010,
   0b00100100,
   0b00011000,
   0b00011000,
   0b00100100,
   0b01011010,
   0b11111111,

   0b11111111,
   0b01000010,
   0b00100100,
   0b00011000,
   0b00011000,
   0b00100100,
   0b01111110,
   0b11111111,

   0b11111111,
   0b01000010,
   0b00100100,
   0b00011000,
   0b00011000,
   0b00111100,
   0b01111110,
   0b11111111,

   0b11100000,
   0b10011000,
   0b01000110,
   0b00110001,
   0b01101011,
   0b01010101,
   0b00101001,
   0b00011110,

   0b00000111,
   0b00011001,
   0b01100010,
   0b10001100,
   0b11010110,
   0b10101010,
   0b10010100,
   0b01111000,

   0b00011110,
   0b00101001,
   0b01010101,
   0b01101011,
   0b00110001,
   0b01000110,
   0b10011000,
   0b11100000,

   0b01111000,
   0b10010100,
   0b10101010,
   0b11010110,
   0b10001100,
   0b01100010,
   0b00011001,
   0b00000111,

   0b01100110,
   0b10011001,
   0b10011001,
   0b11011101,
   0b11011101,
   0b10011001,
   0b10011001,
   0b01100110,

   0b01100110,
   0b11111111,
   0b11111111,
   0b10011001,
   0b10011001,
   0b10011001,
   0b10011001,
   0b01100110,

   0b01100110,
   0b10011001,
   0b10011001,
   0b10111011,
   0b10111011,
   0b10011001,
   0b10011001,
   0b01100110,

   0b01100110,
   0b10011001,
   0b10011001,
   0b10011001,
   0b10011001,
   0b11111111,
   0b11111111,
   0b01100110,

   0b11111111,
   0b10000010,
   0b10000100,
   0b10001000,
   0b10010000,
   0b10100000,
   0b11000000,
   0b10000000,

   0b10000000,
   0b11000000,
   0b10100000,
   0b10010000,
   0b10001000,
   0b10000100,
   0b10000010,
   0b11111111,

   0b00000001,
   0b00000011,
   0b00000101,
   0b00001001,
   0b00010001,
   0b00100001,
   0b01000001,
   0b11111111,

   0b11111111,
   0b01000001,
   0b00100001,
   0b00010001,
   0b00001001,
   0b00000101,
   0b00000011,
   0b00000001
};

/* White mask */
uint8_t mpwhite[] = {
   0b00000000,
   0b01111100,
   0b01111000,
   0b01111000,
   0b01111100,
   0b01001110,
   0b00000100,
   0b00000000,

   0b00000000,
   0b01000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,

   0b00000000,
   0b00101100,
   0b01101110,
   0b01101110,
   0b01100010,
   0b01111110,
   0b00111100,
   0b00000000,

   0b00000000,
   0b01111110,
   0b01000000,
   0b01000000,
   0b01000000,
   0b01000000,
   0b01000000,
   0b00000000,

   0b00000000,
   0b00010000,
   0b00111000,
   0b00111000,
   0b00111000,
   0b00111000,
   0b00111000,
   0b00000000,

   0b00000000,
   0b00111000,
   0b00111000,
   0b00111000,
   0b00111000,
   0b00111000,
   0b00010000,
   0b00000000,

   0b00000000,
   0b00000000,
   0b01111100,
   0b01111110,
   0b01111100,
   0b00000000,
   0b00000000,
   0b00000000,

   0b00000000,
   0b00000000,
   0b00111110,
   0b01111110,
   0b00111110,
   0b00000000,
   0b00000000,
   0b00000000,

   0b00000000,
   0b00111000,
   0b01110010,
   0b01110010,
   0b01100110,
   0b01100110,
   0b00001100,
   0b00000000,

   0b00000000,
   0b00111000,
   0b00101000,
   0b00001000,
   0b00010000,
   0b00000000,
   0b00010000,
   0b00000000,

   0b00000000,
   0b00111000,
   0b00010000,
   0b00010000,
   0b00010000,
   0b00010000,
   0b00111000,
   0b00000000,

   0b00000000,
   0b00010000,
   0b00010000,
   0b01111100,
   0b00010000,
   0b00010000,
   0b00000000,
   0b00000000,

   0b00000000,
   0b01000000,
   0b01000000,
   0b01000000,
   0b01010100,
   0b01111110,
   0b00000000,
   0b00000000,

   0b00000000,
   0b00000000,
   0b01111110,
   0b01010100,
   0b01000000,
   0b01000000,
   0b01000000,
   0b00000000,

   0b00000000,
   0b00111110,
   0b00100000,
   0b00110000,
   0b00100000,
   0b00110000,
   0b00100000,
   0b00000000,

   0b00000000,
   0b01111100,
   0b00000100,
   0b00001100,
   0b00000100,
   0b00001100,
   0b00000100,
   0b00000000,

   0b00000000,
   0b00111100,
   0b00011000,
   0b00000000,
   0b00000000,
   0b00011000,
   0b00111100,
   0b00000000,

   0b00000000,
   0b00111100,
   0b00011000,
   0b00000000,
   0b00000000,
   0b00011000,
   0b00100100,
   0b00000000,

   0b00000000,
   0b00111100,
   0b00011000,
   0b00000000,
   0b00000000,
   0b00011000,
   0b00000000,
   0b00000000,

   0b00000000,
   0b00111100,
   0b00011000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,

   0b00000000,
   0b01100000,
   0b00111000,
   0b00001110,
   0b00010100,
   0b00101010,
   0b00010110,
   0b00000000,

   0b00000000,
   0b00000110,
   0b00011100,
   0b01110000,
   0b00101000,
   0b01010100,
   0b01101000,
   0b00000000,

   0b00000000,
   0b00010110,
   0b00101010,
   0b00010100,
   0b00001110,
   0b00111000,
   0b01100000,
   0b00000000,

   0b00000000,
   0b01101000,
   0b01010100,
   0b00101000,
   0b01110000,
   0b00011100,
   0b00000110,
   0b00000000,

   0b00000000,
   0b01100110,
   0b01100110,
   0b00100010,
   0b00100010,
   0b01100110,
   0b01100110,
   0b00000000,

   0b00000000,
   0b00000000,
   0b00000000,
   0b01100110,
   0b01100110,
   0b01100110,
   0b01100110,
   0b00000000,

   0b00000000,
   0b01100110,
   0b01100110,
   0b01000100,
   0b01000100,
   0b01100110,
   0b01100110,
   0b00000000,

   0b00000000,
   0b01100110,
   0b01100110,
   0b01100110,
   0b01100110,
   0b00000000,
   0b00000000,
   0b00000000,

   0b00000000,
   0b01111100,
   0b01111000,
   0b01110000,
   0b01100000,
   0b01000000,
   0b00000000,
   0b00000000,

   0b00000000,
   0b00000000,
   0b01000000,
   0b01100000,
   0b01110000,
   0b01111000,
   0b01111100,
   0b00000000,

   0b00000000,
   0b00000000,
   0b00000010,
   0b00000110,
   0b00001110,
   0b00011110,
   0b00111110,
   0b00000000,

   0b00000000,
   0b00111110,
   0b00011110,
   0b00001110,
   0b00000110,
   0b00000010,
   0b00000000,
   0b00000000
};

#ifdef BPP32
uint32_t mpsave[64];
#else
uint16_t mpsave[64];
#endif

uint16_t mp_save_x = 65535;
uint16_t mp_save_y = 65535;

void mp_save(void) {
   int x, y;
   for (x=0; x<8; x++) {
      for (y=0; y<8; y++) {
         mpsave[x+y*8] = fb_getpixel(mp_save_x+x*2, mp_save_y-y*2);
      }
   }
}

void mp_restore(void) {
   int x, y;
   for (x=0; x<8; x++) {
      for (y=0; y<8; y++) {
         fb_setpixel(mp_save_x+x*2, mp_save_y-y*2, mpsave[x+y*8]);
      }
   }
}