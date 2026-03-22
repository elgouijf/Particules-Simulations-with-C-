#include "particule.hxx"

#include <iostream>


particule:: particule(){
    id = 0;
    type = 0;
    masse = 0.0;
    position = vecteur();
    vitesse = vecteur();
    force = vecteur();
}

particule:: particule(int id,int type,double m,vecteur p,vecteur v){
    this->id = id;
    this->type = type;
    this->masse = m;
    this->position = p;
    this->vitesse = v;
    this->force = vecteur();
}


const vecteur& particule::getPosition() const { return position; }
const vecteur& particule::getVitesse() const { return vitesse; }
const vecteur& particule::getForce() const { return force; }
double particule::getMasse() const { return masse; }        

void particule::setForce(const vecteur& f) { this->force = f; }
void particule::setPosition(const vecteur& p) { this->position = p; }
void particule::setVitesse(const vecteur& v) { this->vitesse = v; }

void particule::evolue(double dt){
    position = position + vitesse * dt;
    vitesse = vitesse + force * dt / masse;
    force = vecteur();
}

std::ostream& operator<<(std::ostream& os, const particule& p){
    os << "Particule(ID: " << p.id << ", Type: " << p.type 
       << ", Masse: " << p.masse << ", Position: " << p.position 
       << ", Vitesse: " << p.vitesse << ", Force: " << p.force << ")";
    return os;
}