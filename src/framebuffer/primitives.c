#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "primitives.h"
#include "framebuffer.h"
#include "fonts.h"

#ifdef USE_V3D
#include "v3d.h"
#endif

static plotmode_t g_fg_plotmode;
static plotmode_t g_bg_plotmode;

static int16_t g_x_min;
static int16_t g_y_min;
static int16_t g_x_max;
static int16_t g_y_max;

#define FLOOD_QUEUE_SIZE 16384

static int16_t flood_queue_x[FLOOD_QUEUE_SIZE];
static int16_t flood_queue_y[FLOOD_QUEUE_SIZE];
static int flood_queue_wr;
static int flood_queue_rd;

// Rodders: Quadrant definitions for arc rendering
typedef enum {
   Q_NONE,
   Q_START,
   Q_END,
   Q_BOTH,
   Q_ALL
} quadrant_t;

static int16_t arc_end_x;
static int16_t arc_end_y;
static int16_t arc_fill_x;
static int16_t arc_fill_y;

#define NUM_SPRITES 256

typedef struct {
   int width;
   int height;
   void *data;
   size_t data_size;
} sprite_t;

static sprite_t sprites[NUM_SPRITES];

// ==========================================================================
// Static methods (operate at screen resolution)
// ==========================================================================

static inline int max(int a, int b) {
   return (a > b) ? a : b;
}

static inline int min(int a, int b) {
   return (a < b) ? a : b;
}

static int calc_radius(int x1, int y1, int x2, int y2) {
   return (int)(sqrtf((float)((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))) + 0.5F);
}

static pixel_t get_pixel(screen_mode_t *screen, int x, int y) {
   if (x < g_x_min  || x > g_x_max || y < g_y_min || y > g_y_max) {
      // Return the graphics background colour if off the screen
      return fb_get_g_bg_col();
   } else {
      return screen->get_pixel(screen, x, y);
   }
}

static void set_pixel(screen_mode_t *screen, int x, int y, pixel_t colour) {
   if (x < g_x_min  || x > g_x_max || y < g_y_min || y > g_y_max) {
      return;
   }
   if (g_fg_plotmode != PM_NORMAL) {
      pixel_t existing = screen->get_pixel(screen, x, y);
      switch (g_fg_plotmode) {
      case PM_OR:
         colour |= existing;
         break;
      case PM_AND:
         colour &= existing;
         break;
      case PM_XOR:
         colour ^= existing;
         break;
      case PM_INVERT:
         colour = existing ^ 0xFF;
         break;
      case PM_NORMAL : break;
      default:
         break;
      }
   }
   screen->set_pixel(screen, x, y, colour);
}

static void draw_hline(screen_mode_t *screen, int x1, int x2, int y, pixel_t colour) {
   if (x1 > x2) {
      int tmp = x1;
      x1 = x2;
      x2 = tmp;
   }
   for (int x = x1; x <= x2; x++) {
      set_pixel(screen, x, y, colour);
   }
}

static void fill_bottom_flat_triangle(screen_mode_t *screen, int x1, int y1, int x2, int y2, int x3, int y3, pixel_t colour) {
   // Note: y2 and y3 are the same, so the below test is slightly redundant
   if (y1 == y2 || y1 == y3) {
      draw_hline(screen, (int)x2, (int)x3, y1, colour);
   } else {
      float invslope1 = ((float) (x2 - x1)) / ((float) (y1 - y2));
      float invslope2 = ((float) (x3 - x1)) / ((float) (y1 - y3));
      float curx1 = 0.5f + (float)x1;
      float curx2 = curx1;
      for (int scanlineY = y1; scanlineY >= y2; scanlineY--) {
         draw_hline(screen, (int)curx1, (int)curx2, scanlineY, colour);
         curx1 += invslope1;
         curx2 += invslope2;
      }
   }
}

static void fill_top_flat_triangle(screen_mode_t *screen, int x1, int y1, int x2, int y2, int x3, int y3, pixel_t colour) {
   // Note: y1 and y2 are the same, so the below test is slightly redundant
   if (y1 == y3 || y2 == y3) {
      draw_hline(screen, (int)x1, (int)x2, y3, colour);
   } else {
      float invslope1 = ((float) (x3 - x1)) / ((float) (y1 - y3));
      float invslope2 = ((float) (x3 - x2)) / ((float) (y2 - y3));
      float curx1 = 0.5f + (float)x3;
      float curx2 = curx1;
      for (int scanlineY = y3; scanlineY <= y1; scanlineY++) {
         draw_hline(screen, (int)curx1, (int)curx2, scanlineY, colour);
         curx1 -= invslope1;
         curx2 -= invslope2;
      }
   }
}

// Rodders: Arc drawing routines, used by chord and sector fills
static unsigned int arc_quadrant(int x, int y) {
   if (x >= 0) {
      if (y >= 0) {
         return 0;
      } else {
         return 3;
      }
   } else {
      if (y >= 0) {
         return 1;
      } else {
         return 2;
      }
   }
}

