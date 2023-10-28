#include "effects.hpp"
#include <imgui-SFML.h>
#include <utils.hpp>

int main(int argc, char* argv[]) {
    Utils::initialize(argv[0]);
    
    sf::Texture texture;
    texture.loadFromFile(Utils::fetch("field_small.jpg"));

    sf::RenderWindow window;
    window.create(sf::VideoMode(texture.getSize().x, texture.getSize().y), "effects", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    int index = 0;
    std::vector<Effect*> effects = {
        new Blur, new Edge, new Separate
    };
    for (auto e : effects) {
        if (!e->load(texture))
            return EXIT_FAILURE;
    }

    sf::Sprite sprite(texture);

    if (!ImGui::SFML::Init(window))
        return EXIT_FAILURE;
    ImGui::GetIO().FontGlobalScale = 2.0f;
    ImGui::GetStyle().ScaleAllSizes(1.5f);
    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::Left) {
                    index = (index - 1 + effects.size()) % effects.size();
                }
                if (event.key.code == sf::Keyboard::Right) {
                    index = (index + 1) % effects.size();
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        effects[index]->update();

        // window.clear();
        window.draw(sprite, effects[index]->getShader());
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return EXIT_SUCCESS;
}