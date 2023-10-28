#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <iostream>

class Piano: public sf::Drawable {
    struct Key {
        Key(std::string name) : m_name(name), m_highlight(false) {}
        bool load() {
            std::string name = m_name;
            for (char& c : name) {
                if (c >= 'A' && c <= 'Z') {
                    c = c - 'A' + 'a';
                }
            }
            name += ".wav";
            if (!m_soundBuffer.loadFromFile("../res/piano/" + name))
                return false;
            m_sound.setBuffer(m_soundBuffer);
            return true;
        }
        void press() {
            m_highlight = true;
            m_rect.setOutlineThickness(-m_rect.getSize().x * 0.5f);
            m_sound.play();
        }
        void release() {
            m_highlight = false;
            m_rect.setOutlineThickness(0);
        }
        sf::SoundBuffer m_soundBuffer;
        sf::Sound m_sound;
        std::string m_name;
        sf::RectangleShape m_rect;
        bool m_highlight;
    };
private:
    sf::Vector2f m_keysize{ 60.0f, 200.0f };
    std::unordered_map<sf::Keyboard::Key, unsigned> m_mapping{
        {sf::Keyboard::A, 0}, {sf::Keyboard::W, 1}, {sf::Keyboard::S, 2}, {sf::Keyboard::E, 3}, {sf::Keyboard::D, 4}, {sf::Keyboard::F, 5},
        {sf::Keyboard::T, 6}, {sf::Keyboard::G, 7}, {sf::Keyboard::Y, 8}, {sf::Keyboard::H, 9}, {sf::Keyboard::U, 10}, {sf::Keyboard::J, 11} };
    std::vector<Key> m_keys{ {"C1"}, {"C1S"}, {"D1"}, {"D1S"}, {"E1"}, {"F1"}, {"F1S"}, {"G1"}, {"G1S"}, {"A1"}, {"A1S"}, {"B1"} };
    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        for (auto& k : m_keys) {
            target.draw(k.m_rect);
        }
    }
public:
    Piano() {
        int index = 0;
        for (auto& k : m_keys) {
            k.load();
            bool isBlack = k.m_name.back() == 'S';
            if (isBlack) {
                // 黑键
                k.m_rect.setPosition({ (index - 0.5f) * m_keysize.x, 0.0f });
                k.m_rect.setSize({ m_keysize.x * 0.5f, m_keysize.y * 0.6f });
                k.m_rect.setFillColor(sf::Color::Black);
            } else {
                // 白键
                k.m_rect.setPosition({ index * m_keysize.x, 0.0f });
                k.m_rect.setSize(m_keysize);
                k.m_rect.setFillColor(sf::Color::White);
                index++;
            }
            size_t r = std::hash<std::string>().operator()(k.m_name);
            k.m_rect.setOutlineColor(sf::Color(r & 0xff, (r >> 16) & 0xff, (r >> 24) & 0xff, isBlack ? 218 : 128));

        }
    }
    void onKeyPress(sf::Keyboard::Key key) {
        // 设置显示高光 & 播放音乐
        if (m_mapping.find(key) != m_mapping.end()) {
            // 不会连续重复按
            if (!m_keys[m_mapping[key]].m_highlight) {
                m_keys[m_mapping[key]].press();
            }
        }
    }
    void onKeyRelease(sf::Keyboard::Key key) {
        if (m_mapping.find(key) != m_mapping.end()) {
            m_keys[m_mapping[key]].release();
        }
    }
    void onMousePress(int x, int y) {
        std::cout << x << ", " << y << std::endl;
        for (int i = m_keys.size() - 1; i >= 0; i--) {
            auto& k = m_keys[i];
            sf::FloatRect rect(k.m_rect.getPosition(), k.m_rect.getSize());
            if (rect.contains((float)x, (float)y)) {
                k.press();
                return;
            }
        }
    }
    void onMouseRelease(int x, int y) {
        for (int i = m_keys.size() - 1; i >= 0; i--) {
            auto& k = m_keys[i];
            sf::FloatRect rect(k.m_rect.getPosition(), k.m_rect.getSize());
            if (rect.contains((float)x, (float)y)) {
                k.release();
                return;
            }
        }
    }
    sf::Vector2u getSize() {
        return { (unsigned)(m_keysize.x * 7), (unsigned)(m_keysize.y) };
    }
};

int main() {

    Piano piano;

    sf::RenderWindow window(sf::VideoMode(piano.getSize().x, piano.getSize().y), "piano", sf::Style::None);
    window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                piano.onKeyPress(event.key.code);
            }
            if (event.type == sf::Event::KeyReleased) {
                piano.onKeyRelease(event.key.code);
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                piano.onMousePress(event.mouseButton.x, event.mouseButton.y);
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                piano.onMouseRelease(event.mouseButton.x, event.mouseButton.y);
            }
        }


        window.clear();
        window.draw(piano);
        window.display();
    }
    return EXIT_SUCCESS;
}