#ifndef grid_h
#define grid_h

#include <stdlib.h>
#include <string.h>

#define G_SOLID (1)

#define G_UP    (2)
#define G_DOWN  (3)
#define G_RIGHT (4)
#define G_LEFT  (5)

#define G_HOT   (6)
#define G_COLD  (7)

/*
    The flag grid (for walls)
    The grid grid (stores particles, used to optimize collision)
    And the size grid (how many particles are in the grid grid at x, y)
*/

void grid_add(const int x, const int y, const int i);

unsigned short grid_get(const int x, const int y, const int i);

void grid_clear();

void grid_init();

void grid_type(const int x, const int y, const unsigned char state);

unsigned char grid_get_type(const int x, const int y);

unsigned char grid_get_size(const int x, const int y);

#endif
