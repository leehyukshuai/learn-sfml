#pragma once
#include "RenderContext.hpp"
#include "Model.hpp"

class Renderer {
private:
    RenderContext& m_context;
    Model& m_model;

    bool m_showSticks;

    void renderPath() const;
    void renderSticks() const;
public:
    Renderer(Model& model, RenderContext& context);
    void render() const;

    bool getShowSticks() const;
    void setShowSticks(bool showSticks);
};
