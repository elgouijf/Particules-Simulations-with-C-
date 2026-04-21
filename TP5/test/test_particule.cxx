/**
 * @file test_particule.cxx
 * @brief Tests unitaires pour la classe particule.
 *
 * Ce fichier contient un ensemble de tests unitaires basés sur GoogleTest
 * permettant de vérifier le bon fonctionnement de la classe particule.
 *
 * Les tests couvrent :
 * - les constructeurs
 * - les accesseurs et modificateurs
 * - l'évolution temporelle simple
 * - l'intégration de Störmer-Verlet
 * - l'ajout de forces
 * - la gestion du cas particulier d'une masse nulle
 * - l'affichage
 */

#include <gtest/gtest.h>
#include <sstream>
#include "particule.hxx"

/**
 * @brief Vérifie que le constructeur par défaut initialise correctement une particule nulle.
 */
TEST(ParticuleTest, ConstructeurParDefaut) {
    particule p;

    EXPECT_EQ(p.getId(), 0);
    EXPECT_EQ(p.getType(), 0);
    EXPECT_DOUBLE_EQ(p.getMasse(), 0.0);

    EXPECT_DOUBLE_EQ(p.getPosition()[0], 0.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[2], 0.0);

    EXPECT_DOUBLE_EQ(p.getVitesse()[0], 0.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[2], 0.0);

    EXPECT_DOUBLE_EQ(p.getForce()[0], 0.0);
    EXPECT_DOUBLE_EQ(p.getForce()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getForce()[2], 0.0);

    EXPECT_DOUBLE_EQ(p.getForceOld()[0], 0.0);
    EXPECT_DOUBLE_EQ(p.getForceOld()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getForceOld()[2], 0.0);
}

/**
 * @brief Vérifie que le constructeur avec paramètres initialise correctement les attributs physiques.
 */
TEST(ParticuleTest, ConstructeurAvecParametres) {
    vecteur pos(1.0, 2.0, 3.0);
    vecteur vit(4.0, 5.0, 6.0);

    particule p(7, 2, 10.0, pos, vit);

    EXPECT_EQ(p.getId(), 7);
    EXPECT_EQ(p.getType(), 2);
    EXPECT_DOUBLE_EQ(p.getMasse(), 10.0);

    EXPECT_DOUBLE_EQ(p.getPosition()[0], 1.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[1], 2.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[2], 3.0);

    EXPECT_DOUBLE_EQ(p.getVitesse()[0], 4.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[1], 5.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[2], 6.0);

    EXPECT_DOUBLE_EQ(p.getForce()[0], 0.0);
    EXPECT_DOUBLE_EQ(p.getForce()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getForce()[2], 0.0);

    EXPECT_DOUBLE_EQ(p.getForceOld()[0], 0.0);
    EXPECT_DOUBLE_EQ(p.getForceOld()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getForceOld()[2], 0.0);
}

/**
 * @brief Vérifie que les setters modifient correctement les attributs dynamiques de la particule.
 */
TEST(ParticuleTest, Setters) {
    particule p;

    p.setPosition(vecteur(1.0, 2.0, 3.0));
    p.setVitesse(vecteur(4.0, 5.0, 6.0));
    p.setForce(vecteur(7.0, 8.0, 9.0));
    p.setForceOld(vecteur(10.0, 11.0, 12.0));

    EXPECT_DOUBLE_EQ(p.getPosition()[0], 1.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[1], 2.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[2], 3.0);

    EXPECT_DOUBLE_EQ(p.getVitesse()[0], 4.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[1], 5.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[2], 6.0);

    EXPECT_DOUBLE_EQ(p.getForce()[0], 7.0);
    EXPECT_DOUBLE_EQ(p.getForce()[1], 8.0);
    EXPECT_DOUBLE_EQ(p.getForce()[2], 9.0);

    EXPECT_DOUBLE_EQ(p.getForceOld()[0], 10.0);
    EXPECT_DOUBLE_EQ(p.getForceOld()[1], 11.0);
    EXPECT_DOUBLE_EQ(p.getForceOld()[2], 12.0);
}

/**
 * @brief Vérifie l'évolution simple d'une particule sous l'effet d'une force constante.
 *
 * On vérifie que :
 * - la vitesse est d'abord mise à jour
 * - la position est ensuite mise à jour avec la nouvelle vitesse
 * - la force est remise à zéro après évolution
 */
TEST(ParticuleTest, Evolue) {
    particule p(1, 0, 2.0, vecteur(0.0, 0.0, 0.0), vecteur(1.0, 0.0, 0.0));
    p.setForce(vecteur(4.0, 0.0, 0.0));

    p.evolue(0.5);

    EXPECT_DOUBLE_EQ(p.getVitesse()[0], 2.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[2], 0.0);

    EXPECT_DOUBLE_EQ(p.getPosition()[0], 1.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[2], 0.0);

    EXPECT_DOUBLE_EQ(p.getForce()[0], 0.0);
    EXPECT_DOUBLE_EQ(p.getForce()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getForce()[2], 0.0);
}

/**
 * @brief Vérifie qu'une particule de masse nulle n'évolue pas avec la méthode simple.
 */
TEST(ParticuleTest, EvolueMasseNulleNeChangeRien) {
    particule p(1, 0, 0.0, vecteur(1.0, 2.0, 3.0), vecteur(4.0, 5.0, 6.0));
    p.setForce(vecteur(7.0, 8.0, 9.0));

    p.evolue(1.0);

    EXPECT_DOUBLE_EQ(p.getPosition()[0], 1.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[1], 2.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[2], 3.0);

    EXPECT_DOUBLE_EQ(p.getVitesse()[0], 4.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[1], 5.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[2], 6.0);

    EXPECT_DOUBLE_EQ(p.getForce()[0], 7.0);
    EXPECT_DOUBLE_EQ(p.getForce()[1], 8.0);
    EXPECT_DOUBLE_EQ(p.getForce()[2], 9.0);
}

/**
 * @brief Vérifie la mise à jour de position avec la première étape de Störmer-Verlet.
 *
 * On vérifie également que la force courante est bien recopiée dans force_old.
 */
TEST(ParticuleTest, AvancePositionVerlet) {
    particule p(1, 0, 2.0, vecteur(0.0, 0.0, 0.0), vecteur(1.0, 0.0, 0.0));
    p.setForce(vecteur(4.0, 0.0, 0.0));

    p.avance_position_verlet(0.5);

    EXPECT_DOUBLE_EQ(p.getForceOld()[0], 4.0);
    EXPECT_DOUBLE_EQ(p.getForceOld()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getForceOld()[2], 0.0);

    EXPECT_DOUBLE_EQ(p.getPosition()[0], 0.75);
    EXPECT_DOUBLE_EQ(p.getPosition()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[2], 0.0);

    EXPECT_DOUBLE_EQ(p.getVitesse()[0], 1.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[2], 0.0);
}

/**
 * @brief Vérifie la mise à jour de vitesse avec la seconde étape de Störmer-Verlet.
 */
TEST(ParticuleTest, AvanceVitesseVerlet) {
    particule p(1, 0, 2.0, vecteur(0.0, 0.0, 0.0), vecteur(1.0, 0.0, 0.0));
    p.setForceOld(vecteur(4.0, 0.0, 0.0));
    p.setForce(vecteur(2.0, 0.0, 0.0));

    p.avance_vitesse_verlet(0.5);

    EXPECT_DOUBLE_EQ(p.getVitesse()[0], 1.75);
    EXPECT_DOUBLE_EQ(p.getVitesse()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[2], 0.0);
}

/**
 * @brief Vérifie qu'une particule de masse nulle ne change pas lors de la mise à jour de position Verlet.
 */
TEST(ParticuleTest, AvancePositionVerletMasseNulleNeChangeRien) {
    particule p(1, 0, 0.0, vecteur(1.0, 2.0, 3.0), vecteur(4.0, 5.0, 6.0));
    p.setForce(vecteur(7.0, 8.0, 9.0));

    p.avance_position_verlet(1.0);

    EXPECT_DOUBLE_EQ(p.getPosition()[0], 1.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[1], 2.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[2], 3.0);

    EXPECT_DOUBLE_EQ(p.getForceOld()[0], 0.0);
    EXPECT_DOUBLE_EQ(p.getForceOld()[1], 0.0);
    EXPECT_DOUBLE_EQ(p.getForceOld()[2], 0.0);
}

/**
 * @brief Vérifie qu'une particule de masse nulle ne change pas lors de la mise à jour de vitesse Verlet.
 */
TEST(ParticuleTest, AvanceVitesseVerletMasseNulleNeChangeRien) {
    particule p(1, 0, 0.0, vecteur(1.0, 2.0, 3.0), vecteur(4.0, 5.0, 6.0));
    p.setForceOld(vecteur(1.0, 1.0, 1.0));
    p.setForce(vecteur(2.0, 2.0, 2.0));

    p.avance_vitesse_verlet(1.0);

    EXPECT_DOUBLE_EQ(p.getVitesse()[0], 4.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[1], 5.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[2], 6.0);
}

/**
 * @brief Vérifie l'ajout de force composante par composante.
 */
TEST(ParticuleTest, AjouterForceParComposantes) {
    particule p;
    p.ajouterForce(1.0, 2.0, 3.0);
    p.ajouterForce(-0.5, 1.0, 0.0);

    EXPECT_DOUBLE_EQ(p.getForce()[0], 0.5);
    EXPECT_DOUBLE_EQ(p.getForce()[1], 3.0);
    EXPECT_DOUBLE_EQ(p.getForce()[2], 3.0);
}

/**
 * @brief Vérifie l'ajout de force par vecteur.
 */
TEST(ParticuleTest, AjouterForceParVecteur) {
    particule p;
    p.ajouterForce(vecteur(1.0, 2.0, 3.0));
    p.ajouterForce(vecteur(4.0, -1.0, 0.5));

    EXPECT_DOUBLE_EQ(p.getForce()[0], 5.0);
    EXPECT_DOUBLE_EQ(p.getForce()[1], 1.0);
    EXPECT_DOUBLE_EQ(p.getForce()[2], 3.5);
}

/**
 * @brief Vérifie le format d'affichage d'une particule via l'opérateur <<.
 */
TEST(ParticuleTest, AffichageFlux) {
    particule p(1, 2, 3.0, vecteur(4.0, 5.0, 6.0), vecteur(7.0, 8.0, 9.0));
    std::ostringstream oss;

    oss << p;

    EXPECT_EQ(
        oss.str(),
        "Particule(ID: 1, Type: 2, Masse: 3, Position: Vecteur(4, 5, 6), Vitesse: Vecteur(7, 8, 9), Force: Vecteur(0, 0, 0))"
    );
}