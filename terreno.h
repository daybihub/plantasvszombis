#ifndef TERRENO_H
#define TERRENO_H

#include <iostream>
#include "planta.h"
#include "zombie.h"
#include <iostream>
#include <vector>

class Terreno {
private:
  std::vector<Planta> plantas;
  std::vector<Zombie> zombies;

public:
  // Constructor
  Terreno() {}

  // Método para generar un zombie
  void generarZombie(Zombie zombie) { 
    zombies.push_back(zombie); 
  }

  // Método para colocar una planta
  void colocarPlanta(Planta planta) { 
    plantas.push_back(planta); 
  }

  
};

#endif // TERRENO_H
