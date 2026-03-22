#ifndef UNIVERS_HXX
#define UNIVERS_HXX

#include "particule.hxx"
#include <vector>
#include <iostream> // pour std::ostream

class univers {
    public:
        std::vector<particule*> particules;
        int num_particules;
        univers();
        univers(std::vector<particule*>& v);
        void ajoute_particule(particule* p);
        void evolue_particules(double dt);
        void calcule_forces(double G, double eps=1e-9);
};

std::ostream& operator<<(std::ostream& os, const univers& u);
#endif // PARTICULE_HXX