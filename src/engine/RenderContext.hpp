#pragma once
#include <SFML/Graphics.hpp>
#include "EventManager.hpp"
#include <iostream>

class RenderContext {
    sf::RenderWindow& m_window;
    sf::View m_view;

    bool m_direct = false;
    bool m_update = false;

    sf::Vector2f m_position;
    bool m_clicking = false;

public:
    RenderContext(sf::RenderWindow& window) :
        m_window(window),
        m_view(window.getDefaultView()) {}

    void draw(const sf::Drawable& drawable, const sf::RenderStates& render_states = {}) {
        if (m_direct || m_update) {
            m_window.setView(m_view);
            m_direct = false;
        }
        m_window.draw(drawable, render_states);
    }

    void drawDirect(const sf::Drawable& drawable, const sf::RenderStates& render_states = {}) {
        if (!m_direct) {
            m_window.setView(m_window.getDefaultView());
            m_direct = true;
        }
        m_window.draw(drawable, render_states);
    }

    void clear(sf::Color color = sf::Color::Black) {
        m_window.clear(color);
    }

    void display() {
        m_window.display();
    }

    void registerCallbacks(EventManager& eventManager) {
        eventManager.addEventCallback(sf::Event::Closed, [&](sfev::CstEv) {m_window.close();});
        eventManager.addKeyPressedCallback(sf::Keyboard::Escape, [&](sfev::CstEv) {m_window.close();});
        eventManager.addMousePressedCallback(sf::Mouse::Left, [&](sfev::CstEv e) {click(e.mouseButton.x, e.mouseButton.y);});
        eventManager.addMouseReleasedCallback(sf::Mouse::Left, [&](sfev::CstEv) {unclick();});
        eventManager.addEventCallback(sf::Event::MouseMoved, [&](sfev::CstEv e) {move(e.mouseMove.x, e.mouseMove.y);});
        eventManager.addEventCallback(sf::Event::MouseWheelScrolled, [&](sfev::CstEv e) {zoom(e.mouseWheelScroll.delta);});
    }

    void click(int x, int y) {
        sf::Vector2f pos((float)x, (float)y);
        m_position = pos;
        m_clicking = true;
    }

    void unclick() {
        m_clicking = false;
    }

    void move(int x, int y) {
        if (m_clicking) {
            sf::Vector2f pos((float)x, (float)y);
            auto delta = pos - m_position;
            m_view.move(-delta);
            m_position = pos;
            m_update = true;
        }
    }

    void zoom(float delta) {
        m_view.zoom(delta > 0 ? 1 / 1.2f : 1.2f);
        m_update = true;
    }
};