#ifndef SISTEMASOLES_H
#define SISTEMASOLES_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class SistemaSoles {
public:
    SistemaSoles();

    void actualizar();
    void dibujar(sf::RenderWindow& ventana);
    void generarSoles();

    int obtenerSoles() const;
    void reducirSoles(int cantidad);  // Añadir el método reducirSoles

private:
    int contadorSoles;
    sf::Clock relojGeneracionSoles;
    sf::Text textoSoles;
    sf::Font fuente;
};

SistemaSoles::SistemaSoles() : contadorSoles(50) {
    if (!fuente.loadFromFile("archivos/caprina_regular.ttf")) {}
    textoSoles.setFont(fuente);
    textoSoles.setCharacterSize(24);
    textoSoles.setFillColor(sf::Color::Black);
    textoSoles.setPosition(210, 100);  // Ajustar posición del contador
    textoSoles.setString(std::to_string(contadorSoles));
}

void SistemaSoles::actualizar() {
    // El método actualizar solo se encarga de actualizar el texto
    textoSoles.setString(std::to_string(contadorSoles));
}

void SistemaSoles::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(textoSoles);
}

void SistemaSoles::generarSoles() {
    contadorSoles += 25;
    textoSoles.setString(std::to_string(contadorSoles));
}

int SistemaSoles::obtenerSoles() const {
    return contadorSoles;
}

void SistemaSoles::reducirSoles(int cantidad) {  // Implementar el método reducirSoles
    contadorSoles -= cantidad;
    textoSoles.setString(std::to_string(contadorSoles));
}

#endif
