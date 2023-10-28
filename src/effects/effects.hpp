#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <cmath>
#include <utils.hpp>

class Effect {
protected:
    sf::Shader m_shader;
public:
    virtual bool load(const sf::Texture& texture) {
        m_shader.setUniform("texture", texture);
        return true;
    };
    virtual void update() = 0;
    const sf::Shader* getShader() const {
        return &m_shader;
    };
};

class Blur: public Effect {
    float m_offset;
public:
    virtual bool load(const sf::Texture& texture) override {
        Effect::load(texture);
        if (!m_shader.loadFromFile(Utils::fetch( "blur.frag"), sf::Shader::Fragment))
            return false;
        m_offset = 0.004f;
        m_shader.setUniform("offset", m_offset);
        return true;
    }
    virtual void update() override {
        ImGui::Begin("Settings");
        ImGui::DragFloat("offset", &m_offset, 0.001f, 0.0f, 0.4f);
        ImGui::End();

        m_shader.setUniform("offset", m_offset);
    }
};

class Edge: public Effect {
    float m_offset;
    bool m_auto;
    sf::Clock m_clock;
    sf::Time m_elapsed;
public:
    virtual bool load(const sf::Texture& texture) override {
        Effect::load(texture);
        if (!m_shader.loadFromFile(Utils::fetch( "edge.frag"), sf::Shader::Fragment))
            return false;
        m_offset = 0.004f;
        m_auto = true;
        m_shader.setUniform("offset", m_offset);
        return true;
    }
    virtual void update() override {
        ImGui::Begin("Settings");
        ImGui::DragFloat("offset", &m_offset, 0.001f, 0.0f, 0.4f);
        if (ImGui::Checkbox("Auto", &m_auto)) {
            m_clock.restart();
        }
        ImGui::End();

        if (m_auto) {
            m_elapsed += m_clock.restart();
            m_offset = (std::sin(m_elapsed.asSeconds()) + 1) / 5.0f;
        }
        m_shader.setUniform("offset", m_offset);
    }
};

class Separate: public Effect {
    float m_offset;
public:
    virtual bool load(const sf::Texture& texture) override {
        Effect::load(texture);
        if (!m_shader.loadFromFile(Utils::fetch( "separate.frag"), sf::Shader::Fragment))
            return false;
        m_offset = 0.01f;
        m_shader.setUniform("offset", m_offset);
        return true;
    }
    virtual void update() override {
        ImGui::Begin("Settings");
        ImGui::DragFloat("offset", &m_offset, 0.001f, 0.0f, 0.4f);
        ImGui::End();

        m_shader.setUniform("offset", m_offset);
    }
};