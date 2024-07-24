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

void ConvertTileMapToPolyMap(int sx, int sy, int w, int h, float fBlockWidth, int pitch)
{
	// Clear "PolyMap"
	vecEdges.clear();

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			for (int j = 0; j < 4; j++)
			{
				world[(y + sy) * pitch + (x + sx)].edge_exist[j] = false;
				world[(y + sy) * pitch + (x + sx)].edge_id[j] = 0;
			}

	// Iterate through region from top left to bottom right
	for (int x = 1; x < w - 1; x++)
		for (int y = 1; y < h - 1; y++)
		{
			// Create some convenient indices
			int i = (y + sy) * pitch + (x + sx);			// This
			int n = (y + sy - 1) * pitch + (x + sx);		// Northern Neighbour
			int s = (y + sy + 1) * pitch + (x + sx);		// Southern Neighbour
			int w = (y + sy) * pitch + (x + sx - 1);	// Western Neighbour
			int e = (y + sy) * pitch + (x + sx + 1);	// Eastern Neighbour

			// If this cell exists, check if it needs edges
			if (world[i].exist)
			{
				// If this cell has no western neighbour, it needs a western edge
				if (!world[w].exist)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (world[n].edge_exist[WEST])
					{
						// Northern neighbour has a western edge, so grow it downwards
						vecEdges[world[n].edge_id[WEST]].ey += fBlockWidth;
						world[i].edge_id[WEST] = world[n].edge_id[WEST];
						world[i].edge_exist[WEST] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						Edge edge;
						edge.sx = (sx + x) * fBlockWidth; edge.sy = (sy + y) * fBlockWidth;
						edge.ex = edge.sx; edge.ey = edge.sy + fBlockWidth;

						// Add edge to Polygon Pool
						int edge_id = vecEdges.size();
						vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[WEST] = edge_id;
						world[i].edge_exist[WEST] = true;
					}
				}

				// If this cell dont have an eastern neignbour, It needs a eastern edge
				if (!world[e].exist)
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (world[n].edge_exist[EAST])
					{
						// Northern neighbour has one, so grow it downwards
						vecEdges[world[n].edge_id[EAST]].ey += fBlockWidth;
						world[i].edge_id[EAST] = world[n].edge_id[EAST];
						world[i].edge_exist[EAST] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						Edge edge;
						edge.sx = (sx + x + 1) * fBlockWidth; edge.sy = (sy + y) * fBlockWidth;
						edge.ex = edge.sx; edge.ey = edge.sy + fBlockWidth;

						// Add edge to Polygon Pool
						int edge_id = vecEdges.size();
						vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[EAST] = edge_id;
						world[i].edge_exist[EAST] = true;
					}
				}

				// If this cell doesnt have a northern neignbour, It needs a northern edge
				if (!world[n].exist)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (world[w].edge_exist[NORTH])
					{
						// Western neighbour has one, so grow it eastwards
						vecEdges[world[w].edge_id[NORTH]].ex += fBlockWidth;
						world[i].edge_id[NORTH] = world[w].edge_id[NORTH];
						world[i].edge_exist[NORTH] = true;
					}
					else
					{
						// Western neighbour does not have one, so create one
						Edge edge;
						edge.sx = (sx + x) * fBlockWidth; edge.sy = (sy + y) * fBlockWidth;
						edge.ex = edge.sx + fBlockWidth; edge.ey = edge.sy;

						// Add edge to Polygon Pool
						int edge_id = vecEdges.size();
						vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[NORTH] = edge_id;
						world[i].edge_exist[NORTH] = true;
					}
				}

				// If this cell doesnt have a southern neignbour, It needs a southern edge
				if (!world[s].exist)
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (world[w].edge_exist[SOUTH])
					{
						// Western neighbour has one, so grow it eastwards
						vecEdges[world[w].edge_id[SOUTH]].ex += fBlockWidth;
						world[i].edge_id[SOUTH] = world[w].edge_id[SOUTH];
						world[i].edge_exist[SOUTH] = true;
					}
					else
					{
						// Western neighbour does not have one, so I need to create one
						Edge edge;
						edge.sx = (sx + x) * fBlockWidth; edge.sy = (sy + y + 1) * fBlockWidth;
						edge.ex = edge.sx + fBlockWidth; edge.ey = edge.sy;

						// Add edge to Polygon Pool
						int edge_id = vecEdges.size();
						vecEdges.push_back(edge);

						// Update tile information with edge information
						world[i].edge_id[SOUTH] = edge_id;
						world[i].edge_exist[SOUTH] = true;
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
    
		ConvertTileMapToPolyMap(0, 0, worldWidth, worldHeight, cellSize, worldWidth);
      


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