static int arc_point(unsigned int q, quadrant_t state, int x, int y, int xs, int ys, int xe, int ye) {
   if (state == Q_ALL) {
      return TRUE;
   }
   if (state == Q_NONE) {
      return FALSE;
   }
   switch (q) {
   case 0:
      if (state == Q_START && x <= xs && y >= ys) {
         return TRUE;
      }
      if (state == Q_END   && x >= xe && y <= ye) {
         return TRUE;
      }
      if (state == Q_BOTH) {
         if (xs > xe) {
            if ((x <= xs && y >= ys) && (x >= xe && y <= ye)) {
               return TRUE;
            }
         } else {
            if ((x <= xs && y >= ys) || (x >= xe && y <= ye)) {
               return TRUE;
            }
         }
      }
      break;
   case 1:
      if (state == Q_START && x <= xs && y <= ys) {
         return TRUE;
      }
      if (state == Q_END   && x >= xe && y >= ye) {
         return TRUE;
      }
      if (state == Q_BOTH) {
         if (xs > xe) {
            if ((x <= xs && y <= ys) && (x >= xe && y >= ye)) {
               return TRUE;
            }
         } else {
            if ((x <= xs && y <= ys) || (x >= xe && y >= ye)) {
               return TRUE;
            }
         }
      }
      break;
   case 2:
      if (state == Q_START && x >= xs && y <= ys) {
         return TRUE;
      }
      if (state == Q_END   && x <= xe && y >= ye) {
         return TRUE;
      }
      if (state == Q_BOTH) {
         if (xs < xe) {
            if ((x >= xs && y <= ys) && (x <= xe && y >= ye)) {
               return TRUE;
            }
         } else {
            if ((x >= xs && y <= ys) || (x <= xe && y >= ye)) {
               return TRUE;
            }
         }
      }
      break;
   case 3:
      if (state == Q_START && x >= xs && y >= ys) {
         return TRUE;
      }
      if (state == Q_END   && x <= xe && y <= ye) {
         return TRUE;
      }
      if (state == Q_BOTH) {
         if (xs < xe) {
            if ((x >= xs && y >= ys) && (x <= xe && y <= ye)) {
               return TRUE;
            }
         } else {
            if ((x >= xs && y >= ys) || (x <= xe && y <= ye)) {
               return TRUE;
            }
         }
      }
      break;
   }
   return FALSE;
}

static void draw_circle(screen_mode_t *screen, int xc, int yc, int r, pixel_t colour) {
   int x = 0;
   int y = r;
   int p = 3 - (2 * r);
   while (x < y) {
      set_pixel(screen, xc + x, yc + y, colour);
      set_pixel(screen, xc + x, yc - y, colour);
      set_pixel(screen, xc + y, yc + x, colour);
      set_pixel(screen, xc - y, yc + x, colour);
      if (x > 0) {
         set_pixel(screen, xc - x, yc + y, colour);
         set_pixel(screen, xc - x, yc - y, colour);
         set_pixel(screen, xc + y, yc - x, colour);
         set_pixel(screen, xc - y, yc - x, colour);
      }
      if (p < 0) {
         p += 4 * x + 6;
         x++;
      } else {
         p += 4 * (x - y) + 10;
         x++;
         y--;
      }
   }
   if (x == y) {
      set_pixel(screen, xc + x, yc + y, colour);
      set_pixel(screen, xc - x, yc + y, colour);
      set_pixel(screen, xc + x, yc - y, colour);
      set_pixel(screen, xc - x, yc - y, colour);
   }
}

static void fill_circle(screen_mode_t *screen, int xc, int yc, int r, pixel_t colour) {
   int x = 0;
   int y = r;
   int p = 3 - (2 * r);
   while (x < y) {
      draw_hline(screen, xc + y, xc - y, yc + x, colour);
      if (x > 0) {
         draw_hline(screen, xc + y, xc - y, yc - x, colour);
      }
      if (p < 0) {
         p += 4 * x + 6;
         x++;
      } else {
         draw_hline(screen, xc + x, xc - x, yc - y, colour);
         draw_hline(screen, xc + x, xc - x, yc + y, colour);
         p += 4 * (x - y) + 10;
         x++;
         y--;
      }
   }
   if (x == y) {
      draw_hline(screen, xc + x, xc - x, yc - y, colour);
      draw_hline(screen, xc + x, xc - x, yc + y, colour);
   }
}

static void draw_normal_ellipse(screen_mode_t *screen, int xc, int yc, int width, int height, pixel_t colour) {
   // Deal with the trivial case of single point
   if (width == 0 && height == 0) {
      set_pixel(screen, xc, yc, colour);
      return;
   }
   // Draw the ellipse
   int a2 = width * width;
   int b2 = height * height;
   int fa2 = 4 * a2, fb2 = 4 * b2;
   int x, y, sigma;
   /* First half */
   for (x = 0, y = height, sigma = 2 * b2 + a2 * (1 - 2 * height); b2 * x <= a2 * y; x++) {
      set_pixel(screen, xc + x, yc + y, colour);
      set_pixel(screen, xc + x, yc - y, colour);
      if (x > 0) {
         set_pixel(screen, xc - x, yc + y, colour);
         set_pixel(screen, xc - x, yc - y, colour);
      }
      if (sigma >= 0) {
         sigma += fa2 * (1 - y);
         y--;
      }
      sigma += b2 * ((4 * x) + 6);
   }
   /* Second half */
   for (x = width, y = 0, sigma = 2 * a2 + b2 * (1 - 2 * width); a2 * y <= b2 * x; y++) {
      set_pixel(screen, xc + x, yc + y, colour);
      set_pixel(screen, xc - x, yc + y, colour);
      if (y > 0) {
         set_pixel(screen, xc + x, yc - y, colour);
         set_pixel(screen, xc - x, yc - y, colour);
      }
      if (sigma >= 0) {
         sigma += fb2 * (1 - x);
         x--;
      }
      sigma += a2 * ((4 * y) + 6);
   }
}

