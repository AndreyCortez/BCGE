#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../BCGE.h"

#define screenWidth 240
#define screenHeight 80
#define charWidth 4
#define charHeight 4
char title[20] = "Doom";

// player
typedef struct
{
    Vector2 position;
    float rotation;
    float FOV;
} Player;

// map
#define mapWidth 20
#define mapHeight 14

char map[mapHeight * mapWidth] =
"####################\
#........#.........#\
#..................#\
#..................#\
#........#.........#\
#........#.........#\
##########.........#\
#############...####\
#........#.........#\
#..##....#.....#...#\
#..##..........#...#\
#..............#...#\
#........#.....#...#\
####################";

// Inicializacao
Player player;
float fieldDepth = 16.0;
void OnCreate()
{
    player.position = (Vector2){4, 4};
    player.rotation = 0;
    player.FOV = M_PI / 2;
    ConstructGameWindow(screenWidth, screenHeight, charWidth, charHeight, title);
}

void OnUpdate(float elapsedTime)
{
    FillScreen(' ', Black, Black);

    if (Keys[K_W].Pressed)
    {
        player.position = VectorAdition(player.position, VectorTimesScalar(RotateVector((Vector2){0, 1}, player.rotation), 5 * elapsedTime));
    }
    if (Keys[K_A].Pressed)
    {
        player.rotation -= 2 * elapsedTime;
    }
    if (Keys[K_S].Pressed)
    {
        player.position = VectorSubtraction(player.position, VectorTimesScalar(RotateVector((Vector2){0, 1}, player.rotation), 5 * elapsedTime));
    }
    if (Keys[K_D].Pressed)
    {
        player.rotation += 2 * elapsedTime;
    }

    for (int i = 0; i < screenWidth; i++)
    {
        float rayAngle = (player.rotation - player.FOV / 2.0) + ((float)i / (float)screenWidth) * player.FOV;
        float distanceToWall = 0;
        bool wallHit = false;

        Vector2 unitVector = (Vector2){sinf(rayAngle), cosf(rayAngle)};

        while (!wallHit && distanceToWall < fieldDepth)
        {
            distanceToWall += 0.1;
            Vector2 testVector = VectorAdition(player.position, VectorTimesScalar(unitVector, distanceToWall));
            if (testVector.X < 0 || testVector.X >= mapWidth || testVector.Y < 0 || testVector.X >= mapWidth)
            {
                wallHit = true;
                distanceToWall = fieldDepth;
            }
            else
            {
                if (map[(int)testVector.Y * mapWidth + (int)testVector.X] == '#')
                {
                    wallHit = true;
                }
            }
        }

        int ceiling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceToWall);
        int floor = screenHeight - ceiling;

        for (int j = 0; j < screenHeight; j++)
        {
            Color wallShade;
            if (distanceToWall >= fieldDepth * 3 / 4)
            {
                wallShade = (Color){0, 0, 0, 0};
            }
            else if (distanceToWall >= fieldDepth / 2)
            {
                wallShade = (Color){0, 0, 0, 1};
            }
            else if (distanceToWall >= fieldDepth / 4)
            {
                wallShade = (Color){1, 1, 1, 0};
            }
            else
            {
                wallShade = (Color){1, 1, 1, 1};
            }

            if (j < ceiling)
                WriteText(" ", (Vector2){i, j}, Black, Black);
            else if (j > ceiling && j <= floor)
                WriteText(" ", (Vector2){i, j}, Black, wallShade);
            else
                WriteText(" ", (Vector2){i, j}, Black, Black);
        }
    }
}

int main(int argc, char **argv)
{
    GameRun(60);
    return 0;
}