/**
 * @file test_univers.cxx
 * @brief Tests unitaires pour la classe univers.
 *
 * Ce fichier contient un ensemble de tests unitaires basés sur GoogleTest
 * permettant de vérifier le bon fonctionnement de la classe univers.
 *
 * Les tests couvrent :
 * - les constructeurs ;
 * - l'ajout de particules ;
 * - l'adaptation des particules à la dimension de l'univers ;
 * - le placement dans les cellules ;
 * - le calcul de l'énergie cinétique ;
 * - le calcul des forces ;
 * - l'évolution temporelle globale ;
 * - l'affichage textuel de l'univers.
 */

#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include "univers.hxx"
#include "particule.hxx"
#include "vecteur.hxx"

/**
 * @brief Vérifie que le constructeur par défaut initialise correctement l'univers.
 */
TEST(UniversTest, ConstructeurParDefaut) {
    univers u;

    EXPECT_EQ(u.getNumParticules(), 0);
    EXPECT_EQ(u.getDim(), 3);
    EXPECT_DOUBLE_EQ(u.getRCut(), 2.5);

    ASSERT_EQ(u.getLds().size(), 3);
    EXPECT_DOUBLE_EQ(u.getLds()[0], 10.0);
    EXPECT_DOUBLE_EQ(u.getLds()[1], 10.0);
    EXPECT_DOUBLE_EQ(u.getLds()[2], 10.0);

    ASSERT_EQ(u.getNcd().size(), 3);
    EXPECT_EQ(u.getNcd()[0], 4);
    EXPECT_EQ(u.getNcd()[1], 4);
    EXPECT_EQ(u.getNcd()[2], 4);

    EXPECT_EQ(u.getCellules().size(), 64);
    EXPECT_TRUE(u.getParticules().empty());
}

/**
 * @brief Vérifie qu'un univers paramétré est correctement initialisé.
 */
TEST(UniversTest, ConstructeurAvecParametresValides) {
    std::vector<particule*> parts;
    std::vector<double> Lds = {6.0, 8.0};

    univers u(parts, Lds, 2.0, 2, 1.5, 0.8,0.0);

    EXPECT_EQ(u.getNumParticules(), 0);
    EXPECT_EQ(u.getDim(), 2);
    EXPECT_DOUBLE_EQ(u.getRCut(), 2.0);

    ASSERT_EQ(u.getLds().size(), 2);
    EXPECT_DOUBLE_EQ(u.getLds()[0], 6.0);
    EXPECT_DOUBLE_EQ(u.getLds()[1], 8.0);

    ASSERT_EQ(u.getNcd().size(), 2);
    EXPECT_EQ(u.getNcd()[0], 3);
    EXPECT_EQ(u.getNcd()[1], 4);

    EXPECT_EQ(u.getCellules().size(), 12);
}

/**
 * @brief Vérifie qu'une particule ajoutée est bien comptée et stockée.
 */
TEST(UniversTest, AjouteParticuleAugmenteCompteur) {
    univers u;

    auto* p = new particule(1, 0, 2.0, vecteur(1.0, 1.0, 1.0), vecteur());

    u.ajoute_particule(p);

    EXPECT_EQ(u.getNumParticules(), 1);
    ASSERT_EQ(u.getParticules().size(), 1);
    EXPECT_EQ(u.getParticules()[0], p);
}

/**
 * @brief Vérifie qu'en dimension 1 les composantes y et z sont annulées lors de l'ajout.
 */
TEST(UniversTest, AjouteParticuleProjetteEnDimension1) {
    std::vector<particule*> parts;
    univers u(parts, {10.0}, 2.5, 1, 1.0, 1.0,0.0);

    auto* p = new particule(1, 0, 1.0,
                            vecteur(3.0, 4.0, 5.0),
                            vecteur(6.0, 7.0, 8.0));
    p->setForce(vecteur(9.0, 10.0, 11.0));

    u.ajoute_particule(p);

    EXPECT_DOUBLE_EQ(p->getPosition()[0], 3.0);
    EXPECT_DOUBLE_EQ(p->getPosition()[1], 0.0);
    EXPECT_DOUBLE_EQ(p->getPosition()[2], 0.0);

    EXPECT_DOUBLE_EQ(p->getVitesse()[0], 6.0);
    EXPECT_DOUBLE_EQ(p->getVitesse()[1], 0.0);
    EXPECT_DOUBLE_EQ(p->getVitesse()[2], 0.0);

    EXPECT_DOUBLE_EQ(p->getForce()[0], 9.0);
    EXPECT_DOUBLE_EQ(p->getForce()[1], 0.0);
    EXPECT_DOUBLE_EQ(p->getForce()[2], 0.0);
}

/**
 * @brief Vérifie qu'en dimension 2 la composante z est annulée lors de l'ajout.
 */
TEST(UniversTest, AjouteParticuleProjetteEnDimension2) {
    std::vector<particule*> parts;
    univers u(parts, {10.0, 10.0}, 2.5, 2, 1.0, 1.0,0.0);

    auto* p = new particule(1, 0, 1.0,
                            vecteur(3.0, 4.0, 5.0),
                            vecteur(6.0, 7.0, 8.0));
    p->setForce(vecteur(9.0, 10.0, 11.0));

    u.ajoute_particule(p);

    EXPECT_DOUBLE_EQ(p->getPosition()[0], 3.0);
    EXPECT_DOUBLE_EQ(p->getPosition()[1], 4.0);
    EXPECT_DOUBLE_EQ(p->getPosition()[2], 0.0);

    EXPECT_DOUBLE_EQ(p->getVitesse()[0], 6.0);
    EXPECT_DOUBLE_EQ(p->getVitesse()[1], 7.0);
    EXPECT_DOUBLE_EQ(p->getVitesse()[2], 0.0);

    EXPECT_DOUBLE_EQ(p->getForce()[0], 9.0);
    EXPECT_DOUBLE_EQ(p->getForce()[1], 10.0);
    EXPECT_DOUBLE_EQ(p->getForce()[2], 0.0);
}

/**
 * @brief Vérifie qu'une particule est bien placée dans une cellule lors de son ajout.
 */
TEST(UniversTest, AjouteParticulePlaceDansUneCellule) {
    univers u;

    auto* p = new particule(1, 0, 1.0, vecteur(1.0, 1.0, 1.0), vecteur());
    u.ajoute_particule(p);

    int total_occurrences = 0;
    for (const cellule& c : u.getCellules()) {
        total_occurrences += static_cast<int>(c.getParticules().size());
    }

    EXPECT_EQ(total_occurrences, 1);
}

/**
 * @brief Vérifie le calcul de l'énergie cinétique totale.
 */
TEST(UniversTest, EnergieCinetique) {
    univers u;

    auto* p1 = new particule(1, 0, 2.0, vecteur(), vecteur(3.0, 0.0, 0.0)); // Ec = 9
    auto* p2 = new particule(2, 0, 1.0, vecteur(), vecteur(0.0, 4.0, 0.0)); // Ec = 8

    u.ajoute_particule(p1);
    u.ajoute_particule(p2);

    EXPECT_DOUBLE_EQ(u.energie_cinetique(), 17.0);
}

/**
 * @brief Vérifie que deux particules proches subissent des forces opposées après calcul.
 */
TEST(UniversTest, CalculeForcesActionReaction) {
    univers u;

    auto* p1 = new particule(1, 0, 1.0, vecteur(0.0, 0.0, 0.0), vecteur());
    auto* p2 = new particule(2, 0, 1.0, vecteur(1.0, 0.0, 0.0), vecteur());

    u.ajoute_particule(p1);
    u.ajoute_particule(p2);

    u.calcule_forces();

    EXPECT_NEAR(p1->getForce()[0], -p2->getForce()[0], 1e-9);
    EXPECT_NEAR(p1->getForce()[1], -p2->getForce()[1], 1e-9);
    EXPECT_NEAR(p1->getForce()[2], -p2->getForce()[2], 1e-9);
}

/**
 * @brief Vérifie que deux particules trop éloignées n'interagissent pas.
 */
TEST(UniversTest, CalculeForcesAuDelaDuCutoff) {
    univers u;

    auto* p1 = new particule(1, 0, 1.0, vecteur(0.0, 0.0, 0.0), vecteur());
    auto* p2 = new particule(2, 0, 1.0, vecteur(9.0, 0.0, 0.0), vecteur());

    u.ajoute_particule(p1);
    u.ajoute_particule(p2);

    u.calcule_forces();

    EXPECT_NEAR(p1->getForce()[0], 0.0, 1e-12);
    EXPECT_NEAR(p1->getForce()[1], 0.0, 1e-12);
    EXPECT_NEAR(p1->getForce()[2], 0.0, 1e-12);

    EXPECT_NEAR(p2->getForce()[0], 0.0, 1e-12);
    EXPECT_NEAR(p2->getForce()[1], 0.0, 1e-12);
    EXPECT_NEAR(p2->getForce()[2], 0.0, 1e-12);
}

/**
 * @brief Vérifie que l'évolution globale modifie bien l'état des particules.
 */
TEST(UniversTest, EvolueParticules) {
    univers u;

    auto* p1 = new particule(1, 0, 1.0, vecteur(0.0, 0.0, 0.0), vecteur(1.0, 0.0, 0.0));
    auto* p2 = new particule(2, 0, 1.0, vecteur(1.2, 0.0, 0.0), vecteur(-1.0, 0.0, 0.0));

    u.ajoute_particule(p1);
    u.ajoute_particule(p2);

    u.calcule_forces();
    double x1_avant = p1->getPosition()[0];
    double x2_avant = p2->getPosition()[0];

    u.evolue_particules(0.01);

    EXPECT_NE(p1->getPosition()[0], x1_avant);
    EXPECT_NE(p2->getPosition()[0], x2_avant);
}

/**
 * @brief Vérifie que l'affichage d'un univers contient les particules qu'il stocke.
 */
TEST(UniversTest, AffichageFlux) {
    univers u;

    auto* p = new particule(1, 2, 3.0, vecteur(4.0, 5.0, 6.0), vecteur(7.0, 8.0, 9.0));
    u.ajoute_particule(p);

    std::ostringstream oss;
    oss << u;

    EXPECT_NE(oss.str().find("Particule(ID: 1"), std::string::npos);
}

/**
 * @brief Vérifie qu'une particule sortant par xmin en condition périodique
 * réapparaît du côté xmax.
 */
TEST(UniversTest, ConditionPeriodiqueXmin) {
    std::vector<particule*> parts;
    univers u(parts, {10.0, 10.0}, 2.5, 2, 1.0, 1.0, 0.0);

    u.setConditionsLimites(
        ConditionLimite::Periodique,  // xmin
        ConditionLimite::Periodique,  // xmax
        ConditionLimite::Reflexive,   // ymin
        ConditionLimite::Reflexive,   // ymax
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive
    );

    auto* p = new particule(1, 0, 1.0,
                            vecteur(-1.0, 5.0, 0.0),
                            vecteur(-2.0, 0.0, 0.0));

    u.ajoute_particule(p);
    u.applique_conditions_limites();

    ASSERT_EQ(u.getNumParticules(), 1);
    EXPECT_DOUBLE_EQ(p->getPosition().getX(), 9.0);
    EXPECT_DOUBLE_EQ(p->getPosition().getY(), 5.0);
    EXPECT_DOUBLE_EQ(p->getVitesse().getX(), -2.0);
}
/**
 * @brief Vérifie qu'une particule sortant par xmax en condition périodique
 * réapparaît du côté xmin.
 */
TEST(UniversTest, ConditionPeriodiqueXmax) {
    std::vector<particule*> parts;
    univers u(parts, {10.0, 10.0}, 2.5, 2, 1.0, 1.0, 0.0);

    u.setConditionsLimites(
        ConditionLimite::Periodique,
        ConditionLimite::Periodique,
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive
    );

    auto* p = new particule(1, 0, 1.0,
                            vecteur(11.0, 5.0, 0.0),
                            vecteur(2.0, 0.0, 0.0));

    u.ajoute_particule(p);
    u.applique_conditions_limites();

    ASSERT_EQ(u.getNumParticules(), 1);
    EXPECT_DOUBLE_EQ(p->getPosition().getX(), 1.0);
    EXPECT_DOUBLE_EQ(p->getPosition().getY(), 5.0);
    EXPECT_DOUBLE_EQ(p->getVitesse().getX(), 2.0);
}

/**
 * @brief Vérifie qu'une particule sortant par ymin en condition périodique
 * réapparaît du côté ymax.
 */
TEST(UniversTest, ConditionPeriodiqueYmin) {
    std::vector<particule*> parts;
    univers u(parts, {10.0, 10.0}, 2.5, 2, 1.0, 1.0, 0.0);

    u.setConditionsLimites(
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive,
        ConditionLimite::Periodique,
        ConditionLimite::Periodique,
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive
    );

    auto* p = new particule(1, 0, 1.0,
                            vecteur(5.0, -2.0, 0.0),
                            vecteur(0.0, -1.0, 0.0));

    u.ajoute_particule(p);
    u.applique_conditions_limites();

    ASSERT_EQ(u.getNumParticules(), 1);
    EXPECT_DOUBLE_EQ(p->getPosition().getX(), 5.0);
    EXPECT_DOUBLE_EQ(p->getPosition().getY(), 8.0);
    EXPECT_DOUBLE_EQ(p->getVitesse().getY(), -1.0);
}

/**
 * @brief Vérifie qu'une particule sortant par ymax en condition périodique
 * réapparaît du côté ymin.
 */
TEST(UniversTest, ConditionPeriodiqueYmax) {
    std::vector<particule*> parts;
    univers u(parts, {10.0, 10.0}, 2.5, 2, 1.0, 1.0, 0.0);

    u.setConditionsLimites(
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive,
        ConditionLimite::Periodique,
        ConditionLimite::Periodique,
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive
    );

    auto* p = new particule(1, 0, 1.0,
                            vecteur(5.0, 12.0, 0.0),
                            vecteur(0.0, 1.0, 0.0));

    u.ajoute_particule(p);
    u.applique_conditions_limites();

    ASSERT_EQ(u.getNumParticules(), 1);
    EXPECT_DOUBLE_EQ(p->getPosition().getX(), 5.0);
    EXPECT_DOUBLE_EQ(p->getPosition().getY(), 2.0);
    EXPECT_DOUBLE_EQ(p->getVitesse().getY(), 1.0);
}

/**
 * @brief Vérifie qu'une particule sortant par un bord absorbant est supprimée.
 */
TEST(UniversTest, ConditionAbsorbanteSupprimeParticule) {
    std::vector<particule*> parts;
    univers u(parts, {10.0, 10.0}, 2.5, 2, 1.0, 1.0, 0.0);

    u.setConditionsLimites(
        ConditionLimite::Absorbante,  // xmin
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive
    );

    auto* p = new particule(1, 0, 1.0,
                            vecteur(-1.0, 5.0, 0.0),
                            vecteur(-1.0, 0.0, 0.0));

    u.ajoute_particule(p);

    EXPECT_EQ(u.getNumParticules(), 1);

    u.applique_conditions_limites();

    EXPECT_EQ(u.getNumParticules(), 0);
    EXPECT_TRUE(u.getParticules().empty());
}

/**
 * @brief Vérifie que seules les particules sorties par un bord absorbant
 * sont supprimées, tandis que les particules internes restent présentes.
 */
TEST(UniversTest, ConditionAbsorbanteConserveParticulesInternes) {
    std::vector<particule*> parts;
    univers u(parts, {10.0, 10.0}, 2.5, 2, 1.0, 1.0, 0.0);

    u.setConditionsLimites(
        ConditionLimite::Absorbante,
        ConditionLimite::Absorbante,
        ConditionLimite::Absorbante,
        ConditionLimite::Absorbante,
        ConditionLimite::Reflexive,
        ConditionLimite::Reflexive
    );

    auto* p_interne = new particule(1, 0, 1.0,
                                    vecteur(5.0, 5.0, 0.0),
                                    vecteur(0.0, 0.0, 0.0));

    auto* p_sortie = new particule(2, 0, 1.0,
                                   vecteur(12.0, 5.0, 0.0),
                                   vecteur(1.0, 0.0, 0.0));

    u.ajoute_particule(p_interne);
    u.ajoute_particule(p_sortie);

    EXPECT_EQ(u.getNumParticules(), 2);

    u.applique_conditions_limites();

    ASSERT_EQ(u.getNumParticules(), 1);
    ASSERT_EQ(u.getParticules().size(), 1);
    EXPECT_EQ(u.getParticules()[0]->getId(), 1);
}