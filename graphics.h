#ifndef graphics_h
#define graphics_h

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

typedef struct
{
    void (*free)();
    char (*events)();
    GLuint (*load_bmp)(const char *src);
    char (*onkey)(const unsigned short code);
    char (*onbut)(const unsigned char code);
    char (*sync_time)(const unsigned int t);
    int *mouse_x, *mouse_y;
    char *key;
    char *but;
} graphics;

void graphics_init(graphics *gfx, const int width, const int height);

#endif

