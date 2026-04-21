/**
 * @file test_vecteur.cxx
 * @brief Tests unitaires pour la classe vecteur.
 *
 * Ce fichier contient un ensemble de tests unitaires basés sur GoogleTest
 * permettant de vérifier le bon fonctionnement de la classe vecteur.
 *
 * Les tests couvrent :
 * - les constructeurs
 * - les opérations arithmétiques
 * - l'accès aux composantes
 * - la gestion des erreurs
 * - l'affichage
 */

#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include "vecteur.hxx"

/**
 * @brief Vérifie que le constructeur par défaut initialise un vecteur nul.
 */
TEST(VecteurTest, ConstructeurParDefaut) {
    vecteur v;

    EXPECT_DOUBLE_EQ(v[0], 0.0);
    EXPECT_DOUBLE_EQ(v[1], 0.0);
    EXPECT_DOUBLE_EQ(v[2], 0.0);
}

/**
 * @brief Vérifie que le constructeur avec paramètres initialise correctement les composantes.
 */
TEST(VecteurTest, ConstructeurAvecComposantes) {
    vecteur v(1.5, -2.0, 4.25);

    EXPECT_DOUBLE_EQ(v[0], 1.5);
    EXPECT_DOUBLE_EQ(v[1], -2.0);
    EXPECT_DOUBLE_EQ(v[2], 4.25);
}

/**
 * @brief Vérifie le calcul correct de la norme euclidienne.
 */
TEST(VecteurTest, Norme) {
    vecteur v(3.0, 4.0, 12.0);

    EXPECT_DOUBLE_EQ(v.norme(), 13.0);
}

/**
 * @brief Vérifie l'addition de deux vecteurs.
 */
TEST(VecteurTest, Addition) {
    vecteur a(1.0, 2.0, 3.0);
    vecteur b(4.0, 5.0, 6.0);

    vecteur c = a + b;

    EXPECT_DOUBLE_EQ(c[0], 5.0);
    EXPECT_DOUBLE_EQ(c[1], 7.0);
    EXPECT_DOUBLE_EQ(c[2], 9.0);
}

/**
 * @brief Vérifie la soustraction de deux vecteurs.
 */
TEST(VecteurTest, Soustraction) {
    vecteur a(5.0, 7.0, 9.0);
    vecteur b(1.0, 2.0, 3.0);

    vecteur c = a - b;

    EXPECT_DOUBLE_EQ(c[0], 4.0);
    EXPECT_DOUBLE_EQ(c[1], 5.0);
    EXPECT_DOUBLE_EQ(c[2], 6.0);
}

/**
 * @brief Vérifie la multiplication par un scalaire.
 */
TEST(VecteurTest, MultiplicationScalaire) {
    vecteur v(1.0, -2.0, 3.5);

    vecteur r = v * 2.0;

    EXPECT_DOUBLE_EQ(r[0], 2.0);
    EXPECT_DOUBLE_EQ(r[1], -4.0);
    EXPECT_DOUBLE_EQ(r[2], 7.0);
}

/**
 * @brief Vérifie la division par un scalaire.
 */
TEST(VecteurTest, DivisionScalaire) {
    vecteur v(2.0, -4.0, 8.0);

    vecteur r = v / 2.0;

    EXPECT_DOUBLE_EQ(r[0], 1.0);
    EXPECT_DOUBLE_EQ(r[1], -2.0);
    EXPECT_DOUBLE_EQ(r[2], 4.0);
}

/**
 * @brief Vérifie l'accès en lecture aux composantes via l'opérateur [].
 */
TEST(VecteurTest, AccesLectureParIndice) {
    vecteur v(10.0, -3.5, 7.25);

    EXPECT_DOUBLE_EQ(v[0], 10.0);
    EXPECT_DOUBLE_EQ(v[1], -3.5);
    EXPECT_DOUBLE_EQ(v[2], 7.25);
}

/**
 * @brief Vérifie l'accès en écriture aux composantes via l'opérateur [].
 */
TEST(VecteurTest, AccesEcritureParIndice) {
    vecteur v;

    v[0] = 10.0;
    v[1] = -3.5;
    v[2] = 7.25;

    EXPECT_DOUBLE_EQ(v[0], 10.0);
    EXPECT_DOUBLE_EQ(v[1], -3.5);
    EXPECT_DOUBLE_EQ(v[2], 7.25);
}

/**
 * @brief Vérifie l'accès en lecture aux composantes via les méthodes get().
 */
TEST(VecteurTest, AccesLectureParGet) {
    vecteur v(10.0, -3.5, 7.25);

    EXPECT_DOUBLE_EQ(v.getX(), 10.0);
    EXPECT_DOUBLE_EQ(v.getY(), -3.5);
    EXPECT_DOUBLE_EQ(v.getZ(), 7.25);
}


/**
 * @brief Vérifie qu'un accès en lecture avec un indice invalide lance une exception.
 */
TEST(VecteurTest, AccesLectureIndiceInvalideLanceException) {
    vecteur v(1.0, 2.0, 3.0);

    EXPECT_THROW(v[3], std::out_of_range);
    EXPECT_THROW(v[-1], std::out_of_range);
}


/**
 * @brief Vérifie qu'un accès en écriture avec un indice invalide lance une exception.
 */
TEST(VecteurTest, AccesEcritureIndiceInvalideLanceException) {
    vecteur v;

    EXPECT_THROW(v[3] = 1.0, std::out_of_range);
    EXPECT_THROW(v[-1] = 2.0, std::out_of_range);
}

/**
 * @brief Vérifie le format d'affichage du vecteur via l'opérateur <<.
 */
TEST(VecteurTest, AffichageFlux) {
    vecteur v(1.0, 2.0, 3.0);
    std::ostringstream oss;

    oss << v;

    EXPECT_EQ(oss.str(), "Vecteur(1, 2, 3)");
}