static void draw_sheared_ellipse(screen_mode_t *screen, int xc, int yc, int width, int height, int shear, pixel_t colour) {
   // Draw the ellipse
   if (height == 0) {
      draw_hline(screen, xc - width, xc + width, yc, colour);
   } else {
      float axis_ratio = (float) width / (float) height;
      float shear_per_line = (float) (shear) / (float) height;
      float xshear = 0.0;
      int odd_sequence = 1;
      int y_squared = 0;
      int h_squared = height * height;
      // Maintain the left/right coordinated of the previous, current, and next slices
      // to allow lines to be drawn to make sure the pixels are connected
      int xl_prev = 0;
      int xr_prev = 0;
      int xl_this = 0;
      int xr_this = 0;
      // Start at -1 to allow the pipeline to fill
      for (int y = -1; y < height; y++) {
         float x = axis_ratio * sqrtf((float)(h_squared - y_squared));
         int xl_next = (int) (xshear - x);
         int xr_next = (int) (xshear + x);
         xshear += shear_per_line;
         // It's probably quicker to just use y * y
         y_squared += odd_sequence;
         odd_sequence += 2;
         // Initialize the pipeline for the first slice
         if (y == 0) {
            xl_prev = -xr_next;
            xr_prev = -xl_next;
         }
         // Draw the slice as a single horizintal line
         if (y >= 0) {
            // Left line runs from xl_this rightwards to max(xl_this, max(xl_prev, xl_next) - 1)
            int xl = max(xl_this, max(xl_prev, xl_next) - 1);
            // Right line runs from xr_this leftwards to min(xr_this, min(xr_prev, xr_next) + 1)
            int xr = min(xr_this, min(xr_prev, xr_next) + 1);
            draw_hline(screen, xc + xl_this, xc + xl, yc + y, colour);
            draw_hline(screen, xc + xr_this, xc + xr, yc + y, colour);
            if (y > 0) {
               draw_hline(screen, xc - xl_this, xc - xl, yc - y, colour);
               draw_hline(screen, xc - xr_this, xc - xr, yc - y, colour);
            }
         }
         xl_prev = xl_this;
         xr_prev = xr_this;
         xl_this = xl_next;
         xr_this = xr_next;
      }
      // Draw the final slice
      draw_hline(screen, xc + xl_this, xc + xr_this, yc + height, colour);
      draw_hline(screen, xc - xl_this, xc - xr_this, yc - height, colour);
   }
}

#if 0
static void draw_sheared_ellipse(screen_mode_t *screen, int xc, int yc, int width, int height, int shear, pixel_t colour) {
   // Draw the ellipse
   int a2 = width * width;
   int b2 = height * height;
   int fa2 = 4 * a2, fb2 = 4 * b2;
   int x, y, sigma;
   /* First half */
   for (x = 0, y = height, sigma = 2 * b2 + a2 * (1 - 2 * height); b2 * x <= a2 * y; x++) {
      int s = shear * y / height;
      set_pixel(screen, xc + x + s, yc + y, colour);
      set_pixel(screen, xc + x - s, yc - y, colour);
      if (x > 0) {
         set_pixel(screen, xc - x + s, yc + y, colour);
         set_pixel(screen, xc - x - s, yc - y, colour);
      }
      if (sigma >= 0) {
         sigma += fa2 * (1 - y);
         y--;
      }
      sigma += b2 * ((4 * x) + 6);
   }
   /* Second half */
   for (x = width, y = 0, sigma = 2 * a2 + b2 * (1 - 2 * width); a2 * y <= b2 * x; y++) {
      int s = shear * y / height;
      set_pixel(screen, xc + x + s, yc + y, colour);
      set_pixel(screen, xc - x + s, yc + y, colour);
      if (y > 0) {
         set_pixel(screen, xc + x - s, yc - y, colour);
         set_pixel(screen, xc - x - s, yc - y, colour);
      }
      if (sigma >= 0) {
         sigma += fb2 * (1 - x);
         x--;
      }
      sigma += a2 * ((4 * y) + 6);
   }
}
#endif

static void fill_normal_ellipse(screen_mode_t *screen, int xc, int yc, int width, int height, pixel_t colour) {
   // Deal with the trivial case of single point
   if (width == 0 && height == 0) {
      set_pixel(screen, xc, yc, colour);
      return;
   }
   // Fill the ellipse
   int a2 = width * width;
   int b2 = height * height;
   int fa2 = 4 * a2, fb2 = 4 * b2;
   int x, y, sigma;
   /* First half */
   for (x = 0, y = height, sigma = 2 * b2 + a2 * (1 - 2 * height); b2 * x <= a2 * y; x++) {
      if (sigma >= 0) {
         draw_hline(screen, xc + x, xc - x, yc + y, colour);
         draw_hline(screen, xc + x, xc - x, yc - y, colour);
         sigma += fa2 * (1 - y);
         y--;
      }
      sigma += b2 * ((4 * x) + 6);
   }
   /* Second half */
   for (x = width, y = 0, sigma = 2 * a2 + b2 * (1 - 2 * width); a2 * y <= b2 * x; y++) {
      draw_hline(screen, xc + x, xc - x, yc + y, colour);
      if (y > 0) {
         draw_hline(screen, xc + x, xc - x, yc - y, colour);
      }
      if (sigma >= 0) {
         sigma += fb2 * (1 - x);
         x--;
      }
      sigma += a2 * ((4 * y) + 6);
   }
}

static void fill_sheared_ellipse(screen_mode_t *screen, int xc, int yc, int width, int height, int shear, pixel_t colour) {
   // Fill the ellipse
   if (height == 0) {
      draw_hline(screen, xc - width, xc + width, yc, colour);
   } else {
      float axis_ratio = (float) width / (float) height;
      float shear_per_line = (float) (shear) / (float) height;
      float xshear = 0.0;
      int odd_sequence = 1;
      int y_squared = 0;
      int h_squared = height * height;
      for (int y = 0; y <= height; y++) {
         float x = axis_ratio * sqrtf((float)(h_squared - y_squared));
         int xl = (int) (xshear - x);
         int xr = (int) (xshear + x);
         xshear += shear_per_line;
         // It's probably quicker to just use y * y
         y_squared += odd_sequence;
         odd_sequence += 2;
         // Draw the slice as a single horizintal line
         draw_hline(screen, xc + xl, xc + xr, yc + y, colour);
         if (y > 0) {
            draw_hline(screen, xc - xl, xc - xr, yc - y, colour);
         }
      }
   }
}

