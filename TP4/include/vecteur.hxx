#ifndef VECTEUR_HXX
#define VECTEUR_HXX
#include <ostream>

class vecteur {
private:

   // 3 dimensional vector
    double x;
    double y;
    double z;

public:
    // Default Constructor
    vecteur();
    // Parameterized Constructor
    vecteur(double x_val, double y_val, double z_val);

    // Get the norm, const method since it doesn't modify the object
    double norme() const;
    

    vecteur operator+(const vecteur& other) const; // use reference for faster computation, and const as to not modify the input
    vecteur operator-(const vecteur& other) const;
    vecteur operator*(double scalar) const;
    vecteur operator/(double scalar) const;

    // Les méthodes ci dessous sont trops appellées, donc je les mets direct dans le .hxx comme inline pour éviter les appels de fonctions et les créations de vecteurs temporaires
    inline double getX() const { return x; }
    inline double getY() const { return y; }
    inline double getZ() const { return z; }

    inline void setX(double x_val) { x = x_val; }
    inline void setY(double y_val) { y = y_val; }
    inline void setZ(double z_val) { z = z_val; }

    inline void ajoute(double dx, double dy, double dz) {
        x += dx;
        y += dy;
        z += dz;
    }

    inline double norme2() const {
        return x*x + y*y + z*z;
    }

    double& operator[](int index);
    const double& operator[](int index) const;

};

std::ostream& operator<<(std::ostream& os, const vecteur& v);

#endif // VECTEUR_HXX