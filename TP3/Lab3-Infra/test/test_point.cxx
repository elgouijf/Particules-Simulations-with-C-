#include "../src/vecteur.hxx"
#include <iostream>
#include <cmath>

int main(){
  vecteur v1;
  vecteur v2;
  vecteur v3(1.0, 2.0, 3.0);
  vecteur v4 = v3*2.0;
  
  std::cout<<v1<<std::endl;
  std::cout<<v2<<std::endl;
  std::cout<<v3<<std::endl;
  std::cout<<v4<<std::endl;
  
  std::cout<<v3.norme()<<std::endl;
  
  std::cout<<(v3+v4)<<std::endl;
  std::cout<<(v4-v3)<<std::endl;
  std::cout<<(v3*4)<<std::endl;


  return EXIT_SUCCESS;
}