#pragma once
#include <SFML/Graphics.hpp>
#include "EventManager.hpp"
#include "RenderContext.hpp"

// A basic wrapper of RenderContext and EventManager.
class WindowContext {
private:
    sf::RenderWindow m_window;
    RenderContext m_renderContext;
    EventManager m_eventHandler;

public:
    WindowContext(const std::string& window_name, sf::Vector2u window_size, uint32_t window_style = sf::Style::Default);

    bool run();
    RenderContext& getRenderContext();
    EventManager& getEventManager();
    sf::RenderWindow& getWindow();
};