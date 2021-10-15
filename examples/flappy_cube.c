#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../BCGE.h"

#define screenWidth 40
#define screenHeight 60
#define charWidth 10
#define charHeight 10
char title[20] = "flappy_cube";

// Cube
float gravity = 40;
float flapImpulse = 28;
Vector2 initialCubePosition = {2, screenWidth / 2};
Vector2 initialCubeSpeed = {6, 0};
int cubeSize = 3;

typedef struct
{
    Rect body;
    Vector2 speed;
} Cube;

void setCube(Cube *cube)
{
    cube->body = (Rect){initialCubePosition.X, initialCubePosition.Y, cubeSize, cubeSize};
    cube->speed = initialCubeSpeed;
}

// Pipes
int pipeLen = 28;
int pipeWidth = 5;
int pipeVerticalSeparation = 14;
int pipeHorizontalSeparation = 14;
int initialPipePosition = 20;
int negativeBound;
Vector2 pipeHeightRange = {-12, 0};
#define numberOfPipes 4

typedef struct
{
    Rect upper_pipe;
    Rect lower_pipe;
} Pipe;

void resetPipe(Pipe *pipe)
{
    int pipeHeight = RandomIntInRange(pipeHeightRange.X, pipeHeightRange.Y);
    pipe->upper_pipe = (Rect){screenWidth, pipeHeight, pipeWidth, pipeLen};
    pipe->lower_pipe = (Rect){screenWidth, pipeHeight + pipeLen + pipeVerticalSeparation, pipeWidth, pipeLen};
}

void setPipeList(Pipe *pipes)
{
    for (int i = 0; i < numberOfPipes; i++)
    {
        resetPipe(&pipes[i]);
        pipes[i].upper_pipe.posx = initialPipePosition + (pipeWidth + pipeHorizontalSeparation) * i;
        pipes[i].lower_pipe.posx = initialPipePosition + (pipeWidth + pipeHorizontalSeparation) * i;
    }
}

void drawPipe(Pipe pipe)
{
    DrawRect(' ', pipe.lower_pipe, Green, Green, 1);
    DrawRect(' ', pipe.upper_pipe, Green, Green, 1);
}

void drawPipeList(Pipe *pipes)
{
    for (int i = 0; i < numberOfPipes; i++)
    {
        drawPipe(pipes[i]);
    }
}

void updatePipeList(Pipe *pipes, float displacement)
{
    for (int i = 0; i < numberOfPipes; i++)
    {
        pipes[i].upper_pipe.posx -= displacement;
        pipes[i].lower_pipe.posx -= displacement;

        if (pipes[i].lower_pipe.posx < negativeBound)
            resetPipe(&pipes[i]);
    }
}

bool checkPipeCubeCollision(Cube cube, Pipe *pipes)
{
    for (int i = 0; i < numberOfPipes; i++)
    {
        if (RectRectCollision(cube.body, pipes[i].upper_pipe) || RectRectCollision(cube.body, pipes[i].lower_pipe))
        {
            return true;
        }
    }
    return false;
}

// Setup
Cube player;
Pipe pipes[numberOfPipes];
bool lost = false;
float accumulatedDistance = 0;
int score = 0;

void OnCreate()
{
    negativeBound = -(pipeLen + pipeHorizontalSeparation);
    ConstructGameWindow(screenWidth, screenHeight, charWidth, charHeight, title);
    setCube(&player);
    setPipeList(&pipes);
}

void OnUpdate(float elapsedTime)
{
    player.speed.Y += gravity * elapsedTime;
    player.body.posy += player.speed.Y * elapsedTime;

    if (!lost)
    {
        if (Keys[K_W].JustPressed)
        {
            player.speed.Y = -flapImpulse;
        }

        updatePipeList(pipes, player.speed.X * elapsedTime);

        accumulatedDistance += player.speed.X * elapsedTime;
        score = accumulatedDistance / (pipeWidth + pipeHorizontalSeparation);

        lost = (checkPipeCubeCollision(player, pipes) || player.body.posy > screenHeight || player.body.posy < 0);
        if (lost)
            player.speed.Y = -5;
    }

    FillScreen(' ', Black, Blue);
    drawPipeList(pipes);
    DrawRect(' ', player.body, Yellow, Yellow, 1);
    char scoreString[10];
    sprintf(scoreString, "SCORE: %3d", score);
    WriteText(scoreString, (Vector2){ScreenWidth - 10, 0}, Black, White);

    if (lost)
    {
        WriteText("PERDEU", (Vector2){(ScreenWidth / 2) - 3, ScreenHeight / 2}, Black, Red);
    }
}

int main(int argc, char **argv)
{
    GameRun(60);
    return 0;
}