#if 0
static void fill_sheared_ellipse(screen_mode_t *screen, int xc, int yc, int width, int height, int shear, pixel_t colour) {
   int a2 = width * width;
   // Fill the ellipse
   int b2 = height * height;
   int fa2 = 4 * a2, fb2 = 4 * b2;
   int x, y, sigma;
   /* First half */
   for (x = 0, y = height, sigma = 2 * b2 + a2 * (1 - 2 * height); b2 * x <= a2 * y; x++) {
      if (sigma >= 0) {
         int s = shear * y / height;
         draw_hline(screen, xc + x + s, xc - x + s, yc + y, colour);
         draw_hline(screen, xc + x - s, xc - x - s, yc - y, colour);
         sigma += fa2 * (1 - y);
         y--;
      }
      sigma += b2 * ((4 * x) + 6);
   }
   /* Second half */
   for (x = width, y = 0, sigma = 2 * a2 + b2 * (1 - 2 * width); a2 * y <= b2 * x; y++) {
      int s = shear * y / height;
      draw_hline(screen, xc + x + s, xc - x + s, yc + y, colour);
      if (y > 0) {
         draw_hline(screen, xc + x - s, xc - x - s, yc - y, colour);
      }
      if (sigma >= 0) {
         sigma += fb2 * (1 - x);
         x--;
      }
      sigma += a2 * ((4 * y) + 6);
   }
}
#endif

// ==========================================================================
// Public methods
// ==========================================================================

void prim_set_fg_plotmode(plotmode_t plotmode) {
   g_fg_plotmode = plotmode;
}

plotmode_t prim_get_fg_plotmode() {
   return g_fg_plotmode;
}

void prim_set_bg_plotmode(plotmode_t plotmode) {
   g_bg_plotmode = plotmode;
}

plotmode_t prim_get_bg_plotmode() {
   return g_bg_plotmode;
}

void prim_set_graphics_area(screen_mode_t *screen, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
   // Reject illegal windows (this is what OS 1.20 does)
   if (x1 < 0 || x1 >= screen->width || y1 < 0 || y1 >= screen->height) {
      return;
   }
   if (x2 < 0 || x2 >= screen->width || y2 < 0 || y2 >= screen->height) {
      return;
   }
   if (x1 >= x2 || y1 >= y2) {
      return;
   }
   // Update the window
   g_x_min = x1;
   g_y_min = y1;
   g_x_max = x2;
   g_y_max = y2;
}

void prim_clear_graphics_area(screen_mode_t *screen, pixel_t colour) {
   prim_fill_rectangle(screen, g_x_min, g_y_min, g_x_max, g_y_max, colour);
}

void prim_set_pixel(screen_mode_t *screen, int x, int y, pixel_t colour) {
   set_pixel(screen, x, y, colour);
}

pixel_t prim_get_pixel(screen_mode_t *screen, int x, int y) {
   return get_pixel(screen, x, y);
}

int prim_on_screen(screen_mode_t *screen, int x, int y) {
   return x >= g_x_min && x <= g_x_max && y >= g_y_min && y <= g_y_max;
}

// Rodders: Line mode support
// Implementation of Bresenham's line drawing algorithm from here:
// http://tech-algorithm.com/articles/drawing-line-using-bresenham-algorithm/
void prim_draw_line(screen_mode_t *screen, int x1, int y1, int x2, int y2, pixel_t colour, uint8_t linemode) {
   int w = x2 - x1;
   int h = y2 - y1;
   int mask = (linemode & 0x38);
   int dotted =     (mask == 0x10 || mask == 0x18 || mask == 0x30 || mask == 0x38); // Dotted line
   int omit_first = (mask == 0x20 || mask == 0x28 || mask == 0x30 || mask == 0x38); // Omit first
   int omit_last =  (mask == 0x08 || mask == 0x18 || mask == 0x28 || mask == 0x38); // Omit last
   int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
   if (w < 0) {
      dx1 = -1;
   } else if (w > 0) {
      dx1 = 1;
   }
   if (h < 0) {
      dy1 = -1;
   } else if (h > 0) {
      dy1 = 1;
   }
   if (w < 0) {
      dx2 = -1;
   } else if (w > 0) {
      dx2 = 1;
   }
   int longest = abs(w);
   int shortest = abs(h);
   if (!(longest > shortest)) {
      longest = abs(h);
      shortest = abs(w);
      if (h < 0) {
         dy2 = -1;
      } else if (h > 0) {
         dy2 = 1;
      }
      dx2 = 0;
   }
   int numerator = longest >> 1 ;
   int x = x1;
   int y = y1;
   if (omit_last) {
      longest--;
   }
   int start = 0;
   if (omit_first) {
      start++;
   }
   for (int i = start; i <= longest; i++) {
      // TODO: Use Dot Pattern set by VDU 23,6
      if ((dotted == 0) || (i & 1)) {
         set_pixel(screen, x, y, colour);
      }
      numerator += shortest;
      if (!(numerator < longest)) {
         numerator -= longest;
         x += dx1;
         y += dy1;
      } else {
         x += dx2;
         y += dy2;
      }
   }
}

