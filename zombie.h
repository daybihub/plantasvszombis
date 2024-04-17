#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <string>
using namespace std;

class Zombie {
private:
  string tipo;
  int dano;
  int puntos_vida;
  int velocidad;

public:
  // Constructor
  Zombie(string _tipo, int _dano, int _puntos_vida, int _velocidad)
      : tipo(_tipo), dano(_dano), puntos_vida(_puntos_vida),
        velocidad(_velocidad) {}

  // Métodos para acceder a los atributos
  string getTipo() const { 
    return tipo; 
  }
  int getDano() const { 
    return dano; 
  }
  int getPuntosVida() const { 
    return puntos_vida; 
  }
  int getVelocidad() const { 
    return velocidad; 
  }

  // Métodos para modificar los atributos si es necesario
  void setTipo(string _tipo) { 
    tipo = _tipo; 
  }
  void setDano(int _dano) { 
    dano = _dano; 
  }
  void setPuntosVida(int _puntos_vida) { 
    puntos_vida = _puntos_vida; 
  }
  void setVelocidad(int _velocidad) { 
    velocidad = _velocidad; 
  }
};

#endif // ZOMBIE_H
