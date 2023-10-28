#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <utils.hpp>
#include <iostream>


class Player: public sf::Drawable {
private:
    const sf::Vector2f m_size = { 400, 180 };

    class Music {
    private:
        sf::Music music;
        float volume = 100.0f;
        const sf::Time delay = sf::seconds(1.0f);
    public:
        enum State {
            Paused, Pausing, Resuming, Resumed
        } state;
        State getState() const {
            return state;
        }

        void openFile(std::string file) {
            music.openFromFile(Utils::fetch(file));
            music.play();
            state = State::Resumed;
        }
        void resume() {
            music.play();
            state = State::Resuming;
        }
        void pause() {
            state = State::Pausing;
        }
        void update(sf::Time dt) {
            if (state == State::Resuming) {
                volume += (dt / delay) * 100.0f;
                if (volume > 100.0f) {
                    volume = 100.0f;
                    state = State::Resumed;
                }
                music.setVolume(volume);
            }
            if (state == State::Pausing) {
                volume -= (dt / delay) * 100.0f;
                if (volume < 0.0f) {
                    volume = 0.0f;
                    state = State::Paused;
                    music.pause();
                }
                music.setVolume(volume);
            }
        }
    } m_music;


    class Plate: public sf::CircleShape {
        sf::Texture m_texture;
        bool m_rotating = true;
    public:
        Plate(float radius, float margin) :CircleShape(radius) {
            m_texture.loadFromFile(Utils::fetch("flower.jpg"));
            m_texture.generateMipmap();
            setTexture(&m_texture);
            setTextureRect(sf::IntRect(0, 0, m_texture.getSize().x, m_texture.getSize().x));
            setRadius(radius);
            setOrigin({ radius,radius });
            setPosition({ margin + radius, margin + radius });
        }
        void update(sf::Time dt) {
            if (m_rotating)
                rotate(dt / sf::seconds(20.0f) * 360.0f);
        }
        void resume() {
            m_rotating = true;
        }
        void pause() {
            m_rotating = false;
        }
    } m_plate;

    class Title: public sf::Text {
        sf::Font font;
    public:
        Title() {
            font.loadFromFile(Utils::fetch("consola.ttf"));

            setFont(font);
            setCharacterSize(32);
            setFillColor(sf::Color::Black);
            setPosition(190.0f, 20.0f);
        }
    } m_title;

    class Message: public sf::Text {
        sf::Font font;
    public:
        Message() {
            font.loadFromFile(Utils::fetch("consola.ttf"));

            setFont(font);
            setCharacterSize(26);
            setFillColor(sf::Color(100, 100, 100));
            setPosition(192.0f, 80.0f);
        }
    } m_message;

    class BackGround: public sf::RectangleShape {
    public:
        BackGround() :RectangleShape({ 400.0f, 180.0f }) {
            setFillColor(sf::Color(230, 230, 230));
        };
    } m_back;

public:
    Player() : m_plate(75.0f, 15.0f) {
        m_title.setString("allure.flac");
        m_message.setString("SPACE : play\nESCAPE: quit");
        m_music.openFile("allure.flac");
    }

    void flip() {
        auto state = m_music.getState();
        if (state == Music::State::Paused || state == Music::State::Pausing) {
            m_music.resume();
            m_plate.resume();
        } else {
            m_music.pause();
            m_plate.pause();
        }
    };

    void update(sf::Time dt) {
        m_plate.update(dt);
        m_music.update(dt);
    }

    sf::Vector2u getSize() {
        return (sf::Vector2u)m_size;
    }
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_back, states);
        target.draw(m_plate, states);
        target.draw(m_title, states);
        target.draw(m_message, states);
    }
};


int main(int argc, char* argv[]) {
    Utils::initialize(argv[0]);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 2;

    sf::Clock clock;

    Player player;

    sf::RenderWindow window(sf::VideoMode(player.getSize().x, player.getSize().y), "Simple MusicPlayer", sf::Style::Titlebar, settings);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == event.Closed) {
                window.close();
            }
            if (event.type == event.KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::Space) {
                    player.flip();
                }
            }
        }

        sf::Time deltaTime = clock.restart();
        player.update(deltaTime);

        window.clear();
        window.draw(player);
        window.display();
    }
}