void prim_flood_fill(screen_mode_t *screen, int x, int y, pixel_t fill_col, pixel_t ref_col, int c) {
#ifdef DEBUG_VDU
   int maxq = 0;
   printf("Flood fill @ %d,%d with col %"PRIx32"; initial pixel %"PRIx32"\r\n", x, y, fill_col, get_pixel(screen, x, y));
#endif
   if (c && (fill_col == ref_col)) {
      return;
   }
   if (((get_pixel(screen, x, y) == ref_col) ? !c : c)) {
      return;
   }
   flood_queue_x[0] = (int16_t)x;
   flood_queue_y[0] = (int16_t)y;
   flood_queue_rd = 0;
   flood_queue_wr = 1;
   while (flood_queue_rd != flood_queue_wr) {

      x = flood_queue_x[flood_queue_rd];
      y = flood_queue_y[flood_queue_rd];
      flood_queue_rd ++;
      flood_queue_rd &= FLOOD_QUEUE_SIZE - 1;

      if ((get_pixel(screen, x, y) == ref_col) ? !c : c) {
         continue;
      }

      int xl = x;
      int xr = x;
      while (xl > g_x_min && ((get_pixel(screen, xl - 1, y) == ref_col) ? c : !c)) {
         xl--;
      }
      while (xr < g_x_max && ((get_pixel(screen, xr + 1, y) == ref_col) ? c : !c)) {
         xr++;
      }
      for (x = xl; x <= xr; x++) {
         set_pixel(screen, x, y, fill_col);
         if (y > g_y_min && ((get_pixel(screen, x, y - 1) == ref_col) ? c : !c)) {
            flood_queue_x[flood_queue_wr] = (int16_t)x;
            flood_queue_y[flood_queue_wr] = (int16_t)(y - 1);
            flood_queue_wr ++;
            flood_queue_wr &= FLOOD_QUEUE_SIZE - 1;
#ifdef DEBUG_VDU
            if (flood_queue_wr == flood_queue_rd) {
               printf("queue wrapped\r\n");
            }
#endif
         }
         if (y < g_y_max && ((get_pixel(screen, x, y + 1) == ref_col) ? c : !c)) {
            flood_queue_x[flood_queue_wr] = (int16_t)x;
            flood_queue_y[flood_queue_wr] = (int16_t)(y + 1);
            flood_queue_wr ++;
            flood_queue_wr &= FLOOD_QUEUE_SIZE - 1;
#ifdef DEBUG_VDU
            if (flood_queue_wr == flood_queue_rd) {
               printf("queue wrapped\r\n");
            }
#endif
         }
      }
#ifdef DEBUG_VDU
      int size = (FLOOD_QUEUE_SIZE + flood_queue_wr - flood_queue_rd) & (FLOOD_QUEUE_SIZE - 1);
      if (size > maxq) {
         maxq = size;
      }
#endif
   }
#ifdef DEBUG_VDU
   printf("Max queue size = %d\r\n", maxq);
#endif
}

void prim_fill_area(screen_mode_t *screen, int x, int y, pixel_t colour, fill_t mode) {
   int x_left = x;
   int x_right = x;

   // printf("Plot (%d,%d), colour %d, mode %d\r\n", x, y, colour, mode);
   // printf("g_bg_col = %d, g_fg_col = %d\n\r", g_bg_col, g_fg_col);

   pixel_t fg_col = fb_get_g_fg_col();
   pixel_t bg_col = fb_get_g_bg_col();

   if (x < g_x_min  || x > g_x_max || y < g_y_min || y > g_y_max) {
      return;
   }

   switch(mode) {
   case HL_LR_NB:
      if (get_pixel(screen, x, y) != bg_col) {
         return;
      }
      while (get_pixel(screen, x_right + 1, y) == bg_col && x_right + 1 < g_x_max) {
         x_right++;
      }
      while (get_pixel(screen, x_left - 1, y) == bg_col && x_left - 1 > g_x_min) {
         x_left--;
      }
      draw_hline(screen, x_left, x_right, y, colour);
      break;

   case HL_RO_BG:
      if (get_pixel(screen, x, y) == bg_col) {
         return;
      }
      while (get_pixel(screen, x_right + 1, y) != bg_col && x_right + 1 < g_x_max) {
         x_right++;
      }
      draw_hline(screen, x_left, x_right, y, colour);
      break;

   case HL_LR_FG:
      if (get_pixel(screen, x, y) == fg_col) {
         return;
      }
      while (get_pixel(screen, x_right + 1, y) != fg_col && x_right + 1 < g_x_max) {
         x_right++;
      }
      while (get_pixel(screen, x_left - 1, y) != fg_col && x_left - 1 > g_x_min) {
         x_left--;
      }
      draw_hline(screen, x_left, x_right, y, colour);
      break;

   case HL_RO_NF:
      if (get_pixel(screen, x, y) != fg_col) {
         return;
      }
      while (get_pixel(screen, x_right + 1, y) == fg_col && x_right + 1 < g_x_max) {
         x_right++;
      }
      draw_hline(screen, x_left, x_right, y, colour);
      break;

   case AF_NONBG:
      prim_flood_fill(screen, x, y, colour, bg_col, 1);
      break;

   case AF_TOFGD:
      prim_flood_fill(screen, x, y, colour, fg_col, 0);
      break;

   default:
      printf( "Unknown fill mode %d\r\n", mode);
      break;
   }
}


