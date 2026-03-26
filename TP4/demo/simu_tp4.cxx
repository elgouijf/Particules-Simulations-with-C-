/**
 * @file test_simulation_collision_2d.cxx
 * @brief Simulation 2D d'une collision entre un carré et un rectangle de particules.
 *
 * Ce programme :
 * - initialise deux ensembles de particules en 2D,
 * - simule leur interaction via un potentiel de Lennard-Jones,
 * - sauvegarde les résultats soit en format texte, soit en format VTK,
 * - permet une visualisation avec Python ou ParaView.
 *
 * Modes disponibles :
 * - 't' : export texte + visualisation Python
 * - 'v' : export VTK + visualisation ParaView
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cstdlib>

#include "../include/univers.hxx"

#ifndef PROJECT_SOURCE_DIR
#define PROJECT_SOURCE_DIR "."
#endif

/**
 * @brief Sauvegarde une frame dans un fichier texte.
 *
 * Chaque ligne contient :
 * frame_id x y z type
 *
 * @param file Flux de sortie.
 * @param u Univers contenant les particules.
 * @param frame_id Identifiant de la frame.
 */
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

/**
 * @brief Sauvegarde une frame au format VTK.
 *
 * Le format VTK permet une visualisation dans ParaView.
 *
 * @param u Univers contenant les particules.
 * @param frame_id Identifiant de la frame.
 * @param dossier Dossier de sortie.
 */
void sauvegarde_frame_vtk(const univers& u, int frame_id, const std::string& dossier = "vtk_frames") {
    std::ostringstream nom;
    nom << dossier << "/frame_"
        << std::setw(6) << std::setfill('0') << frame_id
        << ".vtk";

    std::ofstream file(nom.str());
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir " << nom.str() << "\n";
        return;
    }

    const std::vector<particule*>& particules = u.getParticules();
    int N = particules.size();

    file << "# vtk DataFile Version 3.0\n";
    file << "Frame " << frame_id << "\n";
    file << "ASCII\n";
    file << "DATASET POLYDATA\n";

    file << "POINTS " << N << " float\n";
    for (particule* p : particules) {
        const vecteur& pos = p->getPosition();
        file << pos[0] << " " << pos[1] << " " << pos[2] << "\n";
    }

    file << "\nVERTICES " << N << " " << 2 * N << "\n";
    for (int i = 0; i < N; ++i) {
        file << "1 " << i << "\n";
    }

    file << "\nPOINT_DATA " << N << "\n";

    file << "SCALARS type int 1\n";
    file << "LOOKUP_TABLE default\n";
    for (particule* p : particules) {
        file << p->getType() << "\n";
    }
}

/**
 * @brief Génère un fichier .vtk.series pour ParaView.
 *
 * Ce fichier décrit la succession temporelle des fichiers VTK
 * afin de permettre une lecture comme animation.
 *
 * @param nb_frames Nombre total de frames sauvegardées.
 * @param dt Pas de temps de la simulation.
 * @param save_every Intervalle de sauvegarde.
 * @param dossier Dossier contenant les fichiers VTK.
 */
void ecrire_fichier_series_json(int nb_frames, double dt, int save_every, const std::string& dossier = "vtk_frames") {
    std::ofstream file(dossier + "/animation.vtk.series");
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir " << dossier << "/animation.vtk.series\n";
        return;
    }

    file << "{\n";
    file << "  \"file-series-version\" : \"1.0\",\n";
    file << "  \"files\" : [\n";

    for (int i = 0; i < nb_frames; ++i) {
        std::ostringstream nom;
        nom << "frame_" << std::setw(6) << std::setfill('0') << i << ".vtk";

        double temps = i * save_every * dt;

        file << "    { \"name\" : \"" << nom.str() << "\", \"time\" : " << temps << " }";
        if (i != nb_frames - 1) file << ",";
        file << "\n";
    }

    file << "  ]\n";
    file << "}\n";
}

/**
 * @brief Recherche le script Python de visualisation 2D.
 *
 * Plusieurs chemins possibles sont testés pour permettre
 * une exécution depuis différents répertoires.
 *
 * @return Chemin vers le script si trouvé, chaîne vide sinon.
 */
std::string trouver_script_python() {
    std::vector<std::string> candidats = {
        std::string(PROJECT_SOURCE_DIR) + "/src/python_plot/plot_collision.py",
        "src/python_plot/plot_collision.py",
        "../src/python_plot/plot_collision.py"
    };

    for (const auto& chemin : candidats) {
        if (std::filesystem::exists(chemin)) {
            return chemin;
        }
    }

    return "";
}

