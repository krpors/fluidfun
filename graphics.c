#include "graphics.h"

GLuint particle_texture;
SDL_Surface *m_surf;
char m_key[512], m_but[256];
int m_width, m_height, m_mouse_x, m_mouse_y, m_last_tickcount;

void graphics_free()
{
    SDL_FreeSurface(m_surf);
    SDL_Quit();
}

char graphics_events()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                m_key[event.key.keysym.sym] = 1;
            break;

            case SDL_KEYUP:
                m_key[event.key.keysym.sym] = 0;
            break;

            case SDL_MOUSEBUTTONDOWN:
                m_but[event.button.button] = 1;
            break;

            case SDL_MOUSEBUTTONUP:
                m_but[event.button.button] = 0;
            break;

            case SDL_MOUSEMOTION:
                m_mouse_x = event.motion.x;
                m_mouse_y = event.motion.y;
            break;

            case SDL_QUIT:
                m_key[SDLK_ESCAPE] = 1;
            break;
        }
    }
    return !m_key[SDLK_ESCAPE];
}

char graphics_onkey(const unsigned short code)
{
    const char c = m_key[code];
    m_key[code] = 0;
    return c;
}

char graphics_onbut(const unsigned char code)
{
    const char c = m_but[code];
    m_but[code] = 0;
    return c;
}

char graphics_sync_time(const unsigned int t)
{
    const unsigned int tickcount = SDL_GetTicks();
    if(tickcount - m_last_tickcount < t) return 1;
    m_last_tickcount = tickcount;
    return 0;
}

void graphics_sdl_and_gl()
{
    SDL_Init(SDL_INIT_VIDEO);
    const SDL_VideoInfo* n = SDL_GetVideoInfo();
    const int f = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;
    m_surf = SDL_SetVideoMode(m_width, m_height, n->vfmt->BitsPerPixel, f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_width, m_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glLineWidth(2);
}

GLuint graphics_load_bmp(const char* src)
{
    SDL_Surface* s = SDL_LoadBMP(src);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, s->w, s->h, GL_RGB, GL_UNSIGNED_BYTE, s->pixels);
    SDL_FreeSurface(s);
    return texture;
}

void graphics_init(graphics *gfx, const int width, const int height)
{
    m_width = width;
    m_height = height;
    int i;
    for(i=0; i<512; i++) m_key[i] = 0;
    for(i=0; i<256; i++) m_but[i] = 0;
    m_last_tickcount = 0;
    gfx->free = graphics_free;
    gfx->events = graphics_events;
    gfx->load_bmp = graphics_load_bmp;
    gfx->onkey = graphics_onkey;
    gfx->onbut = graphics_onbut;
    gfx->sync_time = graphics_sync_time;
    gfx->mouse_x = &m_mouse_x;
    gfx->mouse_y = &m_mouse_y;
    gfx->key = m_key;
    gfx->but = m_but;
    graphics_sdl_and_gl();
}

