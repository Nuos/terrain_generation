#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include "fluid_terrain.h"

using namespace sf;

FluidTerrain::FluidTerrain(unsigned int chunkSize){
    srand(time(NULL));

    // Initialize chunk
    chunk.chunkSize = chunkSize;
    chunk.terrain = new TerrainType* [chunkSize];
    for (int i = 0; i < chunkSize; i++){
        chunk.terrain[i] = new TerrainType [chunkSize];
            for (int j = 0; j < chunkSize; j++)
                chunk.terrain[i][j] = TerrainType(rand()%3);
    }

    // Initialize window
    height = 800;
    width  = 800;
    window.create(VideoMode(width, height), "Fluid terrain", Style::Close);

}

void FluidTerrain::loop(){
    while (window.isOpen()){
        window.clear(Color::Black);
        this->input();
        this->update();
        this->draw();
        window.display();
    }

}

void FluidTerrain::input(){
    if (Keyboard::isKeyPressed(Keyboard::Escape))
        window.close();
}
unsigned int FluidTerrain::getDir(unsigned int i, unsigned int j){
    unsigned int dir;
    if (i == 0){
        if (j == 0)
            dir = rand()%3;     // 0-2
        else if (j == chunk.chunkSize - 1)
            dir = rand()%3+2;   // 2-4
        else
            dir = rand()%5;     // 0-4
    }
    else if (i == chunk.chunkSize-1){
        if (j == 0)
            dir = rand()%3+6;   // 6-8
        else if (j == chunk.chunkSize - 1)
            dir = rand()%3+4;   // 4-6
        else
            dir = rand()%5+4;   // 4-8
    }
    else if (j == 0){
        dir = rand()%5;         // 0-4
        if (j == 0 and dir > 2)
            dir += 3;           
    }
    else if (j == chunk.chunkSize - 1)
        dir = rand()%5+2;       // 2-6
    else
        dir = rand()%8;         // 0-7
    if (dir == 8)
        dir = 0;
    //printf("%d\n", dir);
    return dir;
}

unsigned int FluidTerrain::numNeighbours(unsigned int i, unsigned int j){
    unsigned int neighbours = 0;
    for (int k = -1; k <= 1; k++){
        for (int l = -1; l <= 1; l++){
            if (k+i > 0 and k+i < chunk.chunkSize-1 and l+j > 0 and l+j < chunk.chunkSize-1){
                if (chunk.terrain[i][j] = chunk.terrain[i+k][j+l]){
                    neighbours++;
                }
            }
        }
    }
    //printf("%d\n", neighbours);
    return neighbours;
}

void FluidTerrain::swap(TerrainType *a, TerrainType *b){
    TerrainType terrain_tmp;
    terrain_tmp = *a;
    *a = *b;
    *b = terrain_tmp;
}

void FluidTerrain::update(){
    unsigned int dir;
    unsigned int chunkSize = chunk.chunkSize;
    unsigned int steps = 10000;

    for (int n = 0; n < steps; n++){
        unsigned int attempts = 0;
        int i = rand()%chunkSize, j = rand()%chunkSize;
        bool move = false;
        
        while(!move){
            dir = this->getDir(i, j);
            if (dir = 0){
                if (this->numNeighbours(i, j) < this->numNeighbours(i, j+1)){
                    this->swap(&chunk.terrain[i][j], &chunk.terrain[i][j+1]);
                    move = true;
                }
            }
            else if (dir = 1){
                if (this->numNeighbours(i, j) < this->numNeighbours(i+1, j+1)){
                    this->swap(&chunk.terrain[i][j], &chunk.terrain[i+1][j+1]);
                    move = true;
                }
            }
            else if (dir = 2){
                if (this->numNeighbours(i, j) < this->numNeighbours(i+1, j)){
                    this->swap(&chunk.terrain[i][j], &chunk.terrain[i+1][j]);
                    move = true;
                }
            }
            else if (dir = 3){
                if (this->numNeighbours(i, j) < this->numNeighbours(i+1, j-1)){
                    this->swap(&chunk.terrain[i][j], &chunk.terrain[i+1][j-1]);
                    move = true;
                }
            }
            else if (dir = 4){
                if (this->numNeighbours(i, j) < this->numNeighbours(i, j-1)){
                    this->swap(&chunk.terrain[i][j], &chunk.terrain[i][j-1]);
                    move = true;
                }
            }
            else if (dir = 5){
                if (this->numNeighbours(i, j) < this->numNeighbours(i-1, j-1)){
                    this->swap(&chunk.terrain[i][j], &chunk.terrain[i-1][j-1]);
                    move = true;
                }
            }
            else if (dir = 6){
                if (this->numNeighbours(i, j) < this->numNeighbours(i-1, j)){
                    this->swap(&chunk.terrain[i][j], &chunk.terrain[i-1][j]);
                    move = true;
                }
            }
            else if (dir = 7){
                if (this->numNeighbours(i, j) < this->numNeighbours(i-1, j+1)){
                    this->swap(&chunk.terrain[i][j], &chunk.terrain[i-1][j+1]);
                    move = true;
                }
            }
            attempts ++;
            if (attempts > 10){
                move = true;
            }
        }
    }
}

void FluidTerrain::draw(){
    RectangleShape cell;
    double tileSize = (double)width/(double)chunk.chunkSize;
    for (int i = 0; i < chunk.chunkSize; i++){
        for (int j = 0; j < chunk.chunkSize; j++){
            cell.setPosition(i*tileSize, j*tileSize);
            cell.setSize(Vector2f(tileSize, tileSize));
            
            if (chunk.terrain[i][j] == forest)
                cell.setFillColor(Color(255, 0, 0, 255));
            else if (chunk.terrain[i][j] == desert)
                cell.setFillColor(Color(0, 255, 0, 255));
            else if (chunk.terrain[i][j] == plain)
                cell.setFillColor(Color(0, 0, 255, 255));
            
            window.draw(cell);
        }
    }
}