/**
 * @brief Programme principal de simulation 2D.
 *
 * Étapes :
 * - choix du mode de sortie,
 * - initialisation des particules,
 * - simulation temporelle avec Störmer-Verlet,
 * - sauvegarde des frames,
 * - visualisation avec Python ou ParaView.
 *
 * @return EXIT_SUCCESS si la simulation s'exécute correctement,
 *         EXIT_FAILURE sinon.
 */
int main(){

    char mode;
    std::cout << "Choisir le mode : (t = txt, v = vtk) : ";
    std::cin >> mode;


    // Caractéristiques de l'univers
    double sigma = 1.0;
    double epsilon = 5.0;
    double mass = 1.0;
    double dt = 0.00005;
    //double dt = 0.001;
    double rcut = 2.5 * sigma;
    int dim = 2;
    std::vector<double> Lds = {250.0, 150.0};
    double dist_entre_particules = pow(2, 1/6.0)/sigma;
    std::vector<particule*> particules;

    // Durée de la simulation
    double duration = 19.5;
    //double duration = 13.0;
    int num_frames = duration / dt;

    // Vitesses initiales
    vecteur v2;
    vecteur v1(0.0, -10, 0.0);

    // Nombre de particules
    int N1 = 40*40;
    int N2 = 160*40;

    // Création de l'univers
    univers uni(particules, Lds, rcut, dim, epsilon, sigma);

    int id = 0;
    double largeur_carre = 39 * dist_entre_particules;
    double largeur_rect = 159 * dist_entre_particules;

    // Sauvegarde d'une frame toutes les save_every itérations
    int save_every = 100;

    // Création du carré supérieur
    for (int i = 0; i < N1; ++i) {
        double x = 20.0 + (largeur_rect - largeur_carre) / 2.0 + (i % 40) * dist_entre_particules + dist_entre_particules / 2.0;
        double y = 73.0 + (i / 40) * dist_entre_particules + dist_entre_particules / 2.0;
        vecteur pos = vecteur(x, y, 0.0);
        uni.ajoute_particule(new particule(id++, 1, mass, pos, v1));
    }

    // Création du rectangle inférieur
    for (int i = 0; i < N2; ++i) {
        double x = 20.0 + (i % 160) * dist_entre_particules + dist_entre_particules / 2.0;
        double y = 20.5 + (i / 160) * dist_entre_particules + dist_entre_particules / 2.0;
        vecteur pos = vecteur(x, y, 0.0);
        uni.ajoute_particule(new particule(id++, 2, mass, pos, v2));
    }

    std::ofstream file;
    if (mode == 't') {
        std::filesystem::create_directories("frames");
        file.open("frames/frames.txt");
        if (!file.is_open()) {
            std::cerr << "Impossible d'ouvrir frames/frames.txt\n";
            return EXIT_FAILURE;
        }
    }

    if (mode == 'v') {
        std::filesystem::create_directories("vtk_frames");
    }

    int frame_id = 0;
    auto start = std::chrono::high_resolution_clock::now();

    // Initialisation des forces au temps initial
    uni.calcule_forces();

    // Boucle de simulation
    for (int frame = 0; frame < num_frames; ++frame) {
        uni.evolue_particules(dt);

        if (frame % 1000 == 0) {
            std::cout << "Frame " << frame << "/" << num_frames << "\n";
        }

        if (frame % save_every == 0) {
            if (mode == 't') {
                sauvegarde_frame(file, uni, frame_id);
            } else {
                sauvegarde_frame_vtk(uni, frame_id);
            }
            frame_id++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Temps de simulation : " << elapsed.count() << " secondes\n";

    if (mode == 't') {
        file.close();

        std::string script_python = trouver_script_python();
        if (script_python.empty()) {
            std::cerr << "Impossible de trouver src/plot_collision.py\n";
            return EXIT_FAILURE;
        }

        std::string commande_python = "python3 " + script_python;
        int code_python = system(commande_python.c_str());
        if (code_python != 0) {
            std::cerr << "Erreur lors de l'execution du script Python.\n";
            return EXIT_FAILURE;
        }
    }

    if (mode == 'v') {
        ecrire_fichier_series_json(frame_id, dt, save_every, "vtk_frames");
        std::cout << "Fichier de series genere : vtk_frames/animation.vtk.series\n";
    }



    return EXIT_SUCCESS;
}