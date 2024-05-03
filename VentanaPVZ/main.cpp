#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

sf::RenderWindow window;
bool fullscreen;

sf::Sprite backgroundSprite;
sf::Texture backgroundTexture;
sf::RectangleShape button1;
sf::RectangleShape button2;

void draw() {
    window.clear();
    window.draw(backgroundSprite);
    window.draw(button1);
    window.draw(button2);
    window.display();
}

void input() {
    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
            window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            if (fullscreen)
                window.create(sf::VideoMode::getFullscreenModes()[0], "Plantas Vs Zombis");
            else
                window.create(sf::VideoMode(WIDTH, HEIGHT), "Plantas Vs Zombis");
            fullscreen = !fullscreen;
        }
    }
}

void update() {
    // Aquí iría el código para actualizar el estado del juego
}

int main() {
    if (!backgroundTexture.loadFromFile("menu_pvz.png")) {
        std::cout << "Error al cargar el archivo de imagen\n";
        return 1;
    }
    backgroundSprite.setTexture(backgroundTexture);

    button1.setSize(sf::Vector2f(200, 50));
    button1.setFillColor(sf::Color::Blue);
    button1.setPosition(750, 100);

    button2.setSize(sf::Vector2f(200, 50));
    button2.setFillColor(sf::Color::Green);
    button2.setPosition(750, 450);

    window.create(sf::VideoMode(WIDTH, HEIGHT), "Plantas Vs Zombis");
    window.setFramerateLimit(60);
    fullscreen = false;

    sf::Music music;
    if (!music.openFromFile("musica_pvz.ogg")) {
        std::cout << "Error al cargar el archivo de audio\n";
        return 1;
    }
    music.play();

    while (window.isOpen()) {
        draw();
        input();
        update();
    }

    return 0;
}
//para compilar g++ main3.cpp -o tea -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio
