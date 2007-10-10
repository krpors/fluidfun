#ifndef element_h
#define element_h

#include "particle.h"

#define E_NO_INTERP (1<<0)
#define E_SHARE     (1<<1)
#define E_EMIT      (1<<2)
#define E_BIAS      (1<<3)

typedef struct
{
    unsigned char flags;
    float bias;
    float emit;
    float mass[2];
    float dampen[2];
    float size[2];
    GLubyte rgb[2][3];
} element;

void element_init();

void element_flags(const unsigned char type, const unsigned char flags);

void element_bias(const unsigned char type, const float bias);

void element_emit(const unsigned char type, const float emit);

void element_mass(const unsigned char type, const float a, const float b);

void element_dampen(const unsigned char type, const float a, const float b);

void element_size(const unsigned char type, const float a, const float b);

void element_rgb0(const unsigned char type, const GLubyte r, const GLubyte g, const GLubyte b);

void element_rgb1(const unsigned char type, const GLubyte r, const GLubyte g, const GLubyte b);

void element_apply(particle *p);

element *element_get(const particle *p);

#endif
