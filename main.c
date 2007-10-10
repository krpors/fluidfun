#include <SDL/SDL.h>

#include "graphics.h"
#include "particle.h"
#include "psys.h"
#include "grid.h"
#include "element.h"

/* Point-to-grid and grid-to-point version macros */
#define PCRD(x,y) ((x)<<4), ((y)<<4)
#define GCRD(x,y) ((x)>>4), ((y)>>4)

/* GUI macros */
#define LINE(x1,y1,x2,y2) {glVertex2f((x1),(y1)); glVertex2f((x2),(y2));}
#define PLINE(x1,y1,x2,y2) {glVertex2i(PCRD((x1),(y1))); glVertex2i(PCRD((x2),(y2)));}
#define GLINE(x1,y1,x2,y2) {glVertex2i(GCRD((x1),(y1))); glVertex2i(GCRD((x2),(y2)));}

int element_pointer = 0;

void define_elements()
{
    element_init();

    /* Water is default*/

    /* Air */
    element_mass(1, 0.75, -0.75);
    element_size(1, 7.0, 10.0);
    element_bias(1, 0.75);

    /* Sand */
    element_flags(2, E_NO_INTERP);
    element_mass(2, 1.5, 1.5);
    element_size(2, 6.0, 6.0);
    element_dampen(2, 0.96, 0.96);
    element_rgb0(2, 0xff, 0xaa, 0x00);
    element_bias(2, 0.0);

    /* Lava */
    element_flags(3, E_NO_INTERP | E_EMIT);
    element_mass(3, 1.5, 1.5);
    element_size(3, 6.0, 6.0);
    element_dampen(3, 0.96, 0.96);
    element_rgb0(3, 0xdd, 0x22, 0x00);
    element_bias(3, 0.0);
    element_emit(3, 0.5);

    /* Foam */
    element_flags(4, E_SHARE);
    element_dampen(4, 0.94, 0.98);
    element_rgb0(4, 0xee, 0xee, 0xff);
    element_rgb1(4, 0x22, 0x22, 0xff);
    element_mass(4, 0.25, 0.75);
    element_size(4, 13.0, 7.5);
    element_bias(4, 0.0);

    /* Conductor */
    element_flags(5, E_SHARE);
    element_dampen(5, 0.3, 0.3);
    element_rgb0(5, 0x22, 0x22, 0x22);
    element_rgb1(5, 0xdd, 0xdd, 0xdd);
    element_mass(5, 0.0, 0.0);
    element_size(5, 5.0, 5.0);
    element_bias(5, 0.5);

    /* Hyper */
    element_flags(6, E_NO_INTERP);
    element_dampen(6, 1.1, 1.1);
    element_rgb0(6, 0x00, 0xff, 0x00);
    element_mass(6, 0.0, 0.0);
    element_size(6, 7.0, 7.0);
    element_bias(6, 0.0);

    /* Slow */
    element_flags(7, E_SHARE);
    element_dampen(7, 0.8, 0.8);
    element_rgb0(7, 0x00, 0xff, 0xff);
    element_rgb1(7, 0xff, 0xff, 0x00);
    element_mass(7, 0.0, 0.0);
    element_size(7, 6.0, 6.0);
    element_bias(7, 0.5);

}

void change_selected(int index)
{
    static const char *element_names[8] = {
        "water", "air", "sand", "lava", "foam", "conductor", "hyper", "slow"
    };
    element_pointer = index;
    SDL_WM_SetCaption(element_names[index], "Fluid");
}

void render_arrow()
{
    glBegin(GL_LINES);
    LINE(0.0, -0.4,  0.0, 0.4);
    LINE(0.0, -0.4, -0.4, 0.0);
    LINE(0.0, -0.4,  0.4, 0.0);
    glEnd();
}

int main(int argc, char **argv)
{
    unsigned char x, y;
    graphics gfx;
    graphics_init(&gfx, 640, 512);

    define_elements();

    const GLuint element_gui = gfx.load_bmp("data/element_gui.bmp");

    change_selected(0);

    GLuint grid_list = glGenLists(1);
    glNewList(grid_list, GL_COMPILE);
    glColor3ub(20, 20, 20);
    glBegin(GL_LINES);
    for(y=0; y<32; y++) PLINE(0, y, 32, y);
    for(x=0; x<32; x++) PLINE(x, 0, x, 32);
    glEnd();
    glEndList();

    psys_init(512, 512, 10000, gfx.load_bmp("data/particle.bmp"));

    while(gfx.events())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);
        glColor3f(0xff, 0xff, 0xff);
        glBindTexture(GL_TEXTURE_2D, element_gui);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2i(512, 0);
        glTexCoord2f(1, 0); glVertex2i(640, 0);
        glTexCoord2f(1, 1); glVertex2i(640, 128);
        glTexCoord2f(0, 1); glVertex2i(512, 128);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        glPushMatrix();
        glScalef(16, 8, 1);
        glTranslatef(33, element_pointer*2+1, 0.0);
        glRotatef(90, 0, 0, 1);
        glColor3ub(0xff, 0x00, 0x00);
        render_arrow();
        glPopMatrix();

        for(y=0; y<32; y++)
        {
            for(x=0; x<32; x++)
            {
                unsigned char gtype = grid_get_type(x, y);
                if(gtype)
                {
                    glPushMatrix();
                    glScalef(16, 16, 1);
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
                        else if(gtype == G_HOT) glColor3ub(255, 128, 128);
                        else if(gtype == G_COLD) glColor3ub(128, 128, 255);
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

        SDL_GL_SwapBuffers();

        // Stall when needed to smooth frames
        while(gfx.sync_time(4)){}

        if(gfx.but[SDL_BUTTON_RIGHT] && *gfx.mouse_x < 512)
        {
            if(gfx.key[SDLK_LSHIFT] || gfx.key[SDLK_RSHIFT]) grid_type(GCRD(*gfx.mouse_x, *gfx.mouse_y), 0);
            else if(gfx.key[SDLK_h]) grid_type(GCRD(*gfx.mouse_x, *gfx.mouse_y), G_HOT);
            else if(gfx.key[SDLK_c]) grid_type(GCRD(*gfx.mouse_x, *gfx.mouse_y), G_COLD);
            else if(gfx.key[SDLK_UP]) grid_type(GCRD(*gfx.mouse_x, *gfx.mouse_y), G_UP);
            else if(gfx.key[SDLK_DOWN]) grid_type(GCRD(*gfx.mouse_x, *gfx.mouse_y), G_DOWN);
            else if(gfx.key[SDLK_RIGHT]) grid_type(GCRD(*gfx.mouse_x, *gfx.mouse_y), G_RIGHT);
            else if(gfx.key[SDLK_LEFT]) grid_type(GCRD(*gfx.mouse_x, *gfx.mouse_y), G_LEFT);
            else grid_type(GCRD(*gfx.mouse_x, *gfx.mouse_y), G_SOLID);
        }

        if(gfx.but[SDL_BUTTON_LEFT])
        {
            if(*gfx.mouse_x < 512)
            {
                particle p;
                p.vel[0] = 0.0;
                p.vel[1] = 0.0;
                p.pos[0] = *gfx.mouse_x + (rand()%11) - 5;
                p.pos[1] = *gfx.mouse_y + (rand()%11) - 5;
                particle_factory(&p, element_pointer);
                psys_add(&p);
            }
            else if(*gfx.mouse_y < 128) change_selected((int)((float)(*gfx.mouse_y) / 16));
        }

        for(x=0; x<8; x++) if(gfx.onkey(SDLK_1+x)) change_selected(x);

    }
    gfx.free();
    return 0;
}
