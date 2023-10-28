#include "Application.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Controler.hpp"
#include "utils.hpp"

const char* title = "Sticks";
const unsigned width = 1600, height = 800;


class App: public Application {
    Model m_model;
    Renderer m_render;
    Controler m_control;
private:
    void initialize() {
        m_model.setStartPosition({width*0.7f, height*0.5f});
        std::vector<Stick> sticks{
            Stick(100, 1.0f, true, sf::Color::Red),
            Stick(100, 1.0f, true, sf::Color::Green),
            Stick(100, 1.0f, true, sf::Color::Blue)
        };
        m_model.reset(sticks);
    }
public:
    App(const std::string& window_name, sf::Vector2u window_size, uint32_t window_style = sf::Style::Default) :
        Application(window_name, window_size, window_style),
        m_render(m_model, m_context),
        m_control(m_model, m_render, *this) {
        
        m_manager.addKeyPressedCallback(sf::Keyboard::Space, [&](sfev::CstEv){m_control.pause();});
        setFont(Utils::fetch("consola.ttf").c_str(), 32.0f, true);
        initialize();
    }

protected:
    void update(sf::Time dt) override {
        m_model.update(dt);
    };
    void render() override {
        m_render.render();
        m_control.render();
    };
};

int main(int argc, char **argv) {
    Utils::initialize(argv[0]);
    
    // auto full = sf::VideoMode::getDesktopMode();
    // App app(title, {full.width, full.height}, sf::Style::Fullscreen);

    App app(title, {width, height});

    app.run();
}