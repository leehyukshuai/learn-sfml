#pragma once
#include "Model.hpp"
#include "Renderer.hpp"
#include "Application.hpp"

class Controler {
    Model &m_model;
    Renderer &m_renderer;
    Application &m_application;

public:
    Controler(Model &model, Renderer &renderer, Application &application);
    void render();

    void pause();
};