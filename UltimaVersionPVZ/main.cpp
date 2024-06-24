#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <memory>  // Necesario para usar punteros inteligentes
#include "sistemasoles.h"
#include "planta.h"
#include "zombi.h"
#include "pierdesganas.h"
#include "sistemacompras.h"

#define ANCHO 1280
#define ALTO 720
#define FILAS 5
#define COLUMNAS 9
#define TAMANO_CELDA 95  // Incrementar el tamaño de las celdas

sf::RenderWindow ventana;
bool pantallaCompleta;

sf::Sprite spriteFondoMenu;
sf::Texture texturaFondoMenu;

sf::Sprite spriteFondoJuego;
sf::Texture texturaFondoJuego;

const int cantidadSemillas = 3;
sf::Texture texturasSemillas[cantidadSemillas];
sf::Sprite spritesSemillas[cantidadSemillas];

std::vector<sf::RectangleShape> cuadrilla;
sf::Color verdeClaro(144, 238, 144);
sf::Color verdeOscuro(0, 128, 0);

sf::RectangleShape boton1;
sf::Text textoBoton1;
sf::Font fuente;

bool juegoIniciado = false;
bool arrastrando = false;
sf::Sprite* spriteArrastrando = nullptr;

bool entradaUsuarioActiva = true;
std::string nombreUsuario;
sf::Text textoNombreUsuario;
sf::Text textoPrompt;

// vectores para generacion de plantas y zombies
std::vector<std::unique_ptr<Planta>> plantas;

sf::Texture texturaGirasol;
sf::Texture texturaGuisante;
sf::Texture texturaRepetidora;
sf::Texture texturaZombiNormal;
sf::Texture texturaZombiCono;
int zombisGenerados = 0;

// Gestión de Zombis y Guisantes, Gestión del Tiempo, 
std::vector<std::unique_ptr<Zombi>> zombis;

std::vector<sf::Sprite> guisantes;

sf::Clock relojGeneracionZombis;
sf::Clock relojGeneracionZombisCono;

SistemaSoles sistemaSoles;
SistemaCompras sistemaCompras;
PierdesGanas pierdesGanas;

sf::SoundBuffer bufferZombiComiendo;
sf::Sound sonidoZombiComiendo;
sf::Clock relojSonidoComiendo;

void ajustarTamanoFondo(sf::Sprite &sprite) {
    sf::Vector2u tamanoVentana = ventana.getSize();
    sf::Vector2u tamanoTextura = sprite.getTexture()->getSize();
    float escalaX = static_cast<float>(tamanoVentana.x) / tamanoTextura.x;
    float escalaY = static_cast<float>(tamanoVentana.y) / tamanoTextura.y;
    sprite.setScale(escalaX, escalaY);
}

void generarZombi(TipoZombi tipo) {
    if (tipo == NORMAL && zombisGenerados < 20) {
        zombis.push_back(std::make_unique<ZombiNormal>(texturaZombiNormal));
        zombisGenerados++;
    } else if (tipo == CONO && zombisGenerados < 5) {
        zombis.push_back(std::make_unique<ZombiCono>(texturaZombiCono));
        zombisGenerados++;
    }
}

void dibujar() {
    ventana.clear(sf::Color::Black);  // Fondo negro

    if (entradaUsuarioActiva) {
        ventana.draw(textoPrompt);
        ventana.draw(textoNombreUsuario);
    } else if (juegoIniciado) {
        ventana.clear();
        ventana.draw(spriteFondoJuego);
        for (auto& celda : cuadrilla) {
            ventana.draw(celda);
        }
        for (auto& planta : plantas) {
            ventana.draw(planta->getSprite());
        }
        for (auto& guisante : guisantes) {
            ventana.draw(guisante);
        }
        for (auto& zombi : zombis) {
            ventana.draw(zombi->getSprite());
        }
        for (int i = 0; i < cantidadSemillas; ++i) {
            ventana.draw(spritesSemillas[i]);
        }
        if (spriteArrastrando) {
            ventana.draw(*spriteArrastrando);
        }
        sistemaSoles.dibujar(ventana);
        pierdesGanas.dibujar(ventana);  // Dibujar el mensaje de ganar o perder si corresponde
    } else {
        ventana.draw(spriteFondoMenu);
        ventana.draw(boton1);
        ventana.draw(textoBoton1);
    }
    ventana.display();
}

void entrada() {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed)
            ventana.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            ventana.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            if (pantallaCompleta)
                ventana.create(sf::VideoMode::getFullscreenModes()[0], "Plantas Vs Zombis", sf::Style::Fullscreen);
            else
                ventana.create(sf::VideoMode(ANCHO, ALTO), "Plantas Vs Zombis");
            pantallaCompleta = !pantallaCompleta;
            ajustarTamanoFondo(spriteFondoJuego);
        }

        if (evento.type == sf::Event::Resized) {
            ajustarTamanoFondo(spriteFondoJuego);
        }

        if (entradaUsuarioActiva) {
            if (evento.type == sf::Event::TextEntered) {
                if (evento.text.unicode == '\b') {  // Borrar carácter
                    if (!nombreUsuario.empty()) {
                        nombreUsuario.pop_back();
                    }
                } else if (evento.text.unicode == '\r') {  // Enter presionado
                    entradaUsuarioActiva = false;
                } else if (evento.text.unicode < 128) {
                    nombreUsuario += static_cast<char>(evento.text.unicode);
                }
                textoNombreUsuario.setString(nombreUsuario);
            }
        } else if (evento.type == sf::Event::MouseButtonPressed) {
            if (evento.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i posRaton = sf::Mouse::getPosition(ventana);
                if (!juegoIniciado && boton1.getGlobalBounds().contains(static_cast<float>(posRaton.x), static_cast<float>(posRaton.y))) {
                    juegoIniciado = true;
                }
                if (juegoIniciado) {
                    for (int i = 0; i < cantidadSemillas; ++i) {
                        if (spritesSemillas[i].getGlobalBounds().contains(static_cast<float>(posRaton.x), static_cast<float>(posRaton.y))) {
                            TipoPlanta tipoPlanta;
                            if (i == 0) {
                                tipoPlanta = GUISANTE;
                            } else if (i == 1) {
                                tipoPlanta = REPETIDORA;
                            } else if (i == 2) {
                                tipoPlanta = GIRASOL;
                            }
                            if (sistemaCompras.puedeComprar(tipoPlanta, sistemaSoles.obtenerSoles())) {
                                arrastrando = true;
                                spriteArrastrando = &spritesSemillas[i];
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (evento.type == sf::Event::MouseButtonReleased) {
            if (evento.mouseButton.button == sf::Mouse::Left && arrastrando) {
                arrastrando = false;
                sf::Vector2i posRaton = sf::Mouse::getPosition(ventana);
                for (auto& celda : cuadrilla) {
                    if (celda.getGlobalBounds().contains(static_cast<float>(posRaton.x), static_cast<float>(posRaton.y))) {
                        TipoPlanta tipoPlanta;
                        if (spriteArrastrando == &spritesSemillas[0]) {
                            tipoPlanta = GUISANTE;
                        } else if (spriteArrastrando == &spritesSemillas[1]) {
                            tipoPlanta = REPETIDORA;
                        } else if (spriteArrastrando == &spritesSemillas[2]) {
                            tipoPlanta = GIRASOL;
                        }
                        std::unique_ptr<Planta> planta;
                        if (tipoPlanta == GUISANTE) {
                            planta = std::make_unique<Guisante>(texturaGuisante);
                        } else if (tipoPlanta == REPETIDORA) {
                            planta = std::make_unique<Repetidora>(texturaRepetidora);
                        } else if (tipoPlanta == GIRASOL) {
                            planta = std::make_unique<Girasol>(texturaGirasol);
                        }
                        *planta = celda.getPosition();  // Usar la sobrecarga del operador =
                        plantas.push_back(std::move(planta));
                        sistemaCompras.comprar(tipoPlanta, sistemaSoles);  // Reducir soles después de la compra
                        spriteArrastrando->setPosition(340 + (spriteArrastrando - &spritesSemillas[0]) * 100, 20);  // Reponer semilla
                        spriteArrastrando = nullptr;
                        break;
                    }
                }
            }
        }

        if (evento.type == sf::Event::MouseMoved) {
            if (arrastrando) {
                spriteArrastrando->setPosition(static_cast<float>(evento.mouseMove.x), static_cast<float>(evento.mouseMove.y));
            }
        }
    }
}

void actualizar() {
    if (pierdesGanas.juegoTerminado()) return;  // No actualizar si el juego ha terminado

    for (auto& planta : plantas) {
        planta->actualizar(guisantes, texturaGuisante, sistemaSoles);
    }

    sistemaSoles.actualizar();

    for (auto it = guisantes.begin(); it != guisantes.end(); ) {
        it->move(5, 0);  // Mover guisante hacia la derecha
        if (it->getPosition().x > ANCHO) {
            it = guisantes.erase(it);  // Eliminar guisante si sale del tablero
        } else {
            ++it;
        }
    }

    for (auto& zombi : zombis) {
        zombi->mover();
    }

    // Colisiones entre guisantes y zombis
    for (auto itGuisante = guisantes.begin(); itGuisante != guisantes.end(); ) {
        bool guisanteEliminado = false;
        for (auto itZombi = zombis.begin(); itZombi != zombis.end(); ) {
            if (itGuisante->getGlobalBounds().intersects(itZombi->get()->getSprite().getGlobalBounds())) {
                if (itZombi->get()->recibirdaño(3)) {
                    itZombi = zombis.erase(itZombi);  // Eliminar zombi si su vida es 0 o menor
                } else {
                    ++itZombi;
                }
                itGuisante = guisantes.erase(itGuisante);  // Eliminar guisante
                guisanteEliminado = true;
                break;
            } else {
                ++itZombi;
            }
        }
        if (!guisanteEliminado) {
            ++itGuisante;
        }
    }

    // Colisiones entre zombis y plantas
    for (auto itZombi = zombis.begin(); itZombi != zombis.end(); ) {
        bool plantaComida = false;
        for (auto itPlanta = plantas.begin(); itPlanta != plantas.end(); ) {
            if (itZombi->get()->getSprite().getGlobalBounds().intersects((*itPlanta)->getSprite().getGlobalBounds())) {
                // Reproducir sonido de zombi comiendo
                if (sonidoZombiComiendo.getStatus() != sf::Sound::Playing) {
                    sonidoZombiComiendo.play();
                    relojSonidoComiendo.restart();
                } else if (relojSonidoComiendo.getElapsedTime().asSeconds() > 1) {
                    sonidoZombiComiendo.stop();
                }
                itPlanta = plantas.erase(itPlanta);  // Eliminar planta
                plantaComida = true;
                break;
            } else {
                ++itPlanta;
            }
        }
        if (!plantaComida) {
            ++itZombi;
        }
    }

    if (relojGeneracionZombis.getElapsedTime() >= sf::seconds(8)) {
        generarZombi(NORMAL);
        relojGeneracionZombis.restart();
    }

    if (relojGeneracionZombisCono.getElapsedTime() >= sf::seconds(12)) {
        generarZombi(CONO);
        relojGeneracionZombisCono.restart();
    }

    pierdesGanas.actualizar(zombis, zombisGenerados);  // Actualizar estado de ganar o perder
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    texturaFondoMenu.loadFromFile("archivos/menu_pvz.png");
    spriteFondoMenu.setTexture(texturaFondoMenu);

    texturaFondoJuego.loadFromFile("archivos/patio_dia.png");
    spriteFondoJuego.setTexture(texturaFondoJuego);

    fuente.loadFromFile("archivos/caprina_regular.ttf");

    boton1.setSize(sf::Vector2f(200, 50));
    boton1.setFillColor(sf::Color::Blue);
    boton1.setPosition((ANCHO - 200) / 2 + 400, (ALTO - 50) / 2 - 100);  // Mover el botón 100 unidades hacia arriba

    textoBoton1.setFont(fuente);
    textoBoton1.setString("Comenzar");
    textoBoton1.setCharacterSize(24);
    textoBoton1.setFillColor(sf::Color::White);
    textoBoton1.setPosition(boton1.getPosition().x + 50, boton1.getPosition().y + 10);

    textoPrompt.setFont(fuente);
    textoPrompt.setString("Ingrese su nombre:");
    textoPrompt.setCharacterSize(24);
    textoPrompt.setFillColor(sf::Color::White);
    textoPrompt.setPosition((ANCHO - textoPrompt.getLocalBounds().width) / 2, ALTO / 2 - 50);

    textoNombreUsuario.setFont(fuente);
    textoNombreUsuario.setString("");
    textoNombreUsuario.setCharacterSize(24);
    textoNombreUsuario.setFillColor(sf::Color::White);
    textoNombreUsuario.setPosition((ANCHO - textoNombreUsuario.getLocalBounds().width) / 2, ALTO / 2);

    texturaGirasol.loadFromFile("archivos/girasol.png");
    texturaGuisante.loadFromFile("archivos/seed1.png");
    texturaRepetidora.loadFromFile("archivos/seed2.png");
    texturaZombiNormal.loadFromFile("archivos/zombi_normal.gif");
    texturaZombiCono.loadFromFile("archivos/zombi_cono.gif");

    if (!bufferZombiComiendo.loadFromFile("archivos/zombi_comiendo.mp3")) {
        std::cerr << "Error al cargar el sonido zombi_comiendo.mp3" << std::endl;
        return -1;
    }
    sonidoZombiComiendo.setBuffer(bufferZombiComiendo);

    for (int i = 0; i < cantidadSemillas; ++i) {
        texturasSemillas[i].loadFromFile("archivos/seed" + std::to_string(i + 1) + ".png");
        spritesSemillas[i].setTexture(texturasSemillas[i]);
        spritesSemillas[i].setScale(70.0f / texturasSemillas[i].getSize().x, 90.0f / texturasSemillas[i].getSize().y);  // Ajustar tamaño
        spritesSemillas[i].setPosition(340 + i * 100, 20);  // Espacio entre semillas
    }

    ventana.create(sf::VideoMode(ANCHO, ALTO), "Plantas Vs Zombis");
    ventana.setFramerateLimit(60);
    pantallaCompleta = false;

    sf::Music musica;
    musica.openFromFile("archivos/musica_pvz.ogg");
    musica.play();

    ajustarTamanoFondo(spriteFondoJuego);

    // Crear el terreno
    for (int fila = 0; fila < FILAS; ++fila) {
        for (int columna = 0; columna < COLUMNAS; ++columna) {
            sf::RectangleShape celda(sf::Vector2f(TAMANO_CELDA, TAMANO_CELDA));
            celda.setPosition(315 + columna * TAMANO_CELDA, 215 + fila * TAMANO_CELDA);  // Mover el tablero 50 px hacia abajo y a la izquierda
            if ((columna + fila) % 2 == 0)
                celda.setFillColor(verdeClaro);
            else
                celda.setFillColor(verdeOscuro);
            celda.setOutlineThickness(1);
            celda.setOutlineColor(sf::Color::Black);
            cuadrilla.push_back(celda);
        }
    }

    while (ventana.isOpen()) {
        dibujar();
        entrada();
        actualizar();
    }

    return 0;
}