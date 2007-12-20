#include <SDL/SDL.h>

#include "graphics.h"
#include "particle.h"
#include "psys.h"
#include "grid.h"
#include "element.h"
#include "config.h"
#include "gui.h"

/* Point-to-grid and grid-to-point version macros */
#define PCRD(x,y) ((x)<<3), ((y)<<3)
#define GCRD(x,y) ((x)>>3), ((y)>>3)

/* GUI macros */
#define LINE(x1,y1,x2,y2) {glVertex2f((x1),(y1)); glVertex2f((x2),(y2));}
#define PLINE(x1,y1,x2,y2) {glVertex2i(PCRD((x1),(y1))); glVertex2i(PCRD((x2),(y2)));}
#define GLINE(x1,y1,x2,y2) {glVertex2i(GCRD((x1),(y1))); glVertex2i(GCRD((x2),(y2)));}

int element_count = 0;
int element_pointer = 0;
int xy_pointer[2] = {1, 1};
int next_key = 0;

void change_selected(int delta)
{
    if(element_pointer + delta > element_count - 1) element_pointer = 0;
    else if(element_pointer + delta < 0) element_pointer = element_count - 1;
    else element_pointer += delta;
    SDL_WM_SetCaption(element_get_name(element_pointer), "Fluid");
}

inline void render_arrow()
{
    glBegin(GL_LINES);
    LINE(0.0, -0.4,  0.0, 0.4);
    LINE(0.0, -0.4, -0.4, 0.0);
    LINE(0.0, -0.4,  0.4, 0.0);
    glEnd();
}

