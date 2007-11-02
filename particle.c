#include "particle.h"
#include "element.h"

void particle_factory(particle *p, const unsigned char type)
{
    p->type = type;
    p->next_type = -1;
    p->energy = element_get(p)->bias;
    p->net_energy = element_get(p)->bias;
    p->net_energy_count = 1;
    p->birth = -1;
}

void particle_react(particle *a, const particle *b)
{
    float dvec[2] = {a->pos[0] - b->pos[0], a->pos[1] - b->pos[1]};
    const float d = dvec[0]*dvec[0] + dvec[1]*dvec[1];
    const float ave_att_dist = a->size*a->size + b->size*b->size;

    if(d <= ave_att_dist)
    {
        const float temp = 1.0 / sqrtf(d);
        dvec[0] *= temp;
        dvec[1] *= temp;
        const float ta = a->size - 1;
        const float tb = b->size - 1;
        const float ave_rep_dist = ta*ta + tb*tb;

        const element *ae = element_get(a);
        const element *be = element_get(b);

        if(ae->flags & E_CLUMP)
        {
            const float temp2 = 1.0 - (d/ave_rep_dist);
            a->vel[0] += dvec[0] * temp2;
            a->vel[1] += dvec[1] * temp2;
        }
        else
        {
            if(d < ave_rep_dist)
            {
                const float temp2 = (1.0 - d/ave_rep_dist) * 0.5;
                a->vel[0] += dvec[0] * temp2;
                a->vel[1] += dvec[1] * temp2;
            }
        }

        if(ae->reaction[b->type] > -1)
        {
            if(!(rand() % ae->reaction_chance[b->type]))
            {
                a->next_type = ae->reaction[b->type];
            }
        }

        if(ae->flags & E_SHARE)
        {
            if(be->flags & E_EMIT && a->type != b->type) a->energy += be->emit;
            if(be->flags & E_SHARE)
            {
                a->net_energy +=  b->energy;
                a->net_energy_count++;
            }
        }
    }
}
