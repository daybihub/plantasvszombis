#ifndef SISTEMACOMPRAS_H
#define SISTEMACOMPRAS_H

#include "sistemasoles.h"

class SistemaCompras {
public:
    SistemaCompras();

    bool puedeComprar(TipoPlanta tipo, int soles) const;
    void comprar(TipoPlanta tipo, SistemaSoles& sistemaSoles);

private:
    std::map<TipoPlanta, int> preciosPlantas;
};

SistemaCompras::SistemaCompras() {
    preciosPlantas[GUISANTE] = 100;
    preciosPlantas[REPETIDORA] = 200;
    preciosPlantas[GIRASOL] = 50;
}

bool SistemaCompras::puedeComprar(TipoPlanta tipo, int soles) const {
    auto it = preciosPlantas.find(tipo);
    if (it != preciosPlantas.end() && soles >= it->second) {
        return true;
    }
    return false;
}

void SistemaCompras::comprar(TipoPlanta tipo, SistemaSoles& sistemaSoles) {
    auto it = preciosPlantas.find(tipo);
    if (it != preciosPlantas.end()) {
        sistemaSoles.reducirSoles(it->second);
    }
}

#endif