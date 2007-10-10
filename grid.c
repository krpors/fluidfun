#include "grid.h"

unsigned short grid_grid[32][32][64];
unsigned char grid_size[32][32], grid_flags[32][32];

void grid_add(const int x, const int y, const int i)
{
    if(grid_size[y][x]<64)
    {
        grid_grid[y][x][grid_size[y][x]] = i;
        grid_size[y][x]++;
    }
}

unsigned short grid_get(const int x, const int y, const int i)
{
    return grid_grid[y][x][i];
}

void grid_clear()
{
    memset(grid_size, 0, 32*32);
}

void grid_init()
{
    memset(grid_flags, 0, 32*32);
    grid_clear();
    int x, y;
    for(x=0; x<32; x++)
    {
        for(y=0; y<32; y++)
        {
            if(!x || !y || x==31 || y==31) grid_flags[y][x] = G_SOLID;
        }
    }
}

void grid_type(const int x, const int y, const unsigned char state)
{
    grid_flags[y][x] = state;
}

unsigned char grid_get_type(const int x, const int y)
{
    return grid_flags[y][x];
}

unsigned char grid_get_size(const int x, const int y)
{
    return grid_size[y][x];
}

