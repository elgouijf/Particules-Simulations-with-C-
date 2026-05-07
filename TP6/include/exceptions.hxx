/**
 * @file exceptions.hxx
 * @brief Définition des exceptions personnalisées du simulateur.
 *
 * Ce fichier regroupe les trois catégories d'erreurs
 * susceptibles d'être levées pendant l'exécution :
 * - paramètres physiques ou géométriques invalides,
 * - erreurs d'entrée/sortie sur les fichiers,
 * - anomalies numériques détectées en cours de simulation.
 *
 * Toutes les classes dérivent de std::exception et sont
 * interceptées dans le bloc try/catch de la fonction main.
 */

#ifndef EXCEPTIONS_HXX
#define EXCEPTIONS_HXX

#include <stdexcept>
#include <string>

/**
 * @class ParametreInvalide
 * @brief Exception levée lorsqu'un paramètre de simulation est incorrect.
 *
 * Exemples de cas déclencheurs : dimension hors de {1,2,3},
 * pas de temps négatif, epsilon ou sigma non positifs,
 * taille de domaine incohérente, mode de sortie non reconnu.
 *
 * Dérive de std::invalid_argument.
 */
class ParametreInvalide : public std::invalid_argument {
public:
    /**
     * @brief Construit l'exception avec un message descriptif.
     * @param message Description précise du paramètre invalide.
     */
    explicit ParametreInvalide(const std::string& message)
        : std::invalid_argument("Parametre invalide : " + message) {}
};

/**
 * @class ErreurFichier
 * @brief Exception levée lors d'une erreur d'entrée/sortie.
 *
 * Exemples de cas déclencheurs : impossibilité d'ouvrir
 * un fichier de frames ou d'énergie, échec de création
 * d'un répertoire de sortie.
 *
 * Dérive de std::runtime_error.
 */
class ErreurFichier : public std::runtime_error {
public:
    /**
     * @brief Construit l'exception avec un message descriptif.
     * @param message Description de l'erreur et chemin du fichier concerné.
     */
    explicit ErreurFichier(const std::string& message)
        : std::runtime_error("Erreur fichier : " + message) {}
};

/**
 * @class ErreurNumerique
 * @brief Exception levée lors d'une anomalie numérique en simulation.
 *
 * Exemples de cas déclencheurs : apparition de valeurs NaN
 * ou infinies dans les positions ou vitesses, divergence
 * de l'énergie mécanique détectée à un point de contrôle.
 *
 * Dérive de std::runtime_error.
 */
class ErreurNumerique : public std::runtime_error {
public:
    /**
     * @brief Construit l'exception avec un message descriptif.
     * @param message Description de l'anomalie numérique détectée.
     */
    explicit ErreurNumerique(const std::string& message)
        : std::runtime_error("Erreur numerique : " + message) {}
};

#endif