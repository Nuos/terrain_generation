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
    
    interaction_equal = 0.5;
    interaction_unequal = 10.0;
    
    temp = 10;

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
        if (temp != 0)
            temp --;
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

double FluidTerrain::potential(unsigned int i, unsigned int j){
    double potential = 0;
    for (int k = -10; k <= 10; k++){
        for (int l = -10; l <= 10; l++){
            if (k+i > 10 and k+i < chunk.chunkSize-11 and l+j > 10 and l+j < chunk.chunkSize-11){
                //printf ("k+i %d\tl+j %d\tpotential %f\n", k+i, l+j, potential);
                if (chunk.terrain[i][j] == chunk.terrain[i+k][j+l])
                    potential += interaction_equal;
                else
                    potential += interaction_unequal;
            }
        }
    }
    return potential;

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
    unsigned int length_x, length_y, i, j;
    unsigned int chunkSize = chunk.chunkSize;
    unsigned int steps = 10000;
    TerrainType terrain_tmp;

    for (int n = 0; n < steps; n++){
        i = rand()%chunkSize, j = rand()%chunkSize;
        
        if (temp != 0){
            length_x = rand()%temp*2+1-temp;
            length_y = rand()%temp*2+1-temp;
        }
        else {
            length_x = 1; 
            length_y = 1;
        }
        if (this->potential(i, j) < this->potential(i+length_x, j+length_y))
            std::swap(chunk.terrain[i][j], chunk.terrain[i+length_x][j+length_y]);
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
