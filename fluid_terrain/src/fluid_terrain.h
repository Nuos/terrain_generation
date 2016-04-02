#include <SFML/Graphics.hpp>

using namespace sf;

enum TerrainType {forest, desert, plain, tom};

struct Chunk{
    TerrainType** terrain;
    unsigned int chunkSize;
};

class FluidTerrain{
private:
    Chunk chunk;
    RenderWindow window;
    int height, width;
    double interaction_equal, interaction_unequal;
public:
    FluidTerrain(unsigned int);

    unsigned int getDir(unsigned int, unsigned int);
    unsigned int numNeighbours(unsigned int, unsigned int);
    double potential(unsigned int, unsigned int);
    void count();
    void loop();
    void input();
    void update();
    void draw();
};
