#include "../src/univers.hxx"
#include <iostream>
#include <chrono>

int main() {

    for (int k = 3; k <= 7; ++k) {
        int n = 1 << k; // 2^k
        univers u;
        vecteur vitesse(0.0, 0.0, 0.0);

        // remplir l'univers
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                for (int l = 0; l < n; ++l) {
                    vecteur pos(i / double(n-1), j / double(n-1), l / double(n-1));
                    particule* p = new particule(i*n*n + j*n + l, 0, 1.0, pos, vitesse);
                    u.ajoute_particule(p);
                }
            }
        }

        auto start = std::chrono::high_resolution_clock::now();
        u.calcule_forces(1.0); // calcul des interactions
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();

        std::cout << "2^" << k << " -> " << n*n*n << " particules : " << elapsed << " s" << std::endl;
    }

    return 0;
}