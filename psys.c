#include "psys.h"
#include "element.h"

GLuint psys_texture;

particle *m_p;

int m_active, m_size, m_width, m_height;

void psys_add(particle *p)
{
    if(grid_get_type((int)p->pos[0]>>4, (int)p->pos[1]>>4) == G_SOLID) return;
    if(m_active<m_size)
    {
        memcpy(&m_p[m_active], p, sizeof(particle));
        m_active++;
    }
}

void psys_calc()
{
    register int i;
    int y, x, c, n;
    for(i=0; i<m_active; i++)
    {
        const int dx = (int)m_p[i].pos[0] >> 4;
        const int dy = (int)m_p[i].pos[1] >> 4;
        for(y=dy-1; y<=dy+1; y++)
        {
            if(y<0 || y>31) continue;
            for(x=dx-1; x<=dx+1; x++)
            {
                if(x<0 || x>31) continue;
                if(!(grid_get_type(x, y) == G_SOLID))
                {
                    for(c=0; c < grid_get_size(x, y); c++)
                    {
                        n = grid_get(x, y, c);
                        if(n != i) particle_react(&m_p[i], &m_p[n]);
                    }
                }
            }
        }
    }

    grid_clear();

    /* Kill bad particles */
    for(i=0; i<m_active; i++)
    {
        const unsigned char gtype = grid_get_type((int)m_p[i].pos[0]>>4, (int)m_p[i].pos[1]>>4);
        if(gtype == G_SOLID || m_p[i].pos[0]<0 || m_p[i].pos[1]<0 || m_p[i].pos[0]>511 || m_p[i].pos[1]>511)
        {
            m_active--;
            m_p[i] = m_p[m_active];
            continue;
        }
    }

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, psys_texture);
    for(i=0; i<m_active; i++)
    {
        particle_update(&m_p[i]);

        const unsigned char gtype = grid_get_type((int)m_p[i].pos[0]>>4, (int)m_p[i].pos[1]>>4);
        if(gtype == G_UP) m_p[i].vel[1] -= 0.15;
        else if(gtype == G_DOWN) m_p[i].vel[1] += 0.15;
        else if(gtype == G_RIGHT) m_p[i].vel[0] += 0.15;
        else if(gtype == G_LEFT) m_p[i].vel[0] -= 0.15;
        if(element_get(&m_p[i])->flags & E_SHARE)
        {
            if(gtype == G_HOT) m_p[i].energy += 0.15;
            else if(gtype == G_COLD) m_p[i].energy -= 0.15;
        }

        m_p[i].vel[1] += m_p[i].mass * 0.05;
        m_p[i].vel[0] *= m_p[i].dampen;
        m_p[i].vel[1] *= m_p[i].dampen;

        /* Take note that the condition is on the SQUARED value */
        float vlen = m_p[i].vel[0]*m_p[i].vel[0] + m_p[i].vel[1]*m_p[i].vel[1];
        if(vlen > 25.0)
        {
            vlen = (1.0 / sqrtf(vlen)) * 5.0;
            m_p[i].vel[0] *= vlen;
            m_p[i].vel[1] *= vlen;
        }

        const float test[2] = {m_p[i].pos[0] + m_p[i].vel[0], m_p[i].pos[1] + m_p[i].vel[1]};
        const int t[2][2] = {{(int)test[0]>>4, (int)test[1]>>4}, {(int)m_p[i].pos[0]>>4, (int)m_p[i].pos[1]>>4}};
        const char hitx = grid_get_type(t[0][0], t[1][1])==G_SOLID;
        const char hity = grid_get_type(t[1][0], t[0][1])==G_SOLID;
        if(grid_get_type(t[0][0], t[0][1]) != G_SOLID)
        {
            m_p[i].pos[0] += m_p[i].vel[0];
            m_p[i].pos[1] += m_p[i].vel[1];
        }
        else if(!hity && hitx)
        {
            m_p[i].pos[1] += m_p[i].vel[1];
            m_p[i].vel[0] *= -1.0;
        }
        else if(!hitx && hity)
        {
            m_p[i].pos[0] += m_p[i].vel[0];
            m_p[i].vel[1] *= -1.0;
        }
        else
        {
            m_p[i].vel[0] *= -1.0;
            m_p[i].vel[1] *= -1.0;
        }

        if(!(m_p[i].pos[0]<0 || m_p[i].pos[1]<0 || m_p[i].pos[0]>511 || m_p[i].pos[1]>511))
        {
            grid_add(((int)m_p[i].pos[0])>>4, ((int)m_p[i].pos[1])>>4, i);
        }
    }
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void psys_init(const int width, const int height, const int size, GLuint texture)
{
    m_p = malloc(sizeof(particle) * size);
    m_size = size;
    m_width = width;
    m_height = height;
    m_active = 0;
    psys_size = &m_active;
    psys_texture = texture;
    grid_init();
}

