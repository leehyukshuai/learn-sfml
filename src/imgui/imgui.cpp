#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include "utils.hpp"

int main(int argc, char **argv) {
    Utils::initialize(argv[0]);

    sf::RenderWindow window(sf::VideoMode(1280, 720), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
        return EXIT_FAILURE;

    ImGuiIO &io = ImGui::GetIO();
    std::string filepath = Utils::fetch("consola.ttf");
    ImFont *font = io.Fonts->AddFontFromFileTTF(filepath.c_str(), 28.0f);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::UpdateFontTexture();
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::PushFont(font);
        ImGui::ShowDemoWindow();
        ImGui::PopFont();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}