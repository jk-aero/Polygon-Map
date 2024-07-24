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

int main(void)
{

 

    InitWindow(screenWidth, screenHeight, "Raylib Basic Window Example");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {

        float mousex = GetMousePosition().x;
        float mousey = GetMousePosition().y;

        if (IsMouseButtonReleased(0))//setting tile map on or off
        {   //i=y*width+x converst 2d array to 1d array effectively
            int i = ((int)mousey / (int)cellSize) * worldWidth+((int)mousex/(int)cellSize);
            world[i].exist = !world[i].exist;
        }
       


        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //drawing blocks form tilemap
        for (int x = 0; x < worldWidth; x++)
        {
            for (int y = 0; y < worldHeight; y++)
            {
                if (world[y * worldWidth + x].exist)
                {
                    DrawRectangle(x*cellSize,y*cellSize,cellSize,cellSize,BLUE);
                }
            }

        }
        for (auto& e : vecEdges)
        {
            DrawLine(e.sx, e.sy, e.ex, e.ey, GREEN);
            DrawCircle(e.ex,e.ey,2.0f,RED);
            DrawCircle(e.sx,e.sy,2.0f,RED);
        }
        

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
