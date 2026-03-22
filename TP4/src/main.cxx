#include <iostream>
#include <vector>
#include <cmath>
// Pour enregistrer les frames de la simulation
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "../include/univers.hxx"


void sauvegarde_frame(std::ofstream& file, const univers& u, int frame_id) {
    for (particule* p : u.getParticules()) {
        const vecteur& pos = p->getPosition();
        file << frame_id << " "
             << pos[0] << " "
             << pos[1] << " "
             << pos[2] << " "
             << p->getType() << "\n";
    }
}

int main(){
    // Caractéristiques de l'univers
    double sigma = 1.0;
    double epsilon = 5.0;
    double mass = 1.0;
    double dt = 0.00005;
    double rcut = 2.5 * sigma;
    int dim = 2;
    std::vector<double> Lds = {250.0, 120.0}; // L1 et L2
    double dist_entre_particules = pow(2, 1/6.0)/sigma; // distance entre les particules, donc chaque particule occupe
                                                        // une sphère(ici un cercle) de rayon dist_entre_particules/2, pour que les particules
    std::vector<particule*> particules;


    // Simulation duration and number of frames
    double duration = 19.5; // durée de la simulation en secondes
    int num_frames = duration / dt;


    // Vitesses des coprs
    // !!!!! Attention l'objet vecteur et conçu pour être un vecteur 3D, donc même pour un univers 2D ou 1D, il faut lui donner une composante z (même si elle ne sera pas utilisée) 
    // Cei étant dit notre univers est équipé de projection dimensionnelle, donc on peut lui donner des composantes dans les 3 dimensions et il s'occupera de les projeter dans les bonnes dimensions selon le cas
    vecteur v2;
    vecteur v1(0.0, -10, 0.0); // chute => vecteur vitesse négatif dans la direction y

    // Nombre de particules
    int N1 = 40*40; // nombre de particule du carré
    int N2 = 160*40; // nombre de particule du rectangle

    // Création de l'univers vide (de la matière)
    univers uni(particules, Lds, rcut, dim, epsilon, sigma);
    // Création des particules
    int id = 0;
    double largeur_carre = 39 * dist_entre_particules; //39 intervalles entre les 40 particules du carré, donc 39*dist_entre_particules
    double largeur_rect = 159 * dist_entre_particules; // idem

    for (int i = 0; i < N1; ++i) {
        // simplification : on place les particules sur une grille régulière au lieu d'un cercle
        double x = 20.0 + (largeur_rect - largeur_carre) / 2.0 + (i % 40) * dist_entre_particules + dist_entre_particules / 2.0; // centre du carrée juste au dessus du celui du rectangle
        double y = 55.0 + (i / 40) * dist_entre_particules + dist_entre_particules / 2.0;
        vecteur pos = vecteur(x, y, 0.0);
        uni.ajoute_particule(new particule(id++, 1, mass, pos, v1));
    }

    
    for (int i = 0; i < N2; ++i) {
        double x = 20.0 + (i % 160) * dist_entre_particules + dist_entre_particules / 2.0;
        double y = 2.5 + (i / 160) * dist_entre_particules + dist_entre_particules / 2.0;
        vecteur pos = vecteur(x, y, 0.0);
        uni.ajoute_particule(new particule(id++, 2, mass, pos, v2));
    }

    // Création du fichier pour enregistrer les frames de la simulation
    std::ofstream file("frames/frames.txt");


    int frame_id = 0;
    // Boucle de simulation
    for (int frame = 0; frame < num_frames; ++frame) {
        uni.calcule_forces();
        uni.evolue_particules(dt);

        if (frame % 1000 == 0) {
            std::cout << "Frame " << frame << "/" << num_frames << "\n";
        }
        if (frame % 100 == 0) {
            sauvegarde_frame(file, uni, frame_id);
            frame_id++;
        }
        //sauvegarde_frame(file, uni, frame_id);
        frame_id++;
    }

    file.close();

    // Exécuter le script Python pour tracer les résultats
    int code = system("python3 src/plot_collision.py");
    if (code != 0) {
        std::cerr << "Erreur lors de l'execution du script Python.\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
    
}