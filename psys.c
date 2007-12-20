#include "psys.h"
#include "element.h"

particle *m_p;

int m_active, m_size, m_width, m_height;

void psys_add(particle *p)
{
    if(grid_get_type((int)p->pos[0]>>3, (int)p->pos[1]>>3) == G_SOLID) return;
    if(m_active<m_size)
    {
        memcpy(&m_p[m_active], p, sizeof(particle));
        m_active++;
    }
}

void psys_calc()
{
    particle *e = m_p + m_active;
    for(particle *i=m_p; i<e; ++i)
    {
        const float recip = 1.0 / 8;
        int dx1 = (int)((i->pos[0] - i->size * 2) * recip);
        int dy1 = (int)((i->pos[1] - i->size * 2) * recip);
        int dx2 = (int)((i->pos[0] + i->size * 2) * recip);
        int dy2 = (int)((i->pos[1] + i->size * 2) * recip);
        if(dx1 < 0) dx1 = 0;
        else if(dx2 > 63) dx2 = 63;
        if(dy1 < 0) dy1 = 0;
        else if(dy2 > 63) dy2 = 63;
        for(int y=dy1; y<=dy2; ++y)
        {
            for(int x=dx1; x<=dx2; ++x)
            {
                if(grid_get_type(x, y) != G_SOLID)
                {
                    const unsigned int size = grid_get_size(x, y);
                    for(unsigned char c=0; c < size; ++c)
                    {
                        particle *n = grid_get(x, y, c);
                        if(n != i) particle_react(i, n);
                    }
                }
            }
        }
    }
    grid_clear();

    /* Kill bad particles */
    for(particle *i=m_p; i<m_p+m_active; ++i)
    {
        const unsigned char gtype = grid_get_type((int)i->pos[0]>>3, (int)i->pos[1]>>3);
        if(gtype == G_VOID || gtype == G_SOLID || i->pos[0]<0 || i->pos[1]<0 || i->pos[0]>511 || i->pos[1]>511)
        {
            m_active--;
            *i = m_p[m_active];
            continue;
        }
        if(i->birth > -1 && element_get(i)->flags & E_LIFESPAN)
        {
            if(i->birth + element_get(i)->lifespan < SDL_GetTicks())
            {
                m_active--;
                *i = m_p[m_active];
                continue;
            }
        }
    }

    e = m_p + m_active;
    for(particle *i=m_p; i<e; ++i)
    {
        const element *et = element_get(i);
        if(i->net_energy_count)
        {
            i->net_energy /= i->net_energy_count;
            i->energy = i->energy * 0.8 + i->net_energy * 0.2;
            i->net_energy = 0.0;
            i->net_energy_count = 0;
        }

        if(i->energy > 1.0) i->energy = 1.0;
        else if(i->energy < 0.0) i->energy = 0.0;

        element_apply(i);

        if(i->energy > 0.9)
        {
            if(et->hot_state > -1) i->next_type = et->hot_state;
        }
        else if(i->energy < 0.1)
        {
            if(et->cold_state > -1) i->next_type = et->cold_state;
        }

        const unsigned char gtype = grid_get_type((int)i->pos[0]>>3, (int)i->pos[1]>>3);
        if(gtype == G_UP) i->vel[1] -= 0.1;
        else if(gtype == G_DOWN) i->vel[1] += 0.1;
        else if(gtype == G_RIGHT) i->vel[0] += 0.1;
        else if(gtype == G_LEFT) i->vel[0] -= 0.1;
        if(et->flags & E_SHARE)
        {
            if(gtype == G_HOT) i->energy += 0.2;
            else if(gtype == G_COLD) i->energy -= 0.2;
        }

        i->vel[1] += i->mass * 0.055;
        i->vel[0] *= i->dampen;
        i->vel[1] *= i->dampen;

        /* Take note that the condition is on the SQUARED value */
        float vlen = i->vel[0]*i->vel[0] + i->vel[1]*i->vel[1];
        if(vlen > 16.0)
        {
            vlen = (1.0 / sqrtf(vlen)) * 4.0;
            i->vel[0] *= vlen;
            i->vel[1] *= vlen;
        }

        const float test[2] = {i->pos[0] + i->vel[0], i->pos[1] + i->vel[1]};
        const int t[2][2] = {{(int)test[0]>>3, (int)test[1]>>3}, {(int)i->pos[0]>>3, (int)i->pos[1]>>3}};
        const char hitx = grid_get_type(t[0][0], t[1][1])==G_SOLID;
        const char hity = grid_get_type(t[1][0], t[0][1])==G_SOLID;
        if(grid_get_type(t[0][0], t[0][1]) != G_SOLID)
        {
            i->pos[0] += i->vel[0];
            i->pos[1] += i->vel[1];
        }
        else if(!hity && hitx)
        {
            i->pos[1] += i->vel[1];
            i->vel[0] *= -1.0;
        }
        else if(!hitx && hity)
        {
            i->pos[0] += i->vel[0];
            i->vel[1] *= -1.0;
        }
        else
        {
            i->vel[0] *= -1.0;
            i->vel[1] *= -1.0;
        }

        if(!(i->pos[0]<0 || i->pos[1]<0 || i->pos[0]>511 || i->pos[1]>511))
        {
            grid_add(((int)i->pos[0])>>3, ((int)i->pos[1])>>3, i);
        }
    }
    glDrawArrays(GL_POINTS, 0, m_active);
}

void psys_init(const int width, const int height, const int size)
{
    m_p = malloc(sizeof(particle) * size);
    m_size = size;
    m_width = width;
    m_height = height;
    m_active = 0;
    psys_size = &m_active;
    grid_init();
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(particle), m_p[0].rgb);
    glVertexPointer(2, GL_FLOAT, sizeof(particle), m_p[0].pos);
}

