#include "raylib.h"
#include<iostream>
#include<vector>

struct Edge { float sx, sy, ex, ey; };
struct Cell 
{
    int edge_id[4];
    bool edge_exist[4];
    bool exist = false;
};

#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3

const int screenWidth = 800;
const int screenHeight = 600;

const int cellSize = 20;

const int worldWidth = screenWidth / cellSize;
const int worldHeight = screenHeight / cellSize;

Cell* world = new Cell[worldWidth * worldHeight];

std::vector<Edge> vecEdges;
