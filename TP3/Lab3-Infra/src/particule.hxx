#ifndef PARTICULE_HXX
#define PARTICULE_HXX
#include "vecteur.hxx"


class particule {

public:
    vecteur position;
    vecteur vitesse;
    vecteur force;

    double masse;
    int id;
    int type;
    particule();
    particule(int id,int type,double m,vecteur p,vecteur v);

    // getters
    const vecteur& getPosition() const;
    const vecteur& getVitesse() const;
    const vecteur& getForce() const;
    double getMasse() const;

    // setters
    void setForce(const vecteur& f);
    void setPosition(const vecteur& p);
    void setVitesse(const vecteur& v);

    void evolue(double dt);
    void ajouterForce(const vecteur& f) { this->force = this->force + f; }
};

std::ostream& operator<<(std::ostream& os, const particule& p);
#endif // PARTICULE_HXX
