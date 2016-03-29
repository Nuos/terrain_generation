#include <SFML/Graphics.hpp>

using namespace sf;

enum TerrainType {forest, desert, plain};

struct Chunk{
    TerrainType** terrain;
    unsigned int chunkSize;
};

class FluidTerrain{
private:
    Chunk chunk;
    RenderWindow window;
    int height, width;
public:
    FluidTerrain(unsigned int);

    unsigned int getDir(unsigned int, unsigned int);
    unsigned int numNeighbours(unsigned int, unsigned int);
    void swap(TerrainType*, TerrainType*);
    void loop();
    void input();
    void update();
    void draw();
};
