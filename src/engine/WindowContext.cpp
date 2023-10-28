#include "WindowContext.hpp"

WindowContext::WindowContext(const std::string& window_name, sf::Vector2u window_size, uint32_t window_style) :
    m_window(sf::VideoMode(window_size.x, window_size.y), window_name, window_style),
    m_renderContext(m_window),
    m_eventHandler(m_window, true) {
    m_window.setVerticalSyncEnabled(true);
    m_renderContext.registerCallbacks(m_eventHandler);
}

bool WindowContext::run() {
    m_eventHandler.processEvents();
    return m_window.isOpen();
}

RenderContext& WindowContext::getRenderContext() {
    return m_renderContext;
}

EventManager& WindowContext::getEventManager() {
    return m_eventHandler;
}

sf::RenderWindow& WindowContext::getWindow() {
    return m_window;
}