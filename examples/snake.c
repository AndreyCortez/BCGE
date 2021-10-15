#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../BCGE.h"

#define boardWidth 20
#define boardHeight 20
#define charWidth 16
#define charHeight 16
#define initialSnakeLen 5

#define frameRate 120
#define movementFrames 12

Vector2 snake[boardHeight * boardWidth];
Vector2 initialSnakePosition = {10, 10};
Vector2 movementDirection[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
Vector2 currentMovementDirection = {1, 0};

Vector2 apple;

int snakeLen = initialSnakeLen;
int tailPointer = initialSnakeLen - 1;
int headPointer = 0;

bool lost = false;

float timer = 0;

void spawnApple()
{
    apple = (Vector2){RandomIntInRange(0, boardWidth - 1), RandomIntInRange(0, boardHeight - 1)};
    for (int i = 0; i < snakeLen; i++)
    {
        if (VectorCompare(apple, snake[i]))
            spawnApple();
    }
}

Vector2 coordinateWrap(Vector2 vec)
{
    Vector2 returnable;
    if (vec.X >= boardWidth)
    {
        returnable.X = vec.X - boardWidth;
    }
    if (vec.X < 0)
    {
        returnable.X = vec.X + boardWidth;
    }
    if (vec.Y >= boardHeight)
    {
        returnable.Y = vec.Y - boardHeight;
    }
    if (vec.Y < 0)
    {
        returnable.Y = vec.Y + boardHeight;
    }
    return returnable;
}

void OnCreate()
{
    ConstructGameWindow(boardWidth, boardHeight, charWidth, charHeight, "Snake");
    for (int i = 0; i < initialSnakeLen; i++)
    {
        snake[i] = VectorSubtraction(initialSnakePosition, VectorTimesScalar((Vector2){1, 0}, i));
    }
    spawnApple();
}

void OnUpdate(float elapsedTime)
{
    // Cuida do input
    int K_W = (int)'W';
    int K_A = (int)'A';
    int K_S = (int)'S';
    int K_D = (int)'D';

    if (Keys[K_W].Pressed)
    {
        if (currentMovementDirection.Y != -movementDirection[3].Y)
        {
            currentMovementDirection = movementDirection[3];
        }
    }
    else if (Keys[K_A].Pressed)
    {
        if (currentMovementDirection.X != -movementDirection[1].X)
        {
            currentMovementDirection = movementDirection[1];
        }
    }
    else if (Keys[K_S].Pressed)
    {
        if (currentMovementDirection.Y != -movementDirection[2].Y)
        {
            currentMovementDirection = movementDirection[2];
        }
    }
    else if (Keys[K_D].Pressed)
    {
        if (currentMovementDirection.X != -movementDirection[0].X)
        {
            currentMovementDirection = movementDirection[0];
        }
    }

    // Atualiza a cobra

    if (!lost)
        timer += elapsedTime;

    if (VectorCompare(snake[headPointer], apple))
    {
        snakeLen += 1;
        snake[snakeLen] = (Vector2){-1, -1};
        spawnApple();
    }
    else if (timer > movementFrames * (1.0 / frameRate))
    {
        timer = 0;

        snake[tailPointer] = VectorAdition(snake[headPointer], currentMovementDirection);
        snake[tailPointer] = coordinateWrap(snake[tailPointer]);
        headPointer = tailPointer;
        tailPointer -= 1;
        if (tailPointer < 0)
            tailPointer = snakeLen - 1;

        for (int i = 0; i < snakeLen; i++)
        {
            if (i != headPointer)
            {
                if (VectorCompare(snake[headPointer], snake[i]))
                    lost = true;
            }
        }
    }

    // Desenha
    FillScreen('#', Gray, Black);

    char fpsCounter[8];
    sprintf(fpsCounter, "SCORE:%3d", snakeLen - initialSnakeLen);
    WriteText(fpsCounter, (Vector2){boardWidth - 9, 0}, Black, White);

    for (int i = 0; i < snakeLen; i++)
    {
        WriteText(" ", snake[i], Yellow, Yellow);
        if (i == tailPointer)
        {
            WriteText("<", snake[i], Black, Yellow);
        }
        if (i == headPointer)
        {
            WriteText(":", snake[i], Black, Yellow);
        }
    }

    WriteText("`", apple, Green, Red);

    if (lost)
    {
        WriteText("x", snake[headPointer], Black, Yellow);
        WriteText("PERDEU", (Vector2){(ScreenWidth / 2) - 3, ScreenHeight / 2}, Black, Green);
    }
}

int main(int argc, char **argv)
{
    GameRun(frameRate);
}