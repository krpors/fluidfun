#include "grid.h"

void *grid_grid[64][64][32];
unsigned char grid_size[64][64], grid_flags[64][64][2];

void grid_add(const unsigned char x, const unsigned char y, void *i)
{
    if(grid_size[y][x]<32)
    {
        grid_grid[y][x][grid_size[y][x]] = i;
        grid_size[y][x]++;
    }
}

void *grid_get(const unsigned char x, const unsigned char y, const unsigned char i)
{
    return grid_grid[y][x][i];
}

void grid_clear()
{
    memset(grid_size, 0, 64*64);
}

void grid_init()
{
    memset(grid_flags, 0, 64*64);
    grid_clear();
    int x, y;
    for(x=0; x<64; ++x)
    {
        for(y=0; y<64; ++y)
        {
            if(!x || !y || x==63 || y==63) grid_flags[y][x][0] = G_SOLID;
            grid_flags[y][x][1] = 0;
        }
    }
}

void grid_type(const unsigned char x, const unsigned char y, const unsigned char type)
{
    grid_flags[y][x][0] = type;
}

void grid_data(const unsigned char x, const unsigned char y, const unsigned char data)
{
    grid_flags[y][x][1] = data;
}

unsigned char grid_get_type(const unsigned char x, const unsigned char y)
{
    return grid_flags[y][x][0];
}

unsigned char grid_get_data(const unsigned char x, const unsigned char y)
{
    return grid_flags[y][x][1];
}

unsigned char grid_get_size(const unsigned char x, const unsigned char y)
{
    return grid_size[y][x];
}

