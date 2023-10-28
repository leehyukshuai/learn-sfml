#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <utils.hpp>

#define FPS 60.0f

int main(int argc, char *argv[]) {
    sf::Music music;
    music.openFromFile(fetch(argv[0], "allure.mp3"));
    music.play();

    while (1) {

        sf::Vector2i pos = sf::Mouse::getPosition();
        float pitch = (pos.x * 4.0f / sf::VideoMode::getFullscreenModes()[0].width);
        music.setPitch(pitch);

        static bool last = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (!last) {
                if (music.getStatus() == music.Playing) {
                    music.pause();
                } else {
                    music.play();
                }
            }
            last = true;
        } else {
            last = false;
        }

        sf::sleep(sf::milliseconds(1000.0f / FPS));
    }
}
