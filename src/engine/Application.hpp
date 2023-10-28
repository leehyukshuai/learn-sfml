#pragma once

#include "RenderContext.hpp"
#include "EventManager.hpp"
#include <imgui.h>

// An much more integrated wrapper of RenderContext and EventManager, with support for ImGui
class Application {
protected:
    // BUGFIX: must put m_window before m_context!
    // (because m_view of m_context need m_window to initialize.)
    sf::RenderWindow m_window;
    RenderContext m_context;
    EventManager m_manager;

    bool m_VSync;

protected:
    virtual void update(sf::Time dt) = 0;
    virtual void render() = 0;

public:
    Application(const std::string& window_name, sf::Vector2u window_size, uint32_t window_style);
    ~Application();

    bool getVSync();
    void setVSync(bool vsync);

    void setFont(const char *filepath, float font_pixels, bool enable_chinese = false);

    void run();
};