#include "Application.hpp"
#include <imgui-SFML.h>

Application::Application(const std::string& window_name, sf::Vector2u window_size, uint32_t window_style) :
    m_window(sf::VideoMode(window_size.x, window_size.y), window_name, window_style),
    m_context(m_window),
    m_manager(m_window, true),
    m_VSync(true) {

    m_window.setVerticalSyncEnabled(m_VSync);
    m_context.registerCallbacks(m_manager);

    ImGui::SFML::Init(m_window);
}

Application::~Application() {
    ImGui::SFML::Shutdown(m_window);
}

bool Application::getVSync() {
    return m_VSync;
}

void Application::setVSync(bool vsync) {
    m_VSync = vsync;
    m_window.setVerticalSyncEnabled(m_VSync);
}

void Application::setFont(const char* filepath, float font_pixels, bool enable_chinese) {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF(filepath, font_pixels, nullptr, enable_chinese ? io.Fonts->GetGlyphRangesChineseSimplifiedCommon() : 0);
    ImGui::SFML::UpdateFontTexture();
}

void Application::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {

        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (!ImGui::GetIO().WantCaptureMouse ||
                event.type == sf::Event::KeyPressed ||
                event.type == sf::Event::KeyReleased) {
                m_manager.processEvent(event);
            }
            ImGui::SFML::ProcessEvent(event);
        }

        auto dt = clock.restart();
        // check if the window is freezed/blocked
        if (dt > sf::seconds(0.02f)) {
            continue;       // if so, we restart again
        }

        update(dt);
        ImGui::SFML::Update(m_window, dt);

        m_window.clear();
        render();
        ImGui::SFML::Render(m_window);
        m_window.display();
    }
}
