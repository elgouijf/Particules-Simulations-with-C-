/**
 * @file simu_tp1.cxx
 * @brief Simulation gravitationnelle du système solaire par algorithme de Störmer-Verlet.
 *
 * Ce programme :
 * - effectue un benchmark d'insertion de particules dans std::list, std::set et std::vector,
 * - initialise le système solaire (Soleil, Terre, Jupiter, Halley),
 * - simule les trajectoires via un potentiel gravitationnel,
 * - sauvegarde les positions dans un fichier texte pour visualisation Python.
 */

#include <string>
#include <iostream>
#include <set>
#include <list>
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <math.h>
#include <fstream>
#include <filesystem>

#include "particule.hxx"
#include "vecteur.hxx"
#include "exceptions.hxx"
#include "output_paths.hxx"

using std::string;
using std::list;
using std::set;
using std::vector;
using std::deque;

/**
 * @brief Comparateur pour ordonner les particules dans un std::set par identifiant croissant.
 */
struct ComparateurParticule {
  /**
   * @brief Opérateur de comparaison entre deux particules.
   *
   * @param[in] p1 Première particule.
   * @param[in] p2 Deuxième particule.
   * @return true si l'identifiant de p1 est strictement inférieur à celui de p2.
   */
  bool operator()(const particule& p1, const particule& p2) const {
    return p1.getId() < p2.getId();
  }
};

/**
 * @brief Calcule les forces gravitationnelles exercées sur chaque particule du système.
 *
 * La force appliquée sur la particule i est :
 * F_i = sum_{j != i} m_i * m_j * r_ij / |r_ij|^3
 *
 * @param[out] F                Matrice des forces résultantes. F[i] = {Fx, Fy} pour la particule i.
 * @param[in]  liste_particules Vecteur des particules du système.
 * @throws ErreurNumerique Si une distance est nulle, infinie, ou si une force devient non finie.
 */
void calcul_force_F(vector<vector<double>>&,vector<particule>&);

/**
 * @brief Intègre les équations du mouvement par l'algorithme de Störmer-Verlet.
 *
 * À chaque pas de temps, positions et vitesses sont mises à jour.
 * Les positions et types sont écrits dans le fichier de sortie à chaque itération.
 *
 * @param[in,out] liste_particules Vecteur des particules du système.
 * @param[in]     delta_t          Pas de temps de l'intégration (strictement positif).
 * @param[in]     t_end            Temps final de la simulation (strictement positif).
 * @param[in,out] file             Flux de sortie pour l'enregistrement des trajectoires.
 * @throws ParametreInvalide Si delta_t ou t_end sont négatifs ou nuls, ou si une masse est invalide.
 * @throws ErreurNumerique   Si une position, vitesse ou énergie devient non finie.
 */
void algo_Stromer(vector<particule>& liste_particules,double delta_t,double t_end,std::ofstream& file);

/**
 * @brief Fonction principale de la simulation gravitationnelle.
 *
 * Étapes :
 * - benchmark comparatif d'insertion dans std::list, std::set et std::vector,
 * - initialisation du système solaire (Soleil, Terre, Jupiter, Halley),
 * - intégration numérique via algo_Stromer,
 * - écriture des trajectoires dans systeme_solaire.txt.
 *
 * @return 0 en cas de succès.
 * @throws ParametreInvalide Si les paramètres de simulation sont invalides.
 * @throws ErreurFichier     Si le fichier de sortie ne peut pas être ouvert.
 * @throws ErreurNumerique   Si un calcul numérique diverge.
 */
int main_simulation() {
  std::random_device rd;
  list<particule> liste_particules;
  set<particule, ComparateurParticule> set_particules;
  vector<particule> vector_particules;

  int taille_list = 400000;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  std::uniform_real_distribution<double> vitesse(5.0, 20.0);

  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < taille_list; ++i){
    double x = dist(mt);
    double y = dist(mt);
    double fx = dist(mt);
    double fy = dist(mt);
    double masse = dist(mt);
    double vx = vitesse(mt);
    double vy = vitesse(mt);
    int id = i;

    particule p(id,0,masse,vecteur(x,y,0.0),vecteur(vx,vy,0.0));
    p.setForce(vecteur(fx,fy,0.0));

    liste_particules.push_back(p);
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "elapsed time for list: " << elapsed_seconds.count() << "s\n";

  auto start_2 = std::chrono::steady_clock::now();
  for (int i = 0; i < taille_list; ++i){
    double x = dist(mt);
    double y = dist(mt);
    double fx = dist(mt);
    double fy = dist(mt);
    double masse = dist(mt);
    double vx = vitesse(mt);
    double vy = vitesse(mt);
    int id = i;

    particule p(id,0,masse,vecteur(x,y,0.0),vecteur(vx,vy,0.0));
    p.setForce(vecteur(fx,fy,0.0));

    set_particules.insert(p);
  }
  auto end_2 = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds_2 = end_2-start_2;
  std::cout << "elapsed time for set: " << elapsed_seconds_2.count() << "s\n";

  auto start_3 = std::chrono::steady_clock::now();
  for (int i = 0; i < taille_list; ++i){
    double x = dist(mt);
    double y = dist(mt);
    double fx = dist(mt);
    double fy = dist(mt);
    double masse = dist(mt);
    double vx = vitesse(mt);
    double vy = vitesse(mt);
    int id = i;

    particule p(id,0,masse,vecteur(x,y,0.0),vecteur(vx,vy,0.0));
    p.setForce(vecteur(fx,fy,0.0));

    vector_particules.push_back(p);
  }
  auto end_3 = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds_3 = end_3-start_3;
  std::cout << "elapsed time for vector: " << elapsed_seconds_3.count() << "s\n";

  vector<particule> systeme_solaire;

  particule soleil(0,0,1,vecteur(0,0,0),vecteur(0,0,0));
  particule terre(1,1,3.0e-6,vecteur(0,1,0),vecteur(-1,0,0));
  particule jupiter(2,2,9.55e-4,vecteur(0,5.36,0),vecteur(-0.425,0,0));
  particule haley(3,3,1e-14,vecteur(34.75,0,0),vecteur(0,0.0296,0));

  systeme_solaire.push_back(soleil);
  systeme_solaire.push_back(terre);
  systeme_solaire.push_back(jupiter);
  systeme_solaire.push_back(haley);

  double delta_t = 0.015;
  double t_end = 468.5;

  if (delta_t <= 0.0) {
    throw ParametreInvalide("delta_t doit etre positif");
  }

  if (t_end <= 0.0) {
    throw ParametreInvalide("t_end doit etre positif");
  }

  std::filesystem::path fichier_txt = std::filesystem::path(PROJECT_SOURCE_DIR) / "systeme_solaire.txt";
  std::ofstream file(fichier_txt);

  if (!file.is_open()) {
    throw ErreurFichier("Impossible d'ouvrir " + fichier_txt.string());
  }

  algo_Stromer(systeme_solaire,delta_t,t_end,file);

  file.close();

  std::filesystem::path script_python =
      std::filesystem::path(PROJECT_SOURCE_DIR) / "src/python_plot/script_visual.py";

  std::cout << "\nLe script python de visualisation attend le fichier .txt en argument.\n";
  std::cout << "Script python : src/python_plot/script_visual.py\n";
  std::cout << "Fichier txt genere : "
          << std::filesystem::absolute(fichier_txt)
          << "\n";
  return 0;
}

void calcul_force_F(vector<vector<double>>& F,vector<particule>& liste_particules){
  int len_liste = liste_particules.size();
  F.clear();

  for (int i = 0; i < len_liste; i++){
    double F_i_x = 0;
    double F_i_y = 0;
    vector<double> F_i;
    particule p_i = liste_particules[i];
    double x_i = p_i.getPosition().getX();
    double y_i = p_i.getPosition().getY();

    for (int j = 0; j < len_liste; j++){
      if (j == i){
        continue;
      }

      particule p_j = liste_particules[j];
      double x_j = p_j.getPosition().getX();
      double y_j = p_j.getPosition().getY();
      double r_i_j_x = x_j - x_i;
      double r_i_j_y = y_j - y_i;

      double norme_r_i_j = sqrt((p_i.getPosition().getX() - p_j.getPosition().getX())*(p_i.getPosition().getX() - p_j.getPosition().getX())
                              + (p_i.getPosition().getY() - p_j.getPosition().getY())*(p_i.getPosition().getY() - p_j.getPosition().getY()));

      if (!std::isfinite(norme_r_i_j)) {
        throw ErreurNumerique("Distance non finie dans calcul_force_F");
      }

      if (norme_r_i_j <= 1e-12) {
        throw ErreurNumerique("Distance trop petite ou nulle dans calcul_force_F");
      }

      F_i_x += p_i.getMasse()*p_j.getMasse()*r_i_j_x/(norme_r_i_j*norme_r_i_j*norme_r_i_j);
      F_i_y += p_i.getMasse()*p_j.getMasse()*r_i_j_y/(norme_r_i_j*norme_r_i_j*norme_r_i_j);

      if (!std::isfinite(F_i_x) || !std::isfinite(F_i_y)) {
        throw ErreurNumerique("Force non finie dans calcul_force_F");
      }
    }

    F_i.push_back(F_i_x);
    F_i.push_back(F_i_y);
    F.push_back(F_i);
  }
}

void algo_Stromer(vector<particule>& liste_particules,double delta_t,double t_end,std::ofstream& file){
  if (delta_t <= 0.0) {
    throw ParametreInvalide("delta_t doit etre positif");
  }

  if (t_end <= 0.0) {
    throw ParametreInvalide("t_end doit etre positif");
  }

  double t = 0;
  vector<vector<double>> F_old;
  vector<vector<double>> F;
  vector<double> Ec;
  vector<double> Ep;
  int len_liste = liste_particules.size();

  calcul_force_F(F,liste_particules);

  while (t < t_end){
    t += delta_t;

    for (int i = 0; i < len_liste; i++ ){
      particule& p = liste_particules[i];
      vector<double> F_i = F[i];

      if (p.getMasse() <= 0.0) {
        throw ParametreInvalide("Masse nulle ou negative dans algo_Stromer");
      }

      double x = delta_t*(p.getVitesse().getX()+0.5/p.getMasse()*F_i[0]*delta_t) + p.getPosition().getX();
      double y = delta_t*(p.getVitesse().getY()+0.5/p.getMasse()*F_i[1]*delta_t) + p.getPosition().getY();

      if (!std::isfinite(x) || !std::isfinite(y)) {
        throw ErreurNumerique("Position non finie dans algo_Stromer");
      }

      liste_particules[i] = particule(p.getId(),p.getType(),p.getMasse(),
                                      vecteur(x,y,0.0),
                                      p.getVitesse());

      liste_particules[i].setForce(p.getForce());

      F_old.push_back(F_i);
    }

    calcul_force_F(F,liste_particules);

    for (int i = 0; i < len_liste; i++ ){
      particule& p = liste_particules[i];
      vector<double> F_i = F[i];
      vector<double> F_old_i = F_old[i];

      if (p.getMasse() <= 0.0) {
        throw ParametreInvalide("Masse nulle ou negative dans mise a jour vitesse");
      }

      double vx = delta_t*0.5/p.getMasse()*(F_i[0] + F_old_i[0]) + p.getVitesse().getX();
      double vy = delta_t*0.5/p.getMasse()*(F_i[1] + F_old_i[1]) + p.getVitesse().getY();

      if (!std::isfinite(vx) || !std::isfinite(vy)) {
        throw ErreurNumerique("Vitesse non finie dans algo_Stromer");
      }

      liste_particules[i] = particule(p.getId(),p.getType(),p.getMasse(),
                                      p.getPosition(),
                                      vecteur(vx,vy,0.0));

      liste_particules[i].setForce(p.getForce());
    }

    for (int i = 0; i < len_liste; i++){
      particule p_i = liste_particules[i];
      double Ep_p = 0;
      double m_i = p_i.getMasse();

      for (int j = i+1; j < len_liste; j++){
        particule p_j = liste_particules[j];
        double m_j = p_j.getMasse();

        double norme_r_i_j = sqrt((p_i.getPosition().getX() - p_j.getPosition().getX())*(p_i.getPosition().getX() - p_j.getPosition().getX())
                                + (p_i.getPosition().getY() - p_j.getPosition().getY())*(p_i.getPosition().getY() - p_j.getPosition().getY()));

        if (!std::isfinite(norme_r_i_j)) {
          throw ErreurNumerique("Distance non finie dans energie potentielle");
        }

        if (norme_r_i_j <= 1e-12) {
          throw ErreurNumerique("Distance trop petite dans energie potentielle");
        }

        Ep_p += - m_i*m_j/norme_r_i_j;
      }

      double vx = p_i.getVitesse().getX();
      double vy = p_i.getVitesse().getY();
      double v = sqrt(vx*vx + vy*vy);
      double Ec_p = 0.5*p_i.getMasse()*v*v;

      if (!std::isfinite(Ec_p) || !std::isfinite(Ep_p)) {
        throw ErreurNumerique("Energie non finie dans algo_Stromer");
      }

      Ec.push_back(Ec_p);
      Ep.push_back(Ep_p);
    }

    file << "t = " << t << "\n";
    for (int i = 0; i < len_liste; i++){
      particule p_i = liste_particules[i];
      file << "type = " << p_i.getType() << "\n";
      file << "x = " << p_i.getPosition().getX() << "\n";
      file << "y = " << p_i.getPosition().getY() << "\n";
    }

    F_old.clear();
  }
}

/**
 * @brief Point d'entrée du programme.
 *
 * Appelle main_simulation() et intercepte toutes les exceptions métier
 * ainsi que les exceptions standard inattendues.
 *
 * @return 0 en cas de succès, EXIT_FAILURE en cas d'erreur.
 */
int main() {
  try {
    return main_simulation();
  }
  catch (const ParametreInvalide& e) {
    std::cerr << "Erreur de parametre : " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch (const ErreurNumerique& e) {
    std::cerr << "Erreur numerique : " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch (const ErreurFichier& e) {
    std::cerr << "Erreur fichier : " << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch (const std::exception& e) {
    std::cerr << "Erreur inattendue : " << e.what() << "\n";
    return EXIT_FAILURE;
  }
}