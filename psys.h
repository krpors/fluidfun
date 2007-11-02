#ifndef particle_sys_h
#define particle_sys_h

#include <GL/gl.h>
#include <string.h>
#include <stdio.h>

#include "grid.h"
#include "particle.h"

void psys_add(particle *p);
void psys_sub(const int x, const int y);
void psys_calc();
void psys_init(const int width, const int height, const int size);
int *psys_size;

#endif