int main(int argc, char **argv)
{
    graphics gfx;
    graphics_init(&gfx, 640, 512, "data/FreeMono.ttf");

    gui ui;
    gui_init(&ui, &gfx, 128, 512);

    element_init();
    element_count = config_parse(&ui, "./data/config.txt");

    gui_build(&ui);

    change_selected(0);

    GLuint grid_list = glGenLists(1);
    glNewList(grid_list, GL_COMPILE);
    glColor3ub(20, 20, 20);
    glBegin(GL_LINES);
    for(int y=0; y<64; ++y) PLINE(0, y, 64, y);
    for(int x=0; x<64; ++x) PLINE(x, 0, x, 64);
    glEnd();
    glEndList();

    psys_init(512, 512, 10000);

    while(graphics_events(&gfx))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        for(int y=0; y<64; ++y)
        {
            for(int x=0; x<64; ++x)
            {
                unsigned char gtype = grid_get_type(x, y);
                if(gtype)
                {
                    glPushMatrix();
                    glScalef(8, 8, 1);
                    if(gtype >= G_UP && gtype <= G_LEFT)
                    {
                        glTranslatef(x+0.5, y+0.5, 0.0);
                        if(gtype == G_DOWN) glRotatef(180, 0, 0, 1);
                        else if(gtype == G_RIGHT) glRotatef(90, 0, 0, 1);
                        else if(gtype == G_LEFT) glRotatef(270, 0, 0, 1);
                        glColor3ub(0x00, 0xaa, 0x00);
                        render_arrow();
                    }
                    else
                    {
                        if(gtype == G_SOLID) glColor3ub(128, 128, 128);
                        else if(gtype == G_VOID) glColor3ub(64, 64, 64);
                        else if(gtype == G_HOT) glColor3ub(255, 128, 128);
                        else if(gtype == G_COLD) glColor3ub(128, 128, 255);
                        else if(gtype == G_EMIT)
                        {
                            glColor3ub(255, 64, 255);
                            particle p;
                            p.vel[0] = 0.0;
                            p.vel[1] = 0.0;
                            p.pos[0] = (x << 3) + (rand() % 8);
                            p.pos[1] = (y << 3) + (rand() % 8);
                            particle_factory(&p, grid_get_data(x, y));
                            psys_add(&p);
                        }
                        glTranslatef(x, y, 0);
                        glBegin(GL_QUADS);
                        glVertex2i(0, 0);
                        glVertex2i(1, 0);
                        glVertex2i(1, 1);
                        glVertex2i(0, 1);
                        glEnd();
                    }
                    glPopMatrix();
                }
            }
        }

        glCallList(grid_list);

        psys_calc();

        gui_render(&ui);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPushMatrix();
        glScalef(8, 8, 1);
        glTranslatef(xy_pointer[0], xy_pointer[1], 0.0);
        glColor3ub(0xff, 0xff, 0xff);
        glBegin(GL_QUADS);
        glVertex2f(0.0, 0.0);
        glVertex2f(1.0, 0.0);
        glVertex2f(1.0, 1.0);
        glVertex2f(0.0, 1.0);
        glEnd();
        glPopMatrix();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glPushMatrix();
        glScalef(8, 8, 1);
        glTranslatef(64.7, element_pointer*2+1, 0.0);
        glRotatef(90, 0, 0, 1);
        glColor3ub(0xff, 0x00, 0x00);
        render_arrow();
        glPopMatrix();

        SDL_GL_SwapBuffers();

        const unsigned int clock = SDL_GetTicks();

        // Stall when needed to smooth frames
        while(graphics_sync_time(&gfx, 4)){}

        void do_key()
        {
            if(gfx.key[SDLK_LSHIFT] || gfx.key[SDLK_RSHIFT])
            {
                next_key = clock + 40;
            }
            else
            {
                next_key = clock + 140;
            }
        }

        if(next_key < clock)
        {
            if(gfx.key[SDLK_UP]) {xy_pointer[1]--; do_key();}
            else if(gfx.key[SDLK_DOWN]) {xy_pointer[1]++; do_key();}
            if(gfx.key[SDLK_RIGHT]) {xy_pointer[0]++; do_key();}
            else if(gfx.key[SDLK_LEFT]) {xy_pointer[0]--; do_key();}

            if(xy_pointer[0] < 0) xy_pointer[0] = 63;
            else if(xy_pointer[0] > 63) xy_pointer[0] = 0;
            if(xy_pointer[1] < 0) xy_pointer[1] = 63;
            else if(xy_pointer[1] > 63) xy_pointer[1] = 0;
        }

        if(gfx.key[SDLK_x]) grid_type(xy_pointer[0], xy_pointer[1], 0);
        else if(gfx.key[SDLK_h]) grid_type(xy_pointer[0], xy_pointer[1], G_HOT);
        else if(gfx.key[SDLK_c]) grid_type(xy_pointer[0], xy_pointer[1], G_COLD);
        else if(gfx.key[SDLK_v]) grid_type(xy_pointer[0], xy_pointer[1], G_VOID);
        else if(gfx.key[SDLK_w]) grid_type(xy_pointer[0], xy_pointer[1], G_UP);
        else if(gfx.key[SDLK_s]) grid_type(xy_pointer[0], xy_pointer[1], G_DOWN);
        else if(gfx.key[SDLK_a]) grid_type(xy_pointer[0], xy_pointer[1], G_LEFT);
        else if(gfx.key[SDLK_d]) grid_type(xy_pointer[0], xy_pointer[1], G_RIGHT);
        else if(gfx.key[SDLK_z])
        {
            grid_type(xy_pointer[0], xy_pointer[1], G_EMIT);
            grid_data(xy_pointer[0], xy_pointer[1], element_pointer);
        }
        else if(gfx.key[SDLK_SPACE]) grid_type(xy_pointer[0], xy_pointer[1], G_SOLID);
        else if(gfx.key[SDLK_RETURN])
        {
            particle p;
            p.vel[0] = 0.0;
            p.vel[1] = 0.0;
            p.pos[0] = (xy_pointer[0]<<3) + (rand()%8);
            p.pos[1] = (xy_pointer[1]<<3) + (rand()%8);
            particle_factory(&p, element_pointer);
            psys_add(&p);
        }

        if(graphics_onkey(&gfx, SDLK_F1))
        {
            element_count = config_parse(&ui, "./data/config.txt");
            gui_build(&ui);
        }


        if(graphics_onkey(&gfx, SDLK_PAGEDOWN))
        {
            change_selected(1);
        }
        else if(graphics_onkey(&gfx, SDLK_PAGEUP))
        {
            change_selected(-1);
        }

    }
    graphics_free(&gfx);
    return 0;
}