void prim_fill_triangle(screen_mode_t *screen, int x1, int y1, int x2, int y2, int x3, int y3, pixel_t colour) {
   int tmp;
#ifdef USE_V3D
   if (screen->log2bpp > 3) {
      // Flip y axis
      y1 = screen->height - 1 - y1;
      y2 = screen->height - 1 - y2;
      y3 = screen->height - 1 - y3;
      // Draw the triangle
      v3d_draw_triangle(screen, x1, y1, x2, y2, x3, y3, colour);
   } else {
#endif
      // Use Standard Triangle Fill
      // http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
      // sort the three vertices by y-coordinate ascending so v1 is the topmost vertice
      if (y2 > y1) {
         tmp = x1; x1 = x2; x2 = tmp;
         tmp = y1; y1 = y2; y2 = tmp;
      }
      if (y3 > y1) {
         tmp = x1; x1 = x3; x3 = tmp;
         tmp = y1; y1 = y3; y3 = tmp;
      }
      if (y3 > y2) {
         tmp = x2; x2 = x3; x3 = tmp;
         tmp = y2; y2 = y3; y3 = tmp;
      }
      // here we know that y1 >= y2 >= y3
      if (y2 == y3) {
         // trivial case of bottom-flat triangle
         fill_bottom_flat_triangle(screen, x1, y1, x2, y2, x3, y3, colour);
      } else if (y1 == y2) {
         // trivial case of top-flat triangle
         fill_top_flat_triangle(screen, x1, y1, x2, y2, x3, y3, colour);
      } else {
         // general case - split the triangle in a topflat and bottom-flat one
         int x4 = (int)((float)x1 + ((float)(y1 - y2) / (float)(y1 - y3)) * (float)(x3 - x1));
         int y4 = y2;
         fill_bottom_flat_triangle(screen, x1, y1, x2, y2, x4, y4, colour);
         fill_top_flat_triangle(screen, x2, y2, x4, y4, x3, y3, colour);
      }
#ifdef USE_V3D
   }
#endif
}

// Rodders: Draw arc using modified Bresenham algorithm
// Finds start and end quadrants and masks plotting of points

// TODO: Update this to work with non-square pixels

void prim_draw_arc(screen_mode_t *screen, int xc, int yc, int x1, int y1, int x2, int y2, pixel_t colour) {
   // Draw arc using modified Bresenham algorithm
   // Finds start and end quadrants and masks plotting of points
   int radius = calc_radius(xc, yc, x1, y1);
   // Don't use calc_radius for r2 as this rounds up and can lead to gaps and leakage
   int r2 = (int)sqrt((x2-xc)*(x2-xc) + (y2-yc)*(y2-yc));

   // Calc end point
   int x3 = xc + (x2 - xc) * radius / r2;
   int y3 = yc + (y2 - yc) * radius / r2;

   // Set up quadrants
   unsigned int qstart = arc_quadrant(x1 - xc, y1 - yc);
   unsigned int qend = arc_quadrant(x3 - xc, y3 - yc);
   quadrant_t q[4] = {Q_NONE, Q_NONE, Q_NONE, Q_NONE};
   q[qstart] = (qstart == qend) ? Q_BOTH : Q_START;
   if (qstart != qend || (y1 >= yc && x1 < x3) || (y1 < yc && x1 > x3)) {
      for (unsigned int i = qstart + 1; i < qstart + 4; i++) {
         unsigned int j = i % 4;
         if (j == qend) {
            q[j] = Q_END;
            break;
         } else {
            q[j] = Q_ALL;
         }
      }
   }

   int x = 0;
   int y = radius;
   int d = 1 - radius;

   // Do compass points
   if (arc_point(0, q[0], xc, yc + y, x1, y1, x3, y3))
      set_pixel(screen, xc, yc + y, colour);
   if (arc_point(1, q[1], xc - y, yc, x1, y1, x3, y3))
      set_pixel(screen, xc - y, yc, colour);
   if (arc_point(2, q[2], xc, yc - y, x1, y1, x3, y3))
      set_pixel(screen, xc, yc - y, colour);
   if (arc_point(3, q[3], xc + y, yc, x1, y1, x3, y3))
      set_pixel(screen, xc + y, yc, colour);

   while(x < y) {
      if (d < 0) {
         d = d + 2 * x + 3;
         x += 1;
      } else {
         d = d + 2 * (x - y) + 5;
         x += 1;
         y -= 1;
      }
      if (arc_point(0, q[0], xc + x, yc + y, x1, y1, x3, y3)) {
         set_pixel(screen, xc + x, yc + y, colour);
      }
      if (arc_point(3, q[3], xc + x, yc - y, x1, y1, x3, y3)) {
         set_pixel(screen, xc + x, yc - y, colour);
      }
      if (arc_point(1, q[1], xc - x, yc + y, x1, y1, x3, y3)) {
         set_pixel(screen, xc - x, yc + y, colour);
      }
      if (arc_point(2, q[2], xc - x, yc - y, x1, y1, x3, y3)) {
         set_pixel(screen, xc - x, yc - y, colour);
      }
      if (arc_point(0, q[0], xc + y, yc + x, x1, y1, x3, y3)) {
         set_pixel(screen, xc + y, yc + x, colour);
      }
      if (arc_point(3, q[3], xc + y, yc - x, x1, y1, x3, y3)) {
         set_pixel(screen, xc + y, yc - x, colour);
      }
      if (arc_point(1, q[1], xc - y, yc + x, x1, y1, x3, y3)) {
         set_pixel(screen, xc - y, yc + x, colour);
      }
      if (arc_point(2, q[2], xc - y, yc - x, x1, y1, x3, y3)) {
         set_pixel(screen, xc - y, yc - x, colour);
      }
   }
   // Save the screen coordinates of the arc endpoint, for sector/chord drawing
   // (but don't reuse the graphics cursor for this purpose)
   arc_end_x = (int16_t)x3;
   arc_end_y = (int16_t)y3;

   // Find chord centre for flood fill
   int xf = (x1 + x3) / 2;
   int yf = (y1 + y3) / 2;

   // Make xf,yf relative to the centre
   xf -= xc;
   yf -= yc;

   // If chord passes through origin use alternative centre
   if (abs(xf) < 5 && abs(yf) < 5) {
      xf = -(y1 - yc) / 2;
      yf =  (x1 - xc) / 2;
   }
   unsigned int qf = arc_quadrant(xf, yf);
   int rf = calc_radius(0, 0, xf, yf);
   if (rf < 5) rf = radius / 2;
   xf = (xf + xf * radius / rf) / 2;
   yf = (yf + yf * radius / rf) / 2;

   // Invert if the point would not be displayed
   if (!arc_point(qf, q[qf], xc + xf, yc + yf, x1, y1, x3, y3)) {
      xf = - xf;
      yf = - yf;
   }

   // Make xf,yf absolute again
   xf += xc;
   yf += yc;

   arc_fill_x = (int16_t)xf;
   arc_fill_y = (int16_t)yf;
}


void prim_fill_chord(screen_mode_t *screen, int xc, int yc, int x1, int y1, int x2, int y2, pixel_t colour) {
   // Draw the arc that bounds the chord
   prim_draw_arc(screen, xc, yc, x1, y1, x2, y2, colour);
   // The arc drawing sets the arc_end_x/y to the arc endpoint (in screen coordinates)
   prim_draw_line(screen, x1, y1, arc_end_x, arc_end_y, colour, 0);
   // Fill the interior
   prim_flood_fill(screen, arc_fill_x, arc_fill_y, colour, colour, 0);
}

void prim_fill_sector(screen_mode_t *screen, int xc, int yc, int x1, int y1, int x2, int y2, pixel_t colour) {
   // Draw the arc that bounds the sector
   prim_draw_arc(screen, xc, yc, x1, y1, x2, y2, colour);
   // The arc drawing sets the arc_end_x/y to the arc endpoint (in screen coordinates)
   prim_draw_line(screen, arc_end_x, arc_end_y, xc, yc, colour, 0);
   prim_draw_line(screen, xc, yc, x1, y1, colour, 0);
   // Fill the interior
   prim_flood_fill(screen, arc_fill_x, arc_fill_y, colour, colour, 0);
}

// Rodders: Bit Blit
// TODO: implement move (the current code only does copy)
void prim_move_copy_rectangle(screen_mode_t *screen, int x1, int y1, int x2, int y2, int x3, int y3, int move) {
   uint8_t *src;
   uint8_t *dst;
   // Make x1, y1 the bottom left, and x2, y2 the top right
   if (x1 > x2) {
      int tmp = x1;
      x1 = x2;
      x2 = tmp;
   }
   if (y1 > y2) {
      int tmp = y1;
      y1 = y2;
      y2 = tmp;
   }
   // Calculate the rectangle dimensions
   int len = (x2 - x1) + 1;
   int rows = (y2 - y1) + 1;
   // Handle the case where part of the destination rectangle is off screen
   // TODO: there are more cases than this
   if (x3 + len > screen->width) {
      len = screen->width - x3;
   }
   // Convert row length from pixels to bytes
   len *= (1 << (screen->log2bpp - 3));
   uint8_t *fb = (uint8_t *)get_fb_address();
   for (int y = 0; y < rows; y++) {
      if (y3 < y1) {
         src = fb + (screen->height - (y1 + y) - 1) * screen->pitch + x1;
         dst = fb + (screen->height - (y3 + y) - 1) * screen->pitch + x3;
      } else {
         src = fb + (screen->height - (y2 - y) - 1) * screen->pitch + x1;
         dst = fb + (screen->height - (y3 + rows - 1 - y) - 1) * screen->pitch + x3;
      }
      memmove(dst, src, (size_t)len);
   }
}

void prim_fill_rectangle(screen_mode_t *screen, int x1, int y1, int x2, int y2, pixel_t colour) {
   // Ensure y1 < y2
   if (y1 > y2) {
      int tmp = y1;
      y1 = y2;
      y2 = tmp;
   }
   for (int y = y1; y <= y2; y++) {
      draw_hline(screen, x1, x2, y, colour);
   }
}

void prim_fill_parallelogram(screen_mode_t *screen, int x1, int y1, int x2, int y2, int x3, int y3, pixel_t colour) {
   int x4 = x3 - x2 + x1;
   int y4 = y3 - y2 + y1;
   // Fill the parallelogram
   prim_fill_triangle(screen, x1, y1, x2, y2, x3, y3, colour);
   prim_fill_triangle(screen, x1, y1, x4, y4, x3, y3, colour);
}


void prim_draw_circle(screen_mode_t *screen, int xc, int yc, int xr, int yr, pixel_t colour) {
   // Draw the circle
   if (screen->xeigfactor == screen->yeigfactor) {
      // Square pixels
      int r = calc_radius(xc, yc, xr, yr);
      draw_circle(screen, xc, yc, r, colour);
   } else {
      // Rectangular pixels
      int r = calc_radius(xc << screen->xeigfactor, yc << screen->yeigfactor, xr << screen->xeigfactor, yr << screen->yeigfactor);
      int width  = r >> screen->xeigfactor;
      int height = r >> screen->yeigfactor;
      draw_normal_ellipse(screen, xc, yc, width, height, colour);
   }
}

void prim_fill_circle(screen_mode_t *screen, int xc, int yc, int xr, int yr, pixel_t colour) {
   // Fill the circle
   if (screen->xeigfactor == screen->yeigfactor) {
      // Square pixels
      int r = calc_radius(xc, yc, xr, yr);
      fill_circle(screen, xc, yc, r, colour);
   } else {
      int r = calc_radius(xc << screen->xeigfactor, yc << screen->yeigfactor, xr << screen->xeigfactor, yr << screen->yeigfactor);
      int width  = r >> screen->xeigfactor;
      int height = r >> screen->yeigfactor;
      // Rectangular pixels
      fill_normal_ellipse(screen, xc, yc, width, height, colour);
   }
}

void prim_draw_ellipse(screen_mode_t *screen, int xc, int yc, int width, int height, int shear, pixel_t colour) {
   // Draw the ellipse
   if (shear) {
      draw_sheared_ellipse(screen, xc, yc, width, height, shear, colour);
   } else {
      draw_normal_ellipse(screen, xc, yc, width, height, colour);
   }
}

void prim_fill_ellipse(screen_mode_t *screen, int xc, int yc, int width, int height, int shear, pixel_t colour) {
   // Fill the ellipse
   if (shear) {
      fill_sheared_ellipse(screen, xc, yc, width, height, shear, colour);
   } else {
      fill_normal_ellipse(screen, xc, yc, width, height, colour);
   }
}

void prim_draw_character(screen_mode_t *screen, int c, int x_pos, int y_pos, pixel_t colour) {
   // Draw the character
   font_t *font = screen->font;
   int x = x_pos;
   int y = y_pos;
   int scale_w = font->get_scale_w(font);
   int scale_h = font->get_scale_h(font);
   int width   = font->width << font->get_rounding(font);
   int height  = font->height << font->get_rounding(font);
   int p       = c * height;
   int mask    = 1 << (width - 1);
   for (int i = 0; i < height; i++) {
      int data = font->buffer[p++];
      for (int j = 0; j < width; j++) {
         if (data & mask) {
            for (int sx = 0; sx < scale_w; sx++) {
               for (int sy = 0; sy < scale_h; sy++) {
                  set_pixel(screen, x + sx, y + sy, colour);
               }
            }
         }
         x += scale_w;
         data <<= 1;
      }
      x = x_pos;
      y -= scale_h;
   }
}

void prim_reset_sprites(screen_mode_t *screen) {
   for (int i = 0; i < NUM_SPRITES; i++) {
      sprites[i].width = 0;
      sprites[i].height = 0;
      if (sprites[i].data) {
         free(sprites[i].data);
      }
      sprites[i].data = 0;
      sprites[i].data_size = 0;
   }
}

void prim_define_sprite(screen_mode_t *screen, int n, int x1, int y1, int x2, int y2) {
   if (n >= NUM_SPRITES) {
      return;
   }
   sprite_t *sprite = sprites + n;

   // Make x1, y1 the bottom left, and x2, y2 the top right
   if (x1 > x2) {
      int tmp = x1;
      x1 = x2;
      x2 = tmp;
   }
   if (y1 > y2) {
      int tmp = y1;
      y1 = y2;
      y2 = tmp;
   }

   printf("defining sprite %d (%d,%d to %d,%d)\r\n", n, x1, y1, x2, y2);

   // Memory allocation
   sprite->width = x2 - x1 + 1;
   sprite->height = y2 - y1 + 1;
   size_t size = (size_t)sprite->width * (size_t)sprite->height * (1 << (screen->log2bpp - 3));
   if (sprite->data == NULL || sprite->data_size < size) {
      if (sprite->data != NULL) {
         free(sprite->data);
      }
      sprite->data = malloc(size);
      sprite->data_size = size;
   }

   // Read the sprite
   if (screen->log2bpp == 4) {
      uint16_t *data = sprite->data;
      for (int yp = 0; yp < sprite->height; yp++) {
         for (int xp = 0; xp < sprite->width; xp++) {
            *data++ = (uint16_t)get_pixel(screen, x1 + xp, y1 + xp);
         }
      }
   } else if (screen->log2bpp == 5)  {
      uint32_t *data = sprite->data;
      for (int yp = 0; yp < sprite->height; yp++) {
         for (int xp = 0; xp < sprite->width; xp++) {
            *data++ = get_pixel(screen, x1 + xp, y1 + yp);
         }
      }
   } else {
      uint8_t *data = sprite->data;
      for (int yp = 0; yp < sprite->height; yp++) {
         for (int xp = 0; xp < sprite->width; xp++) {
            *data++ = (uint8_t)get_pixel(screen, x1 + xp, y1 + yp);
         }
      }
   }
}

void prim_draw_sprite(screen_mode_t *screen, int n, int x, int y) {
   if (n >= NUM_SPRITES) {
      return;
   }
   sprite_t *sprite = sprites + n;
   // Return if sprite is not properly defined
   if (sprite->width == 0) {
      printf("prim_draw_sprite: %d: width zero\n", n);
      return;
   }
   if (sprite->height == 0) {
      printf("prim_draw_sprite: %d: height zero\n", n);
      return;
   }
   if (sprite->data == NULL) {
      printf("prim_draw_sprite: %d: data null\n", n);
      return;
   }
   if (sprite->data_size == 0) {
      printf("prim_draw_sprite: %d: data_size zero\n", n);
      return;
   }

   printf("drawing sprite %d at %d,%d\r\n", n, x, y);

   // Write the sprite, allowing clipping to take care of off-screen pixels
   if (screen->log2bpp == 4) {
      uint16_t *data = sprite->data;
      for (int yp = 0; yp < sprite->height; yp++) {
         for (int xp = 0; xp < sprite->width; xp++) {
            set_pixel(screen, x + xp, y + yp, *data++);
         }
      }
   } else if (screen->log2bpp == 5)  {
      uint32_t *data = sprite->data;
      for (int yp = 0; yp < sprite->height; yp++) {
         for (int xp = 0; xp < sprite->width; xp++) {
            set_pixel(screen, x + xp, y + yp, *data++);
         }
      }
   } else {
      uint8_t *data   = sprite->data;
      for (int yp = 0; yp < sprite->height; yp++) {
         for (int xp = 0; xp < sprite->width; xp++) {
            set_pixel(screen, x + xp, y + yp, *data++);
         }
      }
   }
}
