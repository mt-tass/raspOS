#include "stdint.h"
#ifndef FB_H
#define FB_H

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} color;

#define COLOR_BLACK ((color){0x00, 0x00, 0x00, 0xFF})
#define COLOR_WHITE ((color){0xFF, 0xFF, 0xFF, 0xFF})
#define COLOR_RED ((color){0xFF, 0x00, 0x00, 0xFF})
#define COLOR_GREEN ((color){0x00, 0xFF, 0x00, 0xFF})
#define COLOR_BLUE ((color){0x00, 0x00, 0xFF, 0xFF})
#define COLOR_YELLOW ((color){0xFF, 0xFF, 0x00, 0xFF})
#define COLOR_CYAN ((color){0x00, 0xFF, 0xFF, 0xFF})
#define COLOR_MAGENTA ((color){0xFF, 0x00, 0xFF, 0xFF})

int fb_init(uint32_t width , uint32_t height , uint32_t depth);
uint32_t fb_get_width(void);
uint32_t fb_get_height(void);
uint32_t fb_get_pitch(void);
uint8_t* fb_get_buffer(void);
void fb_draw_pixel(uint32_t x , uint32_t y , color c);
void fb_fill_screen(color c);
void fb_draw_rectangle(uint32_t x , uint32_t y , uint32_t w , uint32_t h , color c );
void fb_draw_char(uint32_t x , uint32_t y , char c , color fg , color bg);
void fb_draw_string(uint32_t x , uint32_t y , const char *str , color fg , color bg);


#endif