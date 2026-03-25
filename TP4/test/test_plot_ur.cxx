/**
 * @file test_simulation_collision_3d.cxx
 * @brief Simulation de collision de particules avec potentiel de Lennard-Jones.
 *
 * Ce programme :
 * - génère deux ensembles de particules (cube et pavé),
 * - simule leur interaction via un potentiel de Lennard-Jones,
 * - sauvegarde les résultats en format texte ou VTK,
 * - permet une visualisation avec Python ou ParaView.
 *
 * Modes :
 * - 't' : sortie texte + visualisation Python
 * - 'v' : sortie VTK + visualisation ParaView
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
#include <string>

#include "../include/univers.hxx"
#include "../include/io.hxx"

#ifndef PROJECT_SOURCE_DIR
#define PROJECT_SOURCE_DIR "."
#endif

/**
 * @brief Sauvegarde une frame de simulation dans un fichier texte.
 *
 * Format :
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
 * Permet une visualisation 3D dans ParaView.
 *
 * @param u Univers contenant les particules.
 * @param frame_id Identifiant de la frame.
 * @param dossier Dossier de sortie.
 */
void sauvegarde_frame_vtk(const univers& u, int frame_id, const std::string& dossier = "vtk_frames_3d") {
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
 * Permet d'interpréter une séquence de fichiers VTK comme une animation.
 *
 * @param nb_frames Nombre de frames.
 * @param dt Pas de temps.
 * @param save_every Intervalle de sauvegarde.
 * @param dossier Dossier des fichiers VTK.
 */
void ecrire_fichier_series_json(int nb_frames, double dt, int save_every, const std::string& dossier = "vtk_frames_3d") {
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
 * @brief Vérifie la présence de ParaView.
 *
 * @return true si ParaView est disponible.
 */
bool paraview_disponible() {
    return std::system("command -v paraview >/dev/null 2>&1") == 0;
}

/**
 * @brief Recherche le script Python de visualisation 3D.
 *
 * @return Chemin du script ou chaîne vide.
 */
std::string trouver_script_python_3d() {
    std::vector<std::string> candidats = {
        std::string(PROJECT_SOURCE_DIR) + "/src/python_plot/plot_collision_3d.py",
        "src/python_plot/plot_collision_3d.py",
        "../src/python_plot/plot_collision_3d.py"
    };

    for (const auto& chemin : candidats) {
        if (std::filesystem::exists(chemin)) {
            return chemin;
        }
    }

    return "";
}

/**
 * @brief Programme principal.
 *
 * Étapes :
 * - choix du mode (texte ou VTK),
 * - création des particules,
 * - simulation temporelle (Verlet),
 * - sauvegarde et visualisation.
 *
 * @return EXIT_SUCCESS si succès.
 */
int main(){
    std::ofstream file("lj.txt");
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir lj.txt\n";
        return 1;
    }

    for (double r = 0.8; r <= 3.0; r += 0.01) {
        file << r << " " << U_r(r) << "\n";
    }

    file.close();

    std::string script_python = trouver_script_python_lj();
    if (script_python.empty()) {
        std::cerr << "Impossible de trouver plot_ur.py\n";
        return 1;
    }

    std::string commande = "python3 " + script_python;
    int code = std::system(commande.c_str());

    if (code != 0) {
        std::cerr << "Erreur lors de l'execution de plot_ur.py\n";
        return 1;
    }

    return 0;
}