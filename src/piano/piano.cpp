#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "utils.hpp"

class Piano: public sf::Drawable, public sf::Transformable {
private:
    struct PianoKey {
        std::string m_name;
        sf::RectangleShape m_shape;
        sf::SoundBuffer m_buffer;
        sf::Sound m_sound;
        bool m_pressed;
        void press() {
            if (!m_pressed) {
                m_pressed = true;
                m_sound.play();
                sf::Color c = m_shape.getOutlineColor();
                c.a = m_name.back() == 's' ? 220 : 128;
                m_shape.setOutlineColor(c);
            }
        }
        void release() {
            if (m_pressed) {
                m_pressed = false;
                sf::Color c = m_shape.getOutlineColor();
                c.a = 0;
                m_shape.setOutlineColor(c);
            }
        }
    };
    // bugfix: don't use std::vector<PianoKey>, otherwise the soundbuffer
    // will be destroyed while sound still use it.
    sf::Vector2f m_whiteKeySize, m_blackKeySize;
    std::vector<std::pair<std::string, sf::Keyboard::Key>> m_keyMap{
         {"c1", sf::Keyboard::A} ,{"c1s", sf::Keyboard::W} ,{"d1", sf::Keyboard::S} ,{"d1s", sf::Keyboard::E},
         {"e1", sf::Keyboard::D} ,{"f1", sf::Keyboard::F} ,{"f1s", sf::Keyboard::T} ,{"g1", sf::Keyboard::G},
         {"g1s", sf::Keyboard::Y} ,{"a1", sf::Keyboard::H} ,{"a1s", sf::Keyboard::U} ,{"b1", sf::Keyboard::J}
    };
    std::vector<std::pair<sf::Keyboard::Key, PianoKey*>> m_pianoKeys;
    const sf::Window& m_window;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        for (auto keyPair : m_pianoKeys) {
            target.draw(keyPair.second->m_shape, states);
        }
    };

public:
    Piano(const sf::Window& window, sf::Vector2f size = { 800.0f, 600.0f }) :
        m_window(window) {
        m_whiteKeySize = { size.x / 7.0f, size.y };
        m_blackKeySize = { size.x / 14.0f, size.y * 0.6f };
        int whiteCount = 0;
        // std::srand(0);
        for (auto keyPair : m_keyMap) {
            std::string keyName = keyPair.first;
            PianoKey* key = new PianoKey;
            key->m_name = keyName;
            if (!key->m_buffer.loadFromFile(Utils::fetch("piano/"+keyName+".wav"))) {
                exit(-1);
            }
            key->m_sound.setBuffer(key->m_buffer);
            key->m_shape.setOutlineColor(sf::Color(std::rand()%0xff, std::rand()%0xff, std::rand()%0xff, 0));
            bool isBlack = (keyName.back() == 's');
            key->m_shape.setOutlineThickness(isBlack ? -m_blackKeySize.x / 2.0f : -m_whiteKeySize.x / 2.0f);
            key->m_shape.setFillColor(isBlack ? sf::Color::Black : sf::Color::White);
            key->m_shape.setSize(isBlack ? m_blackKeySize : m_whiteKeySize);
            key->m_shape.setPosition((whiteCount * m_whiteKeySize.x - (isBlack ? m_blackKeySize.x : 0)), 0);
            m_pianoKeys.push_back({ keyPair.second, key });
            whiteCount += !isBlack;
        }
    };
    ~Piano() {
        for (auto p : m_pianoKeys) {
            delete p.second;
        }
    }
    // info: 相较于事件驱动，直接用每次绘制帧都在update中检测琴键是否被按下实现起来更简单
    // 虽然事件驱动是更节省资源的方式，但在这个例子中，这并不是影响性能的关键因素
    void update() {
        bool blackMousePressed = false; 
        for (auto keyPair = m_pianoKeys.rbegin(); keyPair != m_pianoKeys.rend(); ++keyPair) {
            bool keyPressed = false;
            PianoKey* key = keyPair->second;
            if (sf::Keyboard::isKeyPressed(keyPair->first)) {
                keyPressed = true;
            }
            // 如果已经有黑键被鼠标点击了，那么下面的白键就被忽略。
            bool mousePressed = false;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                auto rect = sf::FloatRect(key->m_shape.getPosition(), key->m_shape.getSize());
                if (rect.contains(sf::Vector2f(sf::Mouse::getPosition(m_window)))) {
                    if (!blackMousePressed) {
                        mousePressed = true;
                    }
                    blackMousePressed = true;
                }
            }
            if (mousePressed || keyPressed) {
                key->press();
            } else if (!mousePressed && !keyPressed) {
                key->release();
            }

        }
    }
};

int main(int argc, char **argv) {
    Utils::initialize(argv[0]);

    sf::Image icon;
    icon.loadFromFile(Utils::fetch("piano/piano.png"));
    sf::Vector2f size(500, 300);

    sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Simple Piano", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    Piano piano(window, size);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        piano.update();

        window.clear();
        window.draw(piano);
        window.display();
    }
    return EXIT_SUCCESS;
}