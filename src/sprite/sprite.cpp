#include <SFML/Graphics.hpp>
#include <sstream>
#include <cmath>
#include <utils.hpp>
#include <unordered_map>

bool isMoveKey(sf::Keyboard::Key k) {
    return (k == sf::Keyboard::W)
        || (k == sf::Keyboard::A)
        || (k == sf::Keyboard::S)
        || (k == sf::Keyboard::D);
}

int main(int argc, char **argv) {
    Utils::initialize(argv[0]);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "sprite", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    if (!font.loadFromFile(Utils::fetch("consola.ttf"))) {
        return EXIT_FAILURE;
    }

    sf::Texture backGround;
    if (!backGround.loadFromFile(Utils::fetch("flower.jpg"))) {
        return EXIT_FAILURE;
    }
    sf::Texture shapeTexture;
    if (!shapeTexture.loadFromFile(Utils::fetch("field.jpg"))) {
        return EXIT_FAILURE;
    }

    sf::Sprite backImage(backGround);

    sf::CircleShape circle(100);
    circle.setOrigin(circle.getLocalBounds().width / 2.0f, circle.getLocalBounds().height / 2.0f);
    circle.setTexture(&shapeTexture, true);
    circle.setOutlineColor({255, 0, 255, 128});
    circle.setOutlineThickness(16);
    sf::Vector2f pos;
    float speed = 2;

    std::unordered_map<sf::Keyboard::Key, bool> moveArray(4);

    while (window.isOpen()) {

        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (isMoveKey(event.key.code))
                    moveArray[event.key.code] = true;
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                break;
            case sf::Event::KeyReleased:
                if (isMoveKey(event.key.code))
                    moveArray[event.key.code] = false;
                break;
            }
        }

        // Update
        sf::Vector2f direct;
        if (moveArray[sf::Keyboard::W]) {
            direct -= {0, 1};
        }
        if (moveArray[sf::Keyboard::S]) {
            direct += {0, 1};
        }
        if (moveArray[sf::Keyboard::A]) {
            direct -= {1, 0};
        }
        if (moveArray[sf::Keyboard::D]) {
            direct += {1, 0};
        }
        float dist = std::sqrt(direct.x * direct.x + direct.y * direct.y);
        if (dist) {
            direct /= dist;
            pos += direct * speed;
        }
        circle.setPosition(pos);

        // Render
        window.clear();

        window.draw(backImage);

        circle.setTextureRect(sf::IntRect(circle.getGlobalBounds()));
        window.draw(circle);

        std::stringstream s;
        s << "x:" << pos.x << "\n"
            << "y:" << pos.y;
        sf::Text text(s.str(), font);
        text.setCharacterSize(40);
        window.draw(text);

        window.display();
    }
}