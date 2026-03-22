#include "../include/vecteur.hxx"
#include <cmath>
#include <iostream>

// Constructeur vide
vecteur::vecteur(){ // Classe :: méthode
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

vecteur::vecteur(double x_val, double y_val, double z_val){
    x = x_val;
    y = y_val;
    z = z_val;
}

double vecteur::norme() const{ // type de retour, classe :: méthode
    return std::sqrt(x*x + y*y + z*z);
}


vecteur vecteur:: operator+(const vecteur& other) const{
    return vecteur(x + other.x, y + other.y, z + other.z);
}

vecteur vecteur::operator-(const vecteur& other) const {
    return vecteur(x - other.x, y - other.y, z - other.z);
}

vecteur vecteur:: operator*(double scalar) const{
    return vecteur(x * scalar, y * scalar, z * scalar);
}

vecteur vecteur:: operator/(double scalar) const{
    return vecteur(x / scalar, y / scalar, z / scalar);
}  

const double& vecteur::operator[](int index) const {
    if (index == 0) return x;
    else if (index == 1) return y;
    else if (index == 2) return z;
    else throw std::out_of_range("Index out of range for vecteur");
}

double& vecteur::operator[](int index) {
    if (index == 0) return x; // les getters retournent des copies, mais nous on veut les modifié par suite d"ou cet operator
    else if (index == 1) return y;
    else if (index == 2) return z;
    else throw std::out_of_range("Index out of range for vecteur");
}



std::ostream& operator<<(std::ostream& os, const vecteur& v){
    os << "Vecteur(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")";
    return os;
}
