#ifndef CELLULE_HXX
#define CELLULE_HXX
#include "particule.hxx"
#include <vector>
#include <iostream> // pour std::ostream
class cellule {
    private:
        std::vector<particule*> particules;
        int num_particules;
        //double r_cut;
        //int dim;
        //std::vector<double> Lds(dim); // tailles de domaine pour chaque dimension
        std::vector<cellule*> voisins; // cellules voisines
    public:
        cellule();
        cellule(std::vector<particule*>& v, std::vector<cellule*>& voisins);
        void ajoute_particule(particule* p);
        //void evolue_particules(double dt);
        //void calcule_forces(double G, double eps=1e-9);

        void vide();

        void ajoute_voisin(cellule* voisin);

        const std::vector<particule*>& getParticules() const; // 1 er const pour ne pas modifié le vec retpurné (mais on peu modifié les particules dedans), 2 eme pour ne pas modifié l'objet courant  
        std::vector<particule*>& getParticules(); // version non const pour pouvoir modifié les particules dans la cellule

        const std::vector<cellule*>& getVoisins() const;
        std::vector<cellule*>& getVoisins(); // version non const pour pouvoir modifié les voisins de la cellule
        
};

#endif