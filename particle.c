#include "particle.h"
#include "element.h"

void particle_factory(particle *p, const unsigned char type)
{
    p->type = type;
    p->energy = element_get(p)->bias;
}

void particle_update(particle *p)
{
    if(p->net_energy_count)
    {
        p->net_energy /= p->net_energy_count;
        p->energy = p->energy * 0.7 + p->net_energy * 0.3;
        p->net_energy = 0.0;
        p->net_energy_count = 0;
    }

    if(p->energy > 1.0) p->energy = 1.0;
    else if(p->energy < 0.0) p->energy = 0.0;

    element_apply(p);

    glPushMatrix();
    glTranslatef(p->pos[0], p->pos[1], 0);
    glScalef(p->size*0.55, p->size*0.55, 1);
    glColor3ubv(p->rgb);
    glBegin(GL_QUADS);
    glTexCoord2i(0,0); glVertex2i(-1, -1);
    glTexCoord2i(1,0); glVertex2i( 1, -1);
    glTexCoord2i(1,1); glVertex2i( 1,  1);
    glTexCoord2i(0,1); glVertex2i(-1,  1);
    glEnd();
    glPopMatrix();
}

void particle_react(particle *a, const particle *b)
{
    float dvec[2];
    dvec[0] = a->pos[0] - b->pos[0];
    dvec[1] = a->pos[1] - b->pos[1];
    const float d = dvec[0]*dvec[0] + dvec[1]*dvec[1];
    const float ave_att_dist = a->size*a->size + b->size*b->size;

    if(d <= ave_att_dist)
    {
        float temp = sqrtf(d);
        if(temp > 0.0)
        {
            temp = 1.0 / temp;
            dvec[0] *= temp;
            dvec[1] *= temp;
        }
        else
        {
            temp = 0.1;
            dvec[0] = 0.0;
            dvec[1] = -1.0;
        }

        const float ta = a->size - 1.0;
        const float tb = b->size - 1.0;
        const float ave_rep_dist = ta*ta + tb*tb;

        if(d >= ave_rep_dist) temp = (1.0-d/ave_att_dist) * -0.01;
        else temp = 1.0-d/ave_rep_dist;
        a->vel[0] += dvec[0] * temp;
        a->vel[1] += dvec[1] * temp;

        if(element_get(a)->flags & E_SHARE)
        {
            if(element_get(b)->flags & E_EMIT) a->energy += element_get(b)->emit;
            if(element_get(b)->flags & E_SHARE)
            {
                a->net_energy +=  b->energy;
                a->net_energy_count++;
            }
        }
    }
}
