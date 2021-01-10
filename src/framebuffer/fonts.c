#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "fonts.h"

// These include define the actual font bitmaps
// (they should really be .c files)

#include "fonts/6847.fnt.h"
#include "fonts/8x10.fnt.h"
#include "fonts/8x11snsf.fnt.h"
#include "fonts/8x14.fnt.h"
#include "fonts/8x8.fnt.h"
#include "fonts/8x8ital.fnt.h"
#include "fonts/9x16snsf.fnt.h"
#include "fonts/bigserif.fnt.h"
#include "fonts/blcksnsf.fnt.h"
#include "fonts/block.fnt.h"
#include "fonts/bold.fnt.h"
#include "fonts/broadway.fnt.h"
#include "fonts/computer.fnt.h"
#include "fonts/courier.fnt.h"
#include "fonts/future.fnt.h"
#include "fonts/greek.fnt.h"
#include "fonts/hollow.fnt.h"
#include "fonts/italics.fnt.h"
#include "fonts/lcd.fnt.h"
#include "fonts/medieval.fnt.h"
#include "fonts/norway.fnt.h"
#include "fonts/sanserif.fnt.h"
#include "fonts/script.fnt.h"
#include "fonts/slant.fnt.h"
#include "fonts/small.fnt.h"
#include "fonts/standard.fnt.h"
#include "fonts/stretch.fnt.h"
#include "fonts/sub.fnt.h"
#include "fonts/super.fnt.h"
#include "fonts/thin8x8.fnt.h"
#include "fonts/thin.fnt.h"
#include "fonts/thnserif.fnt.h"


static uint8_t fontbuffer[4096];

// Font Catalog

static font_t font_catalog[] = {
   {"6847",     font6847, 12, 128, 0, 8, 12},
   {"8X10",     font01,   16, 256, 0, 8, 16},
   {"8X11SNSF", font02,   16, 256, 0, 8, 16},
   {"8X14",     font03,   16, 256, 0, 8, 16},
   {"8X8",      font04,   16, 256, 0, 8, 16},
   {"8X8ITAL",  font05,   16, 256, 0, 8, 16},
   {"9X16",     font06,   16, 256, 0, 8, 16},
   {"BIGSERIF", font07,   16, 256, 0, 8, 16},
   {"BLCKSNSF", font08,   16, 256, 0, 8, 16},
   {"BLOCK",    font09,   16, 256, 0, 8, 16},
   {"BOLD",     font10,   16, 256, 0, 8, 16},
   {"BROADWAY", font11,   16, 256, 0, 8, 16},
   {"COMPUTER", font12,   16, 256, 0, 8, 16},
   {"COURIER",  font13,   16, 256, 0, 8, 16},
   {"FUTURE",   font14,   16, 256, 0, 8, 16},
   {"GREEK",    font15,   16, 256, 0, 8, 16},
   {"HOLLOW",   font16,   16, 256, 0, 8, 16},
   {"ITALICS",  font17,   16, 256, 0, 8, 16},
   {"LCD",      font18,   16, 256, 0, 8, 16},
   {"MEDIEVAL", font19,   16, 256, 0, 8, 16},
   {"NORWAY",   font20,   16, 256, 0, 8, 16},
   {"SANSERIF", font21,   16, 256, 0, 8, 16},
   {"SCRIPT",   font22,   16, 256, 0, 8, 16},
   {"SLANT",    font23,   16, 256, 0, 8, 16},
   {"SMALL",    font24,   16, 256, 0, 8, 16},
   {"STANDARD", font25,   16, 256, 0, 8, 16},
   {"STRETCH",  font26,   16, 256, 0, 8, 16},
   {"SUB",      font27,   16, 256, 0, 8, 16},
   {"SUPER",    font28,   16, 256, 0, 8, 16},
   {"THIN",     font29,   16, 256, 0, 8, 16},
   {"THIN8X8",  font30,   16, 256, 0, 8, 16},
   {"THNSERIF", font31,   16, 256, 0, 8, 16},
   {NULL},
};


// The coordinate system is suitable for use with VDU 5
//
// i.e. x_pos, y_pos are the top left hand corner of the character
// and 0,0 is the bottom left of the screen

static void default_draw_character(font_t *font, screen_mode_t *screen, int c, int x_pos, int y_pos, pixel_t fg_col, pixel_t bg_col) {
   int p = c * font->bytes_per_char;
   int x = x_pos;
   int y = y_pos;
   for (int i = 0; i < font->height; i++) {
      int data = fontbuffer[p++];
      for (int j = 0; j < font->width; j++) {
         int col = (data & 0x80) ? fg_col : bg_col;
         for (int sx = 0; sx < font->scale_w; sx++) {
            for (int sy = 0; sy < font->scale_h; sy++) {
               screen->set_pixel(screen, x + sx, y + sy, col);
            }
         }
         x += font->scale_w;
         data <<= 1;
      }
      x = x_pos;
      y -= font->scale_h;
   }
}

static int default_read_character(font_t *font, screen_mode_t *screen, int x_pos, int y_pos) {
   uint8_t screendata[MAX_FONT_HEIGHT];

   // TODO - this is hack to ignore the flashing cursor
   int cursor_lines = 2;

   // TODO: Account for font scaling

   // TODO: Normalize charater order (6847 font)

   // Read the character from screen memory
   int x = x_pos;
   int y = y_pos;
   for (int i = 0; i < font->height - cursor_lines; i++) {
      int row = 0;
      for (int j = 0; j < font->width; j++) {
         row <<= 1;
         if (screen->get_pixel(screen, x + j, y - i)) {
            row |= 1;
         }
      }
      screendata[i] = row;
   }
   // Match against font
   for (int c = 0x00; c < 0x60; c++) {
      int y;
      for (y = 0; y < font->height - cursor_lines; y++) {
         if (fontbuffer[c * font->bytes_per_char + y] != screendata[y]) {
            break;
         }
      }
      if (y == font->height - cursor_lines) {
         // We are still using the Atom 6847 character ROM!
         //
         // Demangle the
         // 00-1F map to 40-5F
         // 20-3F map to 20-3F
         // 40-5F map to 60-7F
         if (c < 0x20) {
            return c + 0x40;
         } else if (c < 0x40) {
            return c;
         } else {
            return c + 0x20;
         }
      }
   }
   return 0;

}


font_t *get_font(char *name) {
   font_t *font = font_catalog;
   while (font->name) {
      if (!strcasecmp(name, font->name)) {

         // Copy the font into a local font buffer, so VDU 23 can update it
         memcpy(fontbuffer, font->data, font->num_chars * font->bytes_per_char);

         font->scale_w = 1;
         font->scale_h = 1;
         font->spacing = 0;
         font->draw_character = default_draw_character;
         font->read_character = default_read_character;

         return font;
      }
      font++;
   }
   return NULL;
}
