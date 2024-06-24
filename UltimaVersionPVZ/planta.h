#ifndef PLANTA_H
#define PLANTA_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "sistemasoles.h"

enum TipoPlanta { GUISANTE, GIRASOL, REPETIDORA };

class Planta {
public:
    Planta(const sf::Texture& textura);

    virtual void actualizar(std::vector<sf::Sprite>& guisantes, const sf::Texture& texturaGuisante, SistemaSoles& sistemaSoles) = 0;
    const sf::Sprite& getSprite() const;
    virtual TipoPlanta getTipo() const = 0;

    // Sobrecarga del operador =
    void operator=(const sf::Vector2f& posicion);

protected:
    sf::Sprite sprite;
    sf::Clock relojAtaque;
    sf::Clock relojGeneracionSoles;
};

class Girasol : public Planta {
public:
    Girasol(const sf::Texture& textura);
    void actualizar(std::vector<sf::Sprite>& guisantes, const sf::Texture& texturaGuisante, SistemaSoles& sistemaSoles) override;
    TipoPlanta getTipo() const override;
};

class Guisante : public Planta {
public:
    Guisante(const sf::Texture& textura);
    void actualizar(std::vector<sf::Sprite>& guisantes, const sf::Texture& texturaGuisante, SistemaSoles& sistemaSoles) override;
    TipoPlanta getTipo() const override;
};

class Repetidora : public Planta {
public:
    Repetidora(const sf::Texture& textura);
    void actualizar(std::vector<sf::Sprite>& guisantes, const sf::Texture& texturaGuisante, SistemaSoles& sistemaSoles) override;
    TipoPlanta getTipo() const override;
};

// Implementación de la clase base Planta
Planta::Planta(const sf::Texture& textura) {
    sprite.setTexture(textura);
    sprite.setScale(70.0f / textura.getSize().x, 90.0f / textura.getSize().y);  // Ajustar tamaño
}

const sf::Sprite& Planta::getSprite() const {
    return sprite;
}

void Planta::operator=(const sf::Vector2f& posicion) {
    sprite.setPosition(posicion);
}

// Implementación de la clase derivada Girasol
Girasol::Girasol(const sf::Texture& textura) : Planta(textura) {}

void Girasol::actualizar(std::vector<sf::Sprite>& guisantes, const sf::Texture& texturaGuisante, SistemaSoles& sistemaSoles) {
    if (relojGeneracionSoles.getElapsedTime() >= sf::seconds(3)) {
        sistemaSoles.generarSoles();
        relojGeneracionSoles.restart();
    }
}

TipoPlanta Girasol::getTipo() const {
    return GIRASOL;
}

// Implementación de la clase derivada Guisante
Guisante::Guisante(const sf::Texture& textura) : Planta(textura) {}

void Guisante::actualizar(std::vector<sf::Sprite>& guisantes, const sf::Texture& texturaGuisante, SistemaSoles& sistemaSoles) {
    if (relojAtaque.getElapsedTime() >= sf::seconds(1.5f)) {
        sf::Sprite guisante;
        guisante.setTexture(texturaGuisante);
        guisante.setPosition(sprite.getPosition().x + sprite.getGlobalBounds().width, sprite.getPosition().y + sprite.getGlobalBounds().height / 2);
        guisantes.push_back(guisante);
        relojAtaque.restart();
    }
}

TipoPlanta Guisante::getTipo() const {
    return GUISANTE;
}

// Implementación de la clase derivada Repetidora
Repetidora::Repetidora(const sf::Texture& textura) : Planta(textura) {}

void Repetidora::actualizar(std::vector<sf::Sprite>& guisantes, const sf::Texture& texturaGuisante, SistemaSoles& sistemaSoles) {
    if (relojAtaque.getElapsedTime() >= sf::seconds(1.5f)) {
        sf::Sprite guisante1, guisante2;
        guisante1.setTexture(texturaGuisante);
        guisante2.setTexture(texturaGuisante);
        guisante1.setPosition(sprite.getPosition().x + sprite.getGlobalBounds().width, sprite.getPosition().y + sprite.getGlobalBounds().height / 3);
        guisante2.setPosition(sprite.getPosition().x + sprite.getGlobalBounds().width, sprite.getPosition().y + 2 * sprite.getGlobalBounds().height / 3);
        guisantes.push_back(guisante1);
        guisantes.push_back(guisante2);
        relojAtaque.restart();
    }
}

TipoPlanta Repetidora::getTipo() const {
    return REPETIDORA;
}

#endif

