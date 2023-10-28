#include <SFML/Graphics.hpp>
#include <cmath>

class ParticleSystem: public sf::Drawable, public sf::Transformable {
private:
    struct Particle {
        sf::Vector2f velocity;
        sf::Time lifetime;
    };

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    sf::Vector2f m_emitter;
    sf::Time m_lifetime;
    float m_speed;

    void resetParticle(unsigned int index) {
        Particle& p = m_particles[index];
        float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
        p.velocity = { std::cos(angle) * (float)(std::rand() % 100 + 50) / 100.0f * m_speed,
                       std::sin(angle) * (float)(std::rand() % 100 + 50) / 100.0f * m_speed };
        p.lifetime = (float)(std::rand() % 100) / 100.0f * m_lifetime;
        m_vertices[index].position = m_emitter;
        m_vertices[index].color = { (sf::Uint8)(rand() % 128 + 128),
                                    (sf::Uint8)(rand() % 128 + 128),
                                    (sf::Uint8)(rand() % 128 + 128) };
    };

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        states.texture = NULL;
        target.draw(m_vertices, states);
    }
public:
    ParticleSystem(sf::Vector2f position = { 0.0f, 0.0f }, unsigned count = 10000, float speed = 30.0f, sf::Time lifetime = sf::seconds(10.0f))
        : m_emitter(position), m_vertices(sf::Points, count), m_particles(count), m_speed(speed), m_lifetime(lifetime) {}

    void update(sf::Time elapsed) {
        unsigned count = m_particles.size();
        for (unsigned index = 0; index < count; ++index) {
            if (m_particles[index].lifetime < sf::Time::Zero) {
                resetParticle(index);
            }

            m_vertices[index].position += m_particles[index].velocity * elapsed.asSeconds();
            m_vertices[index].color.a = (sf::Uint8)((m_particles[index].lifetime / m_lifetime) * 255);
            m_particles[index].lifetime -= elapsed;
        }
    }
};

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 800), "particleSystem");
    window.setVerticalSyncEnabled(true);

    sf::View view = window.getDefaultView();
    view.setCenter(0, 0);
    window.setView(view);

    ParticleSystem particleSystem;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::Resized) {
                view.setSize(event.size.width, event.size.height);
                window.setView(view);
            }
        }

        particleSystem.update(clock.restart());

        window.clear();

        window.draw(particleSystem);

        window.display();
    }
    return EXIT_SUCCESS;
}
