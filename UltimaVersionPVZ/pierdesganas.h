#ifndef PIERDESGANAS_H
#define PIERDESGANAS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "zombi.h"

class PierdesGanas {
public:
    PierdesGanas();

    void actualizar(const std::vector<std::unique_ptr<Zombi>>& zombis, int zombisGenerados);
    void dibujar(sf::RenderWindow& ventana);
    void setPerdiste(bool estado);
    bool juegoTerminado() const;

private:
    sf::Texture texturaGanar;
    sf::Texture texturaPerder;
    sf::Sprite spriteGanar;
    sf::Sprite spritePerder;
    bool perdiste;
    bool ganaste;
};

PierdesGanas::PierdesGanas() : perdiste(false), ganaste(false) {
    if (!texturaGanar.loadFromFile("archivos/ganaste.png")) {
    }
    if (!texturaPerder.loadFromFile("archivos/perdiste.png")) {
    }
    spriteGanar.setTexture(texturaGanar);
    spritePerder.setTexture(texturaPerder);
    spriteGanar.setPosition(0, 0);
    spritePerder.setPosition(0, 0);
}

void PierdesGanas::actualizar(const std::vector<std::unique_ptr<Zombi>>& zombis, int zombisGenerados) {
    if (perdiste || ganaste) return;

    for (const auto& zombi : zombis) {
        if (zombi->getPosicionX() < 200) {
            perdiste = true;
            return;
        }
    }

    if (zombisGenerados >= 25 && zombis.empty()) {
        ganaste = true;
    }
}

void PierdesGanas::dibujar(sf::RenderWindow& ventana) {
    if (perdiste) {
        ventana.draw(spritePerder);
    } else if (ganaste) {
        ventana.draw(spriteGanar);
    }
}

void PierdesGanas::setPerdiste(bool estado) {
    perdiste = estado;
}

bool PierdesGanas::juegoTerminado() const {
    return perdiste || ganaste;
}

#endif

