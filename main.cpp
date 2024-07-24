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
//                          starting x,y width ,height ,
void ConverttileMapToPolyMap(int sx,int sy,int w,int h,float cellSize,int pitch)
{
    vecEdges.clear();
    for (int x = 0; x < w; x++) 
    {
        for (int y = 0; y < h; y++)
        {
            for (int j = 0; j < 4; j++)
            {
                world[(y + sy) * pitch + (x + sx)].edge_exist[j] = false;;
                world[(y + sy) * pitch + (x + sx)].edge_id[j] = 0;
            }
        }
         
        for(int x=1;x<w-1;x++)
        {   for(int y=1;y<h-1;y++)
            {
            //converting 2d to 1d
            int i = (y + sy) * pitch + (x + sy);    //current cell
            int n = (y + sy - 1) * pitch + (x + sx);//norhtern neighbor
            int s = (y + sy +1) * pitch + (x + sx); //southern neighbor
            int w = (y + sy ) * pitch + (x + sx-1); //westeern nighbor
            int e = (y + sy ) * pitch + (x + sx+1); //eastern neighbor
            
            //check if the cell exist
            if(world[i].exist)
            {   

                //if this cell has no western neighbor,it needsa western edge
                if(!world[w].exist)
                {   // It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.

                    if (world[n].edge_exist[WEST]) // Norhtern neighbor has a western edge,so it grows downwards
                    {
                        vecEdges[world[n].edge_id[WEST]].ey += cellSize; //increasing the y axis  size to 1 block width down
                        world[i].edge_id[WEST] = world[n].edge_id[WEST];
                        world[i].edge_exist[WEST] = true;
                    }
                    
                    else 
                    {
                        Edge edge;
                        edge.sx = (sx + x) * cellSize;
                        edge.sy = (sy + y) * cellSize;

                        edge.ex = edge.sx ;
                        edge.ey = edge.sy + cellSize;
                        int edge_id = vecEdges.size();

                        vecEdges.push_back(edge);

                        world[i].edge_id[WEST] = edge_id;
                        world[i].edge_exist[WEST] = true;

                    }
                
                }



                //if this cell has no eastern neighbor,it needsa eastern edge
                if (!world[e].exist)
                {   // It can either extend it from its northern neighbour if they have
                    // one, or It can start a new one.

                    if (world[n].edge_exist[EAST]) // Norhtern neighbor has an eastern  edge,so it grows downwards
                    {
                        vecEdges[world[n].edge_id[EAST]].ey += cellSize; //increasing the y axis  size to 1 block width down
                        world[i].edge_id[EAST] = world[n].edge_id[EAST];
                        world[i].edge_exist[EAST] = true;
                    }

                    else
                    {
                        Edge edge;
                        edge.sx = (sx + x+1) * cellSize;
                        edge.sy = (sy + y) * cellSize;

                        edge.ex = edge.sx;
                        edge.ey = edge.sy + cellSize;
                        int edge_id = vecEdges.size();

                        vecEdges.push_back(edge);

                        world[i].edge_id[EAST] = edge_id;
                        world[i].edge_exist[EAST] = true;

                    }

                }




                //if this cell has no northern neighbor,it needs  a noethern edge
                if (!world[n].exist)
                {   // It can either extend it from its northern neighbour if they have
                    // one, or It can start a new one.

                    if (world[w].edge_exist[NORTH]) // Norhtern neighbor has a western edge,so it grows downwards
                    {
                        vecEdges[world[w].edge_id[NORTH]].ex+= cellSize; //increasing the y axis  size to 1 block width down
                        world[i].edge_id[NORTH] = world[w].edge_id[NORTH];
                        world[i].edge_exist[NORTH] = true;
                    }

                    else
                    {
                        Edge edge;
                        edge.sx = (sx + x) * cellSize;
                        edge.sy = (sy + y) * cellSize;

                        edge.ex = edge.sx + cellSize;
                        edge.ey = edge.sy;
                        int edge_id = vecEdges.size();

                        vecEdges.push_back(edge);

                        world[i].edge_id[NORTH] = edge_id;
                        world[i].edge_exist[NORTH] = true;

                    }

                }




                //if this cell has no southern neighbor,it needsa southern  edge
                if (!world[s].exist)
                {   // It can either extend it from its western neighbour if they have
                    // one, or It can start a new one.

                    if (world[w].edge_exist[SOUTH]) //if  western neighbor has a southern edge,so it grows downwards
                    {
                        vecEdges[world[w].edge_id[SOUTH]].ex += cellSize; //increasing the x axis  size to 1 block width down
                        world[i].edge_id[SOUTH] = world[w].edge_id[SOUTH];
                        world[i].edge_exist[SOUTH] = true;
                    }

                    else
                    {
                        Edge edge;
                        edge.sx = (sx + x) * cellSize;
                        edge.sy = (sy + y+1) * cellSize;

                        edge.ex = edge.sx + cellSize;
                        edge.ey = edge.sy ;
                        int edge_id = vecEdges.size();

                        vecEdges.push_back(edge);

                        world[i].edge_id[SOUTH] = edge_id;
                        world[i].edge_exist[SOUTH] = true;

                    }

                }

            }



            }
        
        
        }
        


    }

}


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
    
            ConverttileMapToPolyMap(0, 0, worldWidth, worldHeight, cellSize, worldWidth);
      


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
