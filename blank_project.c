#include <stdio.h>
#include <string.h>
#include <math.h>
#include "BCGE.h"

#define screenWidth 20
#define screenHeight 20
#define charWidth 16
#define charHeight 16
char title[20] = "blank_project";

Vector2 textPosition = {screenWidth / 2 - 6, screenHeight / 2};

void OnCreate()
{
    ConstructGameWindow(screenWidth, screenHeight, charWidth, charHeight, title);
}

void OnUpdate(float elapsedTime)
{
    
    if (Keys[K_W].Pressed)
    {
        textPosition.Y -= 10 * elapsedTime;
    }
    if (Keys[K_S].Pressed)
    {
        textPosition.Y += 10 * elapsedTime;
    }
    if (Keys[K_A].JustPressed)
    {
        textPosition.X -= 1;
    }
    if (Keys[K_D].JustReleased)
    {
        textPosition.X += 1;
    }

    FillScreen(' ', Black, Black);
    WriteText(title, textPosition, White, Black);
}

int main(int argc, char** argv)
{
    GameRun(60);
    return 0;
}