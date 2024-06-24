#ifndef ZOMBI_H
#define ZOMBI_H

#include <SFML/Graphics.hpp>
#include <vector>

enum TipoZombi { NORMAL, CONO };

class Zombi {
public:
    Zombi(const sf::Texture& textura);

    virtual void mover() = 0;
    virtual bool recibirdaño(int daño) = 0;
    const sf::Sprite& getSprite() const;
    virtual TipoZombi getTipo() const = 0;
    float getPosicionX() const;

protected:
    sf::Sprite sprite;
    int vida;
    float velocidad;
};

class ZombiNormal : public Zombi {
public:
    ZombiNormal(const sf::Texture& textura);
    void mover() override;
    bool recibirdaño(int daño) override;
    TipoZombi getTipo() const override;
};

class ZombiCono : public Zombi {
public:
    ZombiCono(const sf::Texture& textura);
    void mover() override;
    bool recibirdaño(int daño) override;
    TipoZombi getTipo() const override;
};

// Implementación de la clase base Zombi
Zombi::Zombi(const sf::Texture& textura) {
    sprite.setTexture(textura);
    sprite.setScale(80.0f / textura.getSize().y, 80.0f / textura.getSize().y);  // Ajustar tamaño
    int fila = rand() % 5;
    sprite.setPosition(1280 - 100, 200 + fila * 95);  // Poner zombi en una fila aleatoria
}

const sf::Sprite& Zombi::getSprite() const {
    return sprite;
}

float Zombi::getPosicionX() const {
    return sprite.getPosition().x;
}

// Implementación de la clase derivada ZombiNormal
ZombiNormal::ZombiNormal(const sf::Texture& textura) : Zombi(textura) {
    vida = 10;
    velocidad = 0.4f;
}

void ZombiNormal::mover() {
    sprite.move(-velocidad, 0);
}

bool ZombiNormal::recibirdaño(int daño) {
    vida -= daño;
    return vida <= 0;
}

TipoZombi ZombiNormal::getTipo() const {
    return NORMAL;
}

// Implementación de la clase derivada ZombiCono
ZombiCono::ZombiCono(const sf::Texture& textura) : Zombi(textura) {
    vida = 15;
    velocidad = 0.4f;
}

void ZombiCono::mover() {
    sprite.move(-velocidad, 0);
}

bool ZombiCono::recibirdaño(int daño) {
    vida -= daño;
    return vida <= 0;
}

TipoZombi ZombiCono::getTipo() const {
    return CONO;
}

#endif
