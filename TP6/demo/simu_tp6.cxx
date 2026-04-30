/**
 * @file simu_tp6.cxx
 * @brief Simulation 3D d'une collision entre un cube et un pavé de particules.
 *
 * Ce programme :
 * - initialise deux ensembles de particules en 3D,
 * - simule leur interaction via un potentiel de Lennard-Jones,
 * - sauvegarde les résultats soit en format texte, soit en format VTK,
 * - permet une visualisation avec Python ou ParaView.
 *
 * Modes disponibles :
 * - 't' : export texte + visualisation Python
 * - 'v' : export VTK + visualisation ParaView
 * - 'x' : export VTU XML + visualisation ParaView
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

#include "univers.hxx"
#include "io.hxx"

using namespace std;

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
        if (std::filesystem::exists(chemin)) return chemin;
    }
    return "";
}

/**
 * @brief Convertit une string en ConditionLimite.
 *
 * @param s "r" pour Reflexive, "a" pour Absorbante, "p" pour Periodique.
 * @return La ConditionLimite correspondante, Reflexive par défaut.
 */
ConditionLimite convert_string_to_Cdl(string s) {
    if (s == "a") return ConditionLimite::Absorbante;
    if (s == "p") return ConditionLimite::Periodique;
    return ConditionLimite::Reflexive;
}

/**
 * @brief Programme principal de simulation 3D.
 *
 * Étapes :
 * - choix du mode de sortie,
 * - choix des conditions aux limites,
 * - initialisation des particules en 3D,
 * - simulation temporelle avec Störmer-Verlet,
 * - sauvegarde des frames,
 * - visualisation avec Python ou ParaView.
 *
 * @return EXIT_SUCCESS si la simulation s'exécute correctement,
 *         EXIT_FAILURE sinon.
 */
int main() {

    std::string mode;
    std::cout << "Choisir le mode : (t = txt, v = vtk legacy, x = vtu xml) : ";
    std::cin >> mode;

    if (mode != "t" && mode != "v" && mode != "x") {
        std::cerr << "Mode invalide. Choisir 't', 'v' ou 'x'.\n";
        return EXIT_FAILURE;
    }

  
    int dim = 2;

    cout << "Conditions aux limites (r/a/p) pour xmin xmax ymin ymax zmin zmax : ";
    vector<string> liste_condition;
    for (int _ = 0; _ < 2*dim; _++) {
        string condition_l;
        cin >> condition_l;
        liste_condition.push_back(condition_l);
    }

    // Caractéristiques de l'univers
    double sigma   = 1.0;
    double epsilon = 5.0;
    double mass    = 1.0;
    double dt      = 0.001;
    double rcut    = 2.5 * sigma;


    std::vector<double> Lds = {250.0, 150.0, 150.0};

    double dist_entre_particules = pow(2, 1.0/6.0) / sigma;
    std::vector<particule*> particules;


    double duration = 8.5;
    int num_frames  = duration / dt;


    vecteur v2;
    vecteur v1(0.0, 0.0, -10.0);


    int N1x = 0; 
    int N1y = 0; 
    int N1z = 0;
    
    int N2x = 10;
    int N2y = 4;
    int N2z = 2;

    // Création de l'univers
    univers uni(particules, Lds, rcut, dim, epsilon, sigma,9.81);

    uni.setConditionsLimites(
        convert_string_to_Cdl(liste_condition[0]),
        convert_string_to_Cdl(liste_condition[1]),
        convert_string_to_Cdl(liste_condition[2]),
        convert_string_to_Cdl(liste_condition[3]),
        convert_string_to_Cdl(liste_condition[4]),
        convert_string_to_Cdl(liste_condition[5])
    );

    int id = 0;
    double largeur_carre = (N1x-1) * dist_entre_particules;
    double largeur_rect  = (N2x-1) * dist_entre_particules;

    int save_every = 100;
    
    // Carré supérieur (N1x * N1y * N1z particules)
    int N1 = N1x * N1y * N1z;
    for (int i = 0; i < N1; ++i) {
        int ix = i % N1x;
        int iy = (i / N1x) % N1y;
        int iz = i / (N1x * N1y);
        double x = 20.0 + (largeur_rect - largeur_carre) / 2.0
                + ix * dist_entre_particules + dist_entre_particules / 2.0;
        double y = 20.0 + iy * dist_entre_particules + dist_entre_particules / 2.0;
        double z = 90.0 + iz * dist_entre_particules + dist_entre_particules / 2.0;
        uni.ajoute_particule(new particule(id++, 1, mass, vecteur(x, y, z), v1));
    }

    // Rectangle inférieur (N2x * N2y * N2z particules)
    int N2 = N2x * N2y * N2z;
    for (int i = 0; i < N2; ++i) {
        int ix = i % N2x;
        int iy = (i / N2x) % N2y;
        int iz = i / (N2x * N2y);
        double x = 20.0 + ix * dist_entre_particules + dist_entre_particules / 2.0;
        double y = 20.0 + iy * dist_entre_particules + dist_entre_particules / 2.0;
        double z = 20.0 + iz * dist_entre_particules + dist_entre_particules / 2.0;
        uni.ajoute_particule(new particule(id++, 2, mass, vecteur(x, y, z), v2));
    }

    std::ofstream file;
    if (mode == "t") {
        std::filesystem::create_directories("frames");
        file.open("frames/frames.txt");
        if (!file.is_open()) {
            std::cerr << "Impossible d'ouvrir frames/frames.txt\n";
            return EXIT_FAILURE;
        }
    }
    string dossier_vtk;
    string dossier_vtu;

    if (mode == "v") {
        auto now = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S"); // On se sert de ça pour éviter l'écrasement des fichiers
                                                                 // En donnnat à chaque repo le temps

        dossier_vtk = "vtk_frames";

        std::filesystem::create_directories(dossier_vtk);
    }
    if (mode == "x") {
        auto now = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S");

        dossier_vtu = "vtu_frames" ;

        std::filesystem::create_directories(dossier_vtu);
    }
    int frame_id = 0;
    auto start = std::chrono::high_resolution_clock::now();

    uni.calcule_forces();

    for (int frame = 0; frame < num_frames; ++frame) {
        uni.evolue_particules(dt);

        if (frame % 1000 == 0) {
            std::cout << "Frame " << frame << "/" << num_frames
                      << "  particules : " << uni.getNumParticules() << "\n";
            //uni.limite_vitesses();
        }

        if (frame % save_every == 0) {
            if      (mode == "t") sauvegarde_frame_txt(file, uni, frame_id);
            else if (mode == "v") sauvegarde_frame_vtk(uni, frame_id, dossier_vtk);
            else if (mode == "x") sauvegarde_frame_vtu(uni, frame_id, dossier_vtu);
            frame_id++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Temps de simulation : " << elapsed.count() << " secondes\n";

    if (mode == "t") {
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

    if (mode == "v") {
        ecrire_fichier_series_json(frame_id, dt, save_every, dossier_vtk, "vtk");
        std::cout << "Fichier de series genere : vtk_frames/animation.vtk.series\n";
    }

    if (mode == "x") {
        ecrire_fichier_series_json(frame_id, dt, save_every, dossier_vtu, "vtu");
        std::cout << "Fichier de series genere : vtu_frames/animation.vtk.series\n";
    }

    return EXIT_SUCCESS;
}