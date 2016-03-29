#include <SFML/Graphics.hpp>
#include <algorithm>            // std::swap
#include <stdlib.h>             // rand()
#include <time.h>               // time()
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
        //this->count();
        this->draw();
        this->input();
        this->update();
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
                if (chunk.terrain[i][j] == chunk.terrain[i+k][j+l])
                    neighbours++;
            }
        }
    }
    //printf("%d\n", neighbours);
    return neighbours;
}

void FluidTerrain::count(){
    int numForest = 0, numDesert = 0, numPlain = 0;
    for (int i = 0; i < chunk.chunkSize; i++){
        for (int j = 0; j < chunk.chunkSize; j++){
            if (chunk.terrain[i][j] == forest)
                numForest++;
            if (chunk.terrain[i][j] == desert)
                numDesert++;
            if (chunk.terrain[i][j] == plain)
                numPlain++;
        }
    }
    printf("%d\t%d\t%d\n", numForest, numDesert, numPlain);
}

void FluidTerrain::update(){
    unsigned int dir, attempts, i, j, a, b;
    unsigned int chunkSize = chunk.chunkSize;
    unsigned int steps = 1;
    bool move;
    TerrainType terrain_tmp;

    for (int n = 0; n < steps; n++){
        attempts = 0;
        i = rand()%chunkSize, j = rand()%chunkSize;
        move = false;
        
        while(!move){
            dir = this->getDir(i, j);
            if (dir = 0 and this->numNeighbours(i, j) < this->numNeighbours(i, j+1)){
                a = 0;
                b = 1;
                move = true;
            }
            else if (dir = 1 and this->numNeighbours(i, j) < this->numNeighbours(i+1, j+1)){
                a = 1;
                b = 1;
                move = true;
            }
            else if (dir = 2 and this->numNeighbours(i, j) < this->numNeighbours(i+1, j)){
                a = 1;
                b = 0;
                move = true;
            }
            else if (dir = 3 and this->numNeighbours(i, j) < this->numNeighbours(i+1, j-1)){
                a = 1;
                b = -1;
                move = true;
            }
            else if (dir = 4 and this->numNeighbours(i, j) < this->numNeighbours(i, j-1)){
                a = 0;
                b = -1;
                move = true;
            }
            else if (dir = 5 and this->numNeighbours(i, j) < this->numNeighbours(i-1, j-1)){
                a = -1;
                b = -1;
                move = true;
            }
            else if (dir = 6 and this->numNeighbours(i, j) < this->numNeighbours(i-1, j)){
                a = -1;
                b = 0;
                move = true;
            }
            else if (dir = 7 and this->numNeighbours(i, j) < this->numNeighbours(i-1, j+1)){
                a = -1;
                b = 1;
                move = true;
            }

            if (move == true)
                std::swap(chunk.terrain[i][j], chunk.terrain[i+a][j+b]);

            attempts ++;
            if (attempts > 100){
                move = true;
            }
        }
    }
}

void FluidTerrain::draw(){
    double tileSize = (double)width/(double)chunk.chunkSize;
    for (int i = 0; i < chunk.chunkSize; i++){
        for (int j = 0; j < chunk.chunkSize; j++){
            RectangleShape cell;
            cell.setPosition(i*tileSize, j*tileSize);
            cell.setSize(Vector2f(tileSize, tileSize));
            
            if (chunk.terrain[i][j] == forest)
                cell.setFillColor(Color(255, 0, 0, 255));
            else if (chunk.terrain[i][j] == desert)
                cell.setFillColor(Color(0, 255, 0, 255));
            else if (chunk.terrain[i][j] == plain)
                cell.setFillColor(Color(0, 0, 255, 255));
            else if (chunk.terrain[i][j] == tom)
                cell.setFillColor(Color(0, 0, 0, 255)); 
    
            window.draw(cell);
        }
    }
}
