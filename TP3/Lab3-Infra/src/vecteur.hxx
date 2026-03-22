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

    // Getters for x, y, z if needed
    double getX() const;
    double getY() const;
    double getZ() const;

};

std::ostream& operator<<(std::ostream& os, const vecteur& v);

#endif // VECTEUR_HXX