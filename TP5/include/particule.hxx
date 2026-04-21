#ifndef PARTICULE_HXX
#define PARTICULE_HXX

#include "vecteur.hxx"
#include <iostream>
#include <vector>

/**
 * @class particule
 * @brief Représente une particule du système simulé.
 *
 * Une particule est définie par :
 * - sa position,
 * - sa vitesse,
 * - la force qui s'exerce sur elle,
 * - sa masse,
 * - un identifiant,
 * - un type permettant de distinguer différents ensembles de particules.
 *
 * Cette classe fournit également les méthodes nécessaires à l'intégration
 * temporelle, notamment avec l'algorithme de Störmer-Verlet.
 */
class particule {
    private:
        /**
         * @brief Position de la particule.
         */
        vecteur position;

        /**
         * @brief Vitesse de la particule.
         */
        vecteur vitesse;

        /**
         * @brief Force actuelle appliquée à la particule.
         */
        vecteur force;

        /**
         * @brief Force au pas de temps précédent.
         *
         * Utilisée dans l'intégration de type Störmer-Verlet.
         */
        vecteur force_old;

        /**
         * @brief Masse de la particule.
         */
        double masse;

        /**
         * @brief Identifiant unique de la particule.
         */
        int id;

        /**
         * @brief Type de la particule.
         *
         * Permet par exemple de distinguer les particules du carré et du rectangle.
         */
        int type;

    public:
        /**
         * @brief Constructeur par défaut.
         */
        particule();

        /**
         * @brief Constructeur avec initialisation complète.
         * @param id Identifiant de la particule.
         * @param type Type de la particule.
         * @param m Masse de la particule.
         * @param p Position initiale.
         * @param v Vitesse initiale.
         */
        particule(int id, int type, double m, vecteur p, vecteur v);

        /**
         * @brief Retourne l'identifiant de la particule.
         * @return L'identifiant.
         */
        int getId() const;

        /**
         * @brief Retourne le type de la particule.
         * @return Le type.
         */
        int getType() const;

        /**
         * @brief Retourne la position de la particule.
         * @return Référence constante vers la position.
         */
        const vecteur& getPosition() const;

        /**
         * @brief Retourne la vitesse de la particule.
         * @return Référence constante vers la vitesse.
         */
        const vecteur& getVitesse() const;

        /**
         * @brief Retourne la force actuelle appliquée à la particule.
         * @return Référence constante vers la force.
         */
        const vecteur& getForce() const;

        /**
         * @brief Retourne la force au pas précédent.
         * @return Référence constante vers l'ancienne force.
         */
        const vecteur& getForceOld() const;

        /**
         * @brief Retourne la masse de la particule.
         * @return La masse.
         */
        double getMasse() const;

        /**
         * @brief Modifie la force actuelle.
         * @param f Nouvelle force.
         */
        void setForce(const vecteur& f);

        /**
         * @brief Modifie la position de la particule.
         * @param p Nouvelle position.
         */
        void setPosition(const vecteur& p);

        /**
         * @brief Modifie la vitesse de la particule.
         * @param v Nouvelle vitesse.
         */
        void setVitesse(const vecteur& v);

        /**
         * @brief Modifie la force au pas précédent.
         * @param f Nouvelle ancienne force.
         */
        void setForceOld(const vecteur& f);

        /**
         * @brief Fait évoluer la particule sur un pas de temps.
         * @param dt Pas de temps.
         *
         * Cette méthode correspond à une évolution temporelle simple.
         */
        void evolue(double dt);

        /**
         * @brief Ajoute une force à la force actuelle.
         * @param f Force à ajouter.
         */
        void ajouterForce(const vecteur& f);

        /**
         * @brief Ajoute directement des composantes de force à la force actuelle.
         * @param fx Composante selon x.
         * @param fy Composante selon y.
         * @param fz Composante selon z.
         */
        void ajouterForce(double fx, double fy, double fz);

        /**
         * @brief Met à jour la position avec l'étape position de Störmer-Verlet.
         * @param dt Pas de temps.
         */
        void avance_position_verlet(double dt);

        /**
         * @brief Met à jour la vitesse avec l'étape vitesse de Störmer-Verlet.
         * @param dt Pas de temps.
         */
        void avance_vitesse_verlet(double dt);
};

/**
 * @brief Affichage textuel d'une particule.
 * @param os Flux de sortie.
 * @param p Particule à afficher.
 * @return Le flux de sortie.
 */
std::ostream& operator<<(std::ostream& os, const particule& p);

#endif // PARTICULE_HXX