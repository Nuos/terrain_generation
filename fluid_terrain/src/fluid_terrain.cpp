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
                chunk.terrain[i][j] = TerrainType(rand()%2);
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
    int a, b;
    double potential = 0;
    int chunkSize = chunk.chunkSize;
    int range = 20;
    for (int k = -range; k <= range; k++){
        for (int l = -range; l <= range; l++){
            a = k+i;
            b = l+j; 
            
            // Periodic boundary 
            if (a < 0)
                a += chunkSize;
            else if (a > chunkSize-1)
                a -= chunkSize;
            if (b < 0)
                b += chunkSize;
            else if (b > chunkSize-1)
                b -= chunkSize;

            if (chunk.terrain[i][j] == chunk.terrain[a][b])
                potential += interaction_equal;
            else
                potential += interaction_unequal;
        }
    }
    return potential;
}

void FluidTerrain::update(){
    int new_i, new_j, i, j;
    int chunkSize = chunk.chunkSize;
    unsigned int steps = 10000;
    TerrainType terrain_tmp;

    int range = 10;

    for (int n = 0; n < steps; n++){
        i = rand()%chunkSize, j = rand()%chunkSize;
        
        new_i = i + rand()%(range*2+1)-range;
        new_j = j + rand()%(range*2+1)-range;
       
        //printf("i: %d\tj: %d\t", new_i, new_j);

        // Periodic boundary 
        if (new_i < 0)
            new_i += chunkSize;
        else if (new_i > chunkSize-1)
            new_i -= chunkSize;
        if (new_j < 0)
            new_j += chunkSize;
        else if (new_j > chunkSize-1)
            new_j -= chunkSize;

        //printf("new_i: %d\tnew_j: %d\n", new_i, new_j);
        
        if (this->potential(i, j) < this->potential(new_i, new_j))
            std::swap(chunk.terrain[i][j], chunk.terrain[new_i][new_j]);
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
