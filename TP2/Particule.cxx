#include <string>
#include <iostream>
#include <set>
#include <list>
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <math.h>  
using std::string;
using std:: list;
using std:: set;
using std:: vector;
using std:: deque;

class Particule{
  private : double x;
  double y;
  double vx;
  double vy;
  double fx;
  double fy;
  double masse;
  int id;
  string type;
  public : Particule(double x,double y,double vx,double vy,double fx,
            double fy,double masse,string type,int id){
       this -> x = x;
       this -> y = y;
       this -> vx = vx;
       this -> vy = vy;
       this -> fx = fx;
       this -> fy = fy;
       this -> masse = masse;
       this -> type = type;
       this -> id = id;
            }
  bool operator<(const Particule& other) const {
      return id < other.id;
  }
  double getMasse(){
    return this -> masse;
  }
  double getX(){
    return this -> x;
  }
  double getY(){
    return this -> y;
  }
  double getVx(){
    return this -> vx;
  }
  double getVy(){
    return this -> vy;
  }
  string getType(){
    return this -> type;
  }
  double getFx(){
    return this -> fx;
  }
  double getFy(){
    return this -> fy;
  }
  int getId(){
    return this -> id;
  }
};

void calcul_force_F(vector<vector<double>>&,vector<Particule>&);

void algo_Stormer(vector<Particule>& liste_particules,double delta_t,double t_end);



int main() {
  std::random_device rd;
  list<Particule> liste_particules;
  set<Particule> set_particules;
  vector<Particule> vector_particules;
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
    Particule p(x,y,vx,vy,fx,fy,masse,"default",id);
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
    Particule p(x,y,vx,vy,fx,fy,masse,"default",id);
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
      Particule p(x,y,vx,vy,fx,fy,masse,"default",id);
      vector_particules.push_back(p);
  }
  auto end_3 = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds_3 = end_3-start_3;
  std::cout << "elapsed time for vector: " << elapsed_seconds_3.count() << "s\n";
  vector<Particule> systeme_solaire;
  Particule soleil(0,0,0,0,0,0,1,"soleil",0);
  Particule terre(0,1,-1,0,0,0,3.0e-6,"terre",1);
  Particule jupiter(0,5.36,-0.425,0,0,0,9.55e-4,"jupiter",2);
  Particule haley(34.75,0,0,0.0296,0,0,1e-14,"haley",3);
  systeme_solaire.push_back(soleil);
  systeme_solaire.push_back(terre);
  systeme_solaire.push_back(jupiter);
  systeme_solaire.push_back(haley);
  double delta_t = 0.015;
  double t_end = 468.5;
  algo_Stormer(systeme_solaire,delta_t,t_end);
 }

void calcul_force_F(vector<vector<double>>& F,vector<Particule>& liste_particules){
     int len_liste = liste_particules.size();
     F.clear();
     for (int i = 0;i<len_liste;i++){
        double F_i_x = 0;
        double F_i_y = 0;
        vector<double> F_i;
        Particule p_i = liste_particules[i];
        double x_i = p_i.getX();
        double y_i = p_i.getY();
        for (int j = 0;j < len_liste;j++){
          if (j == i){
            continue;
          }
          Particule p_j = liste_particules[j];
          double x_j = p_j.getX();
          double y_j = p_j.getY();
          double r_i_j_x = x_j - x_i;
          double r_i_j_y = y_j - y_i;
          double norme_r_i_j = sqrt((p_i.getX() - p_j.getX())*(p_i.getX() - p_j.getX()) +(p_i.getY() - p_j.getY())*(p_i.getY() - p_j.getY()));
          F_i_x += p_i.getMasse()*p_j.getMasse()*r_i_j_x/(norme_r_i_j*norme_r_i_j*norme_r_i_j);
          F_i_y += p_i.getMasse()*p_j.getMasse()*r_i_j_y/(norme_r_i_j*norme_r_i_j*norme_r_i_j);
        }
        F_i.push_back(F_i_x);
        F_i.push_back(F_i_y);
        F.push_back(F_i);
     }
}


void algo_Stormer(vector<Particule>& liste_particules,double delta_t,double t_end){
      double t = 0;
      vector<vector<double>> F_old;
      vector<vector<double>> F;
      int len_liste = liste_particules.size();
      calcul_force_F(F,liste_particules);
      while (t < t_end){
        t += delta_t;
        for (int i = 0; i <len_liste; i++ ){
          Particule& p = liste_particules[i];
          vector<double> F_i = F[i];
          double x = delta_t*(p.getVx()+0.5/p.getMasse()*F_i[0]*delta_t) + p.getX();
          double y = delta_t*(p.getVy()+0.5/p.getMasse()*F_i[1]*delta_t) + p.getY();
          liste_particules[i] = Particule(x,y,p.getVx(),p.getVy(),p.getFx(),p.getFy(),
                               p.getMasse(),p.getType(),p.getId());
          F_old.push_back(F_i);
        }
        calcul_force_F(F,liste_particules);
        for (int i = 0; i < len_liste; i++ ){
          Particule& p = liste_particules[i];
          vector<double> F_i = F[i];
          vector<double> F_old_i = F_old[i];
          double vx = delta_t*0.5/p.getMasse()*(F_i[0] + F_old_i[0]) + p.getVx();
          double vy = delta_t*0.5/p.getMasse()*(F_i[1] + F_old_i[1]) + p.getVy();
          liste_particules[i] = Particule(p.getX(),p.getY(),vx,vy,p.getFx(),p.getFy(),
          p.getMasse(),p.getType(),p.getId());
        }
        std :: cout << "t = " << t << "\n";
        for (int i = 0;i < len_liste;i++){
          Particule p_i = liste_particules[i];
          std :: cout << "nom = " << p_i.getType() << "\n" ;
          std :: cout << "x = " << p_i.getX() << "\n";
          std :: cout << "y = " << p_i.getY() << "\n";
        }
        F_old.clear();
    }
}