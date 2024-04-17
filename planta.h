#ifndef PLANTA_H
#define PLANTA_H
using namespace std;

#include <string>

class Planta {
private:
  string tipo;
  int daño;
  int puntos_vida;
  int costo;

public:
  // Constructor
  Planta(std::string _tipo, int _dano, int _puntos_vida, int _costo)
      : tipo(_tipo), daño(_dano), puntos_vida(_puntos_vida), costo(_costo) {}

  // Métodos para acceder a los atributos
  string getTipo() const { 
    return tipo; 
  }
  int getDano() const { 
    return daño; 
  }
  int getPuntosVida() const { 
    return puntos_vida; 
  }
  int getCosto() const { 
    return costo; 
  }

  // Métodos para modificar los atributos si es necesario
  void setTipo(string _tipo) { 
    tipo = _tipo; 
  }
  void setDano(int _dano) { 
    daño = _dano; 
  }
  void setPuntosVida(int _puntos_vida) { 
    puntos_vida = _puntos_vida; 
  }
  void setCosto(int _costo) { 
    costo = _costo; 
  }
};

#endif // PLANTA_H
