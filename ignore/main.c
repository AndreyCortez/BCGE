#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../BCGE.h"

Rect player;
Rect box = {20, 20, 20, 20};
float rotation = 0;

void OnCreate()
{
    ConstructGameWindow(50, 60, 16, 16, "teste");
    player = (Rect){2, 2, 5, 5};
}

void OnUpdate(float elapsedTime)
{

    if (Keys[K_W].Pressed)
    {
        player.posy -= 20 * elapsedTime;
    }
    if (Keys[K_A].Pressed)
    {
        player.posx -= 20 * elapsedTime;
    }
    if (Keys[K_S].Pressed)
    {
        player.posy += 20 * elapsedTime;
    }
    if (Keys[K_D].Pressed)
    {
        player.posx += 20 * elapsedTime;
    }

    rotation += 1 * elapsedTime;

    FillScreen(' ', (Color){1, 0, 1, 1}, (Color){1, 0, 1, 0});

    DrawLine(" ", RotateVector((Vector2){12, 12}, rotation), (Vector2){25, 25}, (Color){1, 1, 0, 0}, (Color){0, 1, 1, 1});
    DrawLine(" ", RotateVector((Vector2){12, 12}, rotation), (Vector2){0, 0}, (Color){1, 1, 0, 0}, (Color){0, 1, 1, 1});
    DrawLine(" ", RotateVector((Vector2){12, 12}, rotation), (Vector2){0, 25}, (Color){1, 1, 0, 0}, (Color){0, 1, 1, 1});
    DrawLine(" ", RotateVector((Vector2){12, 12}, rotation), (Vector2){25, 0}, (Color){1, 1, 0, 0}, (Color){0, 1, 1, 1});
    //DrawCircle(" ", (Vector2){50, 50}, 30, (Color){1, 1, 0, 0}, (Color){0, 1, 1, 1});
    DrawRect(' ', box, (Color){0, 1, 0, 0}, (Color){0, 1, 0, 1}, 1);

    if (RectRectCollision(player, box))
    {
        DrawRect(' ', player, (Color){0, 1, 0, 0}, (Color){0, 1, 0, 1}, 1);
    }
    else
    {
        DrawRect(' ', player, (Color){0, 1, 1, 0}, (Color){0, 1, 1, 1}, 1);
    }

    char print_string[8];
    sprintf(print_string, "%.2f", 1.0 / (elapsedTime));
    WriteText(print_string, (Vector2){20, 0}, (Color){0, 0, 0, 0}, (Color){1, 1, 1, 1});

    //Vector2 result = RotateVector((Vector2){6, 8}, rotation);
    //sprintf(print_string, "%.1f , %.1f, %.1f", result.X, result.Y, result.X * result.X + result.Y * result.Y);
    //WriteText(print_string, (Vector2){9, 1}, (Color){0, 0, 0, 0}, (Color){1, 1, 1, 1});
}

int main(int argc, char **argv)
{
    GameRun(1);
}
