#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../BCGE.h"
#include "../modules/BCGE3D.h"

#define screenWidth 300
#define screenHeight 300
#define charWidth 2
#define charHeight 2
char title[20] = "mesh_renderer";

#define verticesInMesh 8
#define facesInMesh 12

Vector3 vertices[verticesInMesh] =
    {{1.000000, 1.000000, -1.000000},
     {1.000000, -1.000000, -1.000000},
     {1.000000, 1.000000, 1.000000},
     {1.000000, -1.000000, 1.000000},
     {-1.000000, 1.000000, -1.000000},
     {-1.000000, -1.000000, -1.000000},
     {-1.000000, 1.000000, 1.000000},
     {-1.000000, -1.000000, 1.000000}};

Face faces[facesInMesh] =
    {
        {5, 3, 1},
        {3, 8, 4},
        {7, 6, 8},
        {2, 8, 6},
        {1, 4, 2},
        {5, 2, 6},
        {5, 7, 3},
        {3, 7, 8},
        {7, 5, 6},
        {2, 4, 8},
        {1, 3, 4},
        {5, 1, 2}};

Camera camera =
    {
        {0, 0, 0},
        {0, 0, 0},
        M_PI / 2,
        1,
        100.0,
        0.1};

Vector3 displacement = {0, 0, 3};

Mesh cube;

void OnCreate()
{
    ConstructGameWindow(screenWidth, screenHeight, charWidth, charHeight, title);
    InitMesh(&cube, vertices, faces, "Cube", facesInMesh);
}

void OnUpdate(float elapsedTime)
{

    int K_Q = (int)'Q';
    int K_E = (int)'E';

    if (Keys[K_W].Pressed)
    {
        displacement.Y -= 1 * elapsedTime;
    }
    if (Keys[K_S].Pressed)
    {
        displacement.Y += 1 * elapsedTime;
    }
    if (Keys[K_A].Pressed)
    {
        displacement.X -= 1 * elapsedTime;
    }
    if (Keys[K_D].Pressed)
    {
        displacement.X += 1 * elapsedTime;
    }
    if (Keys[K_Q].Pressed)
    {
        displacement.Z -= 1 * elapsedTime;
    }
    if (Keys[K_E].Pressed)
    {
        displacement.Z += 1 * elapsedTime;
    }

    cube.Position = displacement;

    FillScreen(' ', Black, Black);

    RenderMesh(cube, camera);
}

int main(int argc, char **argv)
{
    GameRun(60);
    return 0;
}