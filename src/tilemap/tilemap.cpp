#include <SFML/Graphics.hpp>
#include "utils.hpp"

static const float blockSize = 16.0f, scaleFactor = 8.0f;

class TileMap: public sf::Drawable, public sf::Transformable {
private:
    sf::Texture m_texture;
    sf::VertexArray m_vertices;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.texture = &m_texture;
        states.transform *= getTransform();
        target.draw(m_vertices, states);
    }

public:
    enum TileType {
        Void, Dirt, Bricks, GoldenBlock, LuckyBlock, MagicPotion
    };
    bool load(const std::string& tileset, const int* map, unsigned int height, unsigned int width) {
        if (!m_texture.loadFromFile(tileset))
            return false;
        m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        m_vertices.resize(height * width * 6);
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {

                int tile = map[i * width + j];
                if (tile != TileType::Void) {
                    int u, v;
                    if (tile == TileType::Dirt) {
                        bool left = false, right = false, up = false, down = false;
                        if (j == 0 || map[i * width + j - 1] == 1) {
                            left = true;
                        }
                        if (j == width - 1 || map[i * width + j + 1] == 1) {
                            right = true;
                        }
                        if (left && right) {
                            v = 1;
                        } else if (left) {
                            v = 2;
                        } else if (right) {
                            v = 0;
                        } else {
                            v = 3;
                        }
                        if (i == 0 || map[(i - 1) * width + j] == 1) {
                            up = true;
                        }
                        if (i == height - 1 || map[(i + 1) * width + j] == 1) {
                            down = true;
                        }
                        if (up && down) {
                            u = 1;
                        } else if (up) {
                            u = 2;
                        } else if (down) {
                            u = 0;
                        } else {
                            u = 3;
                        }
                    } else if (tile == TileType::Bricks) {
                        u = 1;
                        v = 4;
                    } else if (tile == TileType::GoldenBlock) {
                        u = 3;
                        v = 4;
                    } else if (tile == TileType::LuckyBlock) {
                        u = 0;
                        v = 4;
                    } else if (tile == TileType::MagicPotion) {
                        u = 2;
                        v = 4;
                    }

                    sf::Vertex* triangles = &m_vertices[(i * width + j) * 6];
                    triangles[0].position = { j * blockSize * scaleFactor, i * blockSize * scaleFactor };
                    triangles[1].position = { (j + 1) * blockSize * scaleFactor, i * blockSize * scaleFactor };
                    triangles[2].position = { j * blockSize * scaleFactor, (i + 1) * blockSize * scaleFactor };
                    triangles[3].position = { (j + 1) * blockSize * scaleFactor, i * blockSize * scaleFactor };
                    triangles[4].position = { j * blockSize * scaleFactor, (i + 1) * blockSize * scaleFactor };
                    triangles[5].position = { (j + 1) * blockSize * scaleFactor, (i + 1) * blockSize * scaleFactor };

                    triangles[0].texCoords = { v * blockSize, u * blockSize };
                    triangles[1].texCoords = { (v + 1) * blockSize, u * blockSize };
                    triangles[2].texCoords = { v * blockSize, (u + 1) * blockSize };
                    triangles[3].texCoords = { (v + 1) * blockSize, u * blockSize };
                    triangles[4].texCoords = { v * blockSize, (u + 1) * blockSize };
                    triangles[5].texCoords = { (v + 1) * blockSize, (u + 1) * blockSize };
                }
            }
        }
        return true;
    }

};

int main(int argc, char **argv) {
    Utils::initialize(argv[0]);

    unsigned int width = 12, height = 7;
    int map[] = {
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
        0, 0, 0, 2, 4, 2, 0, 1, 5, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
        1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1,
        1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    };

    sf::RenderWindow window(sf::VideoMode(width * blockSize * scaleFactor, height * blockSize * scaleFactor), "tilemap");

    TileMap tileMap;
    if (!tileMap.load(Utils::fetch("tileset.png"), map, height, width))
        return EXIT_FAILURE;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color(210, 220, 200));
        window.draw(tileMap);
        window.display();
    }
    return EXIT_SUCCESS;
}