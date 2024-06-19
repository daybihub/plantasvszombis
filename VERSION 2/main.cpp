#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

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

const int cantidadSemillas = 5;
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

struct Planta {
    sf::Sprite sprite;
    sf::Clock relojAtaque;
};

struct Zombi {
    sf::Sprite sprite;
    float velocidad;
};

std::vector<Planta> plantas;

sf::Texture texturaGuisante;
std::vector<sf::Sprite> guisantes;

sf::Texture texturaZombi;
std::vector<Zombi> zombis;

sf::Clock relojGeneracionZombis;
int zombisGenerados = 0;

void ajustarTamanoFondo(sf::Sprite &sprite) {
    sf::Vector2u tamanoVentana = ventana.getSize();
    sf::Vector2u tamanoTextura = sprite.getTexture()->getSize();
    float escalaX = static_cast<float>(tamanoVentana.x) / tamanoTextura.x;
    float escalaY = static_cast<float>(tamanoVentana.y) / tamanoTextura.y;
    sprite.setScale(escalaX, escalaY);
}

void generarZombi() {
    if (zombisGenerados < 20) {
        Zombi zombi;
        zombi.sprite.setTexture(texturaZombi);
        zombi.sprite.setScale(80.0f / texturaZombi.getSize().y, 80.0f / texturaZombi.getSize().y);  // Ajustar tamaño
        int fila = rand() % FILAS;
        zombi.sprite.setPosition(ANCHO - 100, 200 + fila * TAMANO_CELDA);  // Poner zombi en una fila aleatoria
        zombi.velocidad = 0.5f;  // Velocidad del zombi
        zombis.push_back(zombi);
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
            ventana.draw(planta.sprite);
        }
        for (auto& guisante : guisantes) {
            ventana.draw(guisante);
        }
        for (auto& zombi : zombis) {
            ventana.draw(zombi.sprite);
        }
        for (int i = 0; i < cantidadSemillas; ++i) {
            ventana.draw(spritesSemillas[i]);
        }
        if (spriteArrastrando) {
            ventana.draw(*spriteArrastrando);
        }
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
                            arrastrando = true;
                            spriteArrastrando = &spritesSemillas[i];
                            break;
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
                        Planta planta;
                        planta.sprite = *spriteArrastrando;
                        planta.sprite.setPosition(celda.getPosition());
                        plantas.push_back(planta);
                        spriteArrastrando->setPosition(50 + (spriteArrastrando - &spritesSemillas[0]) * 100, 20);  // Reponer semilla
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
    sf::Time intervaloAtaque = sf::seconds(3);
    for (auto& planta : plantas) {
        if (planta.relojAtaque.getElapsedTime() >= intervaloAtaque) {
            sf::Sprite guisante;
            guisante.setTexture(texturaGuisante);
            guisante.setPosition(planta.sprite.getPosition().x + planta.sprite.getGlobalBounds().width, planta.sprite.getPosition().y + planta.sprite.getGlobalBounds().height / 2);
            guisantes.push_back(guisante);
            planta.relojAtaque.restart();
        }
    }

    for (auto it = guisantes.begin(); it != guisantes.end(); ) {
        it->move(5, 0);  // Mover guisante hacia la derecha
        if (it->getPosition().x > ANCHO) {
            it = guisantes.erase(it);  // Eliminar guisante si sale del tablero
        } else {
            ++it;
        }
    }

    for (auto& zombi : zombis) {
        zombi.sprite.move(-zombi.velocidad, 0);  // Mover zombi hacia la izquierda
    }

    for (auto itGuisante = guisantes.begin(); itGuisante != guisantes.end(); ) {
        bool guisanteEliminado = false;
        for (auto itZombi = zombis.begin(); itZombi != zombis.end(); ) {
            if (itGuisante->getGlobalBounds().intersects(itZombi->sprite.getGlobalBounds())) {
                itZombi = zombis.erase(itZombi);  // Eliminar zombi si es golpeado por un guisante
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

    if (relojGeneracionZombis.getElapsedTime() >= sf::seconds(5)) {
        generarZombi();
        relojGeneracionZombis.restart();
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    if (!texturaFondoMenu.loadFromFile("menu_pvz.png")) {
        std::cout << "Error al cargar el archivo de imagen menu_pvz.png\n";
        return 1;
    }
    spriteFondoMenu.setTexture(texturaFondoMenu);

    if (!texturaFondoJuego.loadFromFile("patio_dia.png")) {
        std::cout << "Error al cargar el archivo de imagen patio_dia.png\n";
        return 1;
    }
    spriteFondoJuego.setTexture(texturaFondoJuego);

    if (!fuente.loadFromFile("arial.ttf")) {
        std::cout << "Error al cargar la fuente\n";
        return 1;
    }

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

    if (!texturaGuisante.loadFromFile("bolita_guisantes.png")) {
        std::cout << "Error al cargar el archivo de imagen bolita_guisantes.png\n";
        return 1;
    }

    if (!texturaZombi.loadFromFile("zombi_normal.gif")) {
        std::cout << "Error al cargar el archivo de imagen zombi_normal.gif\n";
        return 1;
    }

    for (int i = 0; i < cantidadSemillas; ++i) {
        if (!texturasSemillas[i].loadFromFile("seed" + std::to_string(i + 1) + ".png")) {
            std::cout << "Error al cargar el archivo de imagen seed" << i + 1 << ".png\n";
            return 1;
        }
        spritesSemillas[i].setTexture(texturasSemillas[i]);
        spritesSemillas[i].setScale(70.0f / texturasSemillas[i].getSize().x, 70.0f / texturasSemillas[i].getSize().y);  // Ajustar tamaño
        spritesSemillas[i].setPosition(50 + i * 100, 20);  // Espacio entre semillas
    }

    ventana.create(sf::VideoMode(ANCHO, ALTO), "Plantas Vs Zombis");
    ventana.setFramerateLimit(60);
    pantallaCompleta = false;

    sf::Music musica;
    if (!musica.openFromFile("musica_pvz.ogg")) {
        std::cout << "Error al cargar el archivo de audio\n";
        return 1;
    }
    musica.play();

    ajustarTamanoFondo(spriteFondoJuego);

    // Crear el terreno
    for (int fila = 0; fila < FILAS; ++fila) {
        for (int columna = 0; columna < COLUMNAS; ++columna) {
            sf::RectangleShape celda(sf::Vector2f(TAMANO_CELDA, TAMANO_CELDA));
            celda.setPosition(50 + columna * TAMANO_CELDA, 200 + fila * TAMANO_CELDA);  // Mover el tablero 50 px hacia abajo y a la izquierda
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
