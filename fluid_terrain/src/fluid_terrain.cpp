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
    
    interaction_equal = 0.7;
    interaction_unequal = 1.0;
    
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

double FluidTerrain::potential(unsigned int i, unsigned int j){
    double potential = 0;
    for (int k = -10; k <= 10; k++){
        for (int l = -10; l <= 10; l++){
            if (k+i > 10 and k+i < chunk.chunkSize-10 and l+j > 10 and l+j < chunk.chunkSize-10){
                if (chunk.terrain[i][j] == chunk.terrain[i+k][j+l])
                    potential += interaction_equal;
                else
                    potential += interaction_unequal;
            }
        }
    }
    return potential;
}

void FluidTerrain::update(){
    unsigned int length_x, length_y, i, j;
    unsigned int chunkSize = chunk.chunkSize;
    unsigned int steps = 100000;
    TerrainType terrain_tmp;
    
    for (int n = 0; n < steps; n++){
        i = rand()%chunkSize, j = rand()%chunkSize;
        
        length_x = rand()%2-1;
        length_y = rand()%2-1;
        
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
