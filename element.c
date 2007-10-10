#include "element.h"

element elements[256];

void element_init()
{
    int i;
    for(i=0; i<256; i++)
    {
        element_mass(i, 0.75, -0.75);
        element_size(i, 7.0, 10.0);
        element_dampen(i, 0.98, 0.99);
        element_rgb0(i, 0x00, 0x00, 0xff);
        element_rgb1(i, 0xff, 0x00, 0x00);
        element_flags(i, E_BIAS | E_SHARE);
        element_bias(i, 0.25);
    }
}

void element_flags(const unsigned char type, const unsigned char flags)
{
    elements[type].flags = flags;
}

void element_bias(const unsigned char type, const float bias)
{
    elements[type].bias = bias;
}

void element_emit(const unsigned char type, const float emit)
{
    elements[type].emit = emit;
}

void element_mass(const unsigned char type, const float a, const float b)
{
    element *e = &elements[type];
    e->mass[0] = a;
    e->mass[1] = b;
}

void element_dampen(const unsigned char type, const float a, const float b)
{
    element *e = &elements[type];
    e->dampen[0] = a;
    e->dampen[1] = b;
}

void element_size(const unsigned char type, const float a, const float b)
{
    element *e = &elements[type];
    e->size[0] = a;
    e->size[1] = b;
}

void element_rgb0(const unsigned char type, const GLubyte r, const GLubyte g, const GLubyte b)
{
    element *e = &elements[type];
    e->rgb[0][0] = r;
    e->rgb[0][1] = g;
    e->rgb[0][2] = b;
}

void element_rgb1(const unsigned char type, const GLubyte r, const GLubyte g, const GLubyte b)
{
    element *e = &elements[type];
    e->rgb[1][0] = r;
    e->rgb[1][1] = g;
    e->rgb[1][2] = b;
}

void element_apply(particle *p)
{
    element *e = &elements[p->type];

    if(e->flags & E_NO_INTERP)
    {
        p->mass =   e->mass[0];
        p->dampen = e->dampen[0];
        p->size =   e->size[0];
        p->rgb[0] = e->rgb[0][0];
        p->rgb[1] = e->rgb[0][1];
        p->rgb[2] = e->rgb[0][2];
    }
    else
    {
        const float b = p->energy, a = 1.0-b;
        p->mass =   e->mass[0]*a + e->mass[1]*b;
        p->dampen = e->dampen[0]*a + e->dampen[1]*b;
        p->size =   e->size[0]*a + e->size[1]*b;
        p->rgb[0] = (GLubyte)((float)e->rgb[0][0]*a + (float)e->rgb[1][0]*b);
        p->rgb[1] = (GLubyte)((float)e->rgb[0][1]*a + (float)e->rgb[1][1]*b);
        p->rgb[2] = (GLubyte)((float)e->rgb[0][2]*a + (float)e->rgb[1][2]*b);
    }

    if(e->flags & E_BIAS)
    {
        if(p->energy < e->bias) p->energy += 0.0005;
        else if(p->energy > e->bias) p->energy -= 0.0005;
    }
}

element *element_get(const particle *p)
{
    return &elements[p->type];
}
