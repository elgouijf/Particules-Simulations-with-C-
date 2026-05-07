#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <vector>

#include "output_paths.hxx"

/**
 * @brief Calcule le potentiel de Lennard-Jones pour une distance donnée.
 *
 * @param r Distance entre deux particules.
 * @return Valeur du potentiel U(r).
 */
double U_r(double r) {
    const double epsilon = 1.0;
    const double sigma = 1.0;

    const double sr = sigma / r;
    const double sr6 = std::pow(sr, 6);

    return 4.0 * epsilon * (sr6 * sr6 - sr6);
}

/**
 * @brief Programme principal.
 *
 * Génère les données du potentiel de Lennard-Jones
 * dans un fichier texte.
 *
 * Le tracé est volontairement séparé :
 * l'utilisateur doit lancer manuellement le script Python.
 *
 * @return EXIT_SUCCESS si succès, EXIT_FAILURE sinon.
 */
int main() {
    std::filesystem::path dossier_data =
        ensure_output_dir(OutputType::Potentials);

    std::filesystem::path dossier_plots =
        ensure_output_dir(OutputType::Plots);

    std::filesystem::path fichier_data =
        dossier_data / "lj.txt";

    std::filesystem::path fichier_plot =
        dossier_plots / "potentiel_lennard_jones.png";

    std::ofstream file(fichier_data);

    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir "
                  << fichier_data << "\n";
        return EXIT_FAILURE;
    }

    for (double r = 0.8; r <= 3.0; r += 0.01) {
        file << r << " " << U_r(r) << "\n";
    }

    file.close();

    std::filesystem::path script_python =
        std::filesystem::path("src/python_plot/plot_ur.py");

    std::cout << "\n=== Potentiel de Lennard-Jones ===\n";

    std::cout << "Donnees generees :\n";
    std::cout << "  potentials/lj.txt\n\n";

    std::cout << "Pour tracer le potentiel, executer le script Python correspondant :\n";
    std::cout << "  src/python_plot/plot_ur.py\n\n";

    std::cout << "Commande conseillee depuis la racine TP6 :\n";
    std::cout << "  python3 src/python_plot/plot_ur.py potentials/lj.txt\n\n";

    std::cout << "La figure sera sauvegardee dans :\n";
    std::cout << "  plots/potentiel_lennard_jones.png\n";

    std::cout << "==================================\n";

    return EXIT_SUCCESS;
}