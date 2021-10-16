#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#define _WIN32_WINNT 0x0500
#include <windows.h>

#define M_PI 3.14159265358979323846

// Declaracao de variaveis e structs

// Variaveis da tela
int ScreenWidth;
int ScreenHeight;
int BufferSize;

CHAR_INFO *Buffer;
HANDLE wHnd;
HANDLE rHnd;
HWND cWind;
SMALL_RECT WindowSize;

// Structs fundamentais
typedef struct
{
    bool Red;
    bool Green;
    bool Blue;
    bool Intensity;
} Color;

typedef struct
{
    float X;
    float Y;
} Vector2;

typedef struct
{
    float a00;
    float a01;
    float a10;
    float a11;
} Matrix2x2;

typedef struct
{
    float posx;
    float posy;
    float lenx;
    float leny;
} Rect;

typedef struct
{
    bool Pressed;
    bool JustPressed;
    bool JustReleased;
} KeyState;

Color Yellow = {1, 1, 0, 1};
Color Black = {0, 0, 0, 0};
Color Gray = {0, 0, 0, 1};
Color White = {1, 1, 1, 1};
Color Red = {1, 0, 0, 1};
Color Green = {0, 1, 0, 1};
Color Blue = {0, 0, 1, 1};

// Variveis de input
KeyState Keys[256];
bool KeyData[256];

int K_W = (int)'W';
int K_A = (int)'A';
int K_S = (int)'S';
int K_D = (int)'D';

// Declaracao de funcoes

// Funcoes de inicializacao
void ConstructGameWindow(int width, int height, int char_width, int char_height, char *name);
void GameRun(float target_FPS);

// Funcoes do usuario
void OnCreate();
void OnUpdate(float elapsed_time);

// Funcoes Utilitarias
int ScreenToBuffer(Vector2 pos);
void WriteOnBuffer(char write_char, Vector2 pos, Color foreground, Color background);
int ColorToAtribute(Color color, bool foreground);

Vector2 VectorAdition(Vector2 vec1, Vector2 vec2);
Vector2 VectorSubtraction(Vector2 vec1, Vector2 vec2);
Vector2 VectorTimesScalar(Vector2 vec1, float scalar);
Vector2 VectorDotMatrix(Vector2 target_vector, Matrix2x2 target_matrix);
Vector2 NormalizeVector(Vector2 target);
Vector2 RotateVector(Vector2 target, float angle);
float DotProduct(Vector2 vec1, Vector2 vec2);
float VectorLength(Vector2 target);
bool VectorCompare(Vector2 vec1, Vector2 vec2);
void CopyVector(Vector2 *vec1, Vector2 *vec2);

void SetRngSeed(int seed);
float RandomFloatInRange(float min, float max);
int RandomIntInRange(int min, int max);

// Funcoes de desenho
void FillScreen(char fill_char, Color foreground, Color background);
void WriteText(char *string, Vector2 pos, Color foreground, Color background);
void DrawRect(char fill_char, Rect target_rect, Color foreground, Color background, int width);
void DrawLine(char fill_char, Vector2 ini_pos, Vector2 fin_pos, Color foreground, Color background, int width);
void DrawCircle(char fill_char, Vector2 center, int radius, Color foreground, Color background, int width);

// Funcoes de overlap
bool RectRectCollision(Rect rect1, Rect rect2);

// Implementacao de funcoes

// Funcoes de inicializacao
void ConstructGameWindow(int width, int height, int char_width, int char_height, char *name)
{
    // Declara as variaveis locais
    DWORD prev_mode;
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    COORD ScreenBufferSize;

    // Inicia variveis locais e globais
    ScreenWidth = width;
    ScreenHeight = height;
    BufferSize = width * height;

    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);
    cWind = GetConsoleWindow();

    Buffer = malloc(sizeof(CHAR_INFO) * BufferSize);

    // Coloca titulo na aplicacao
    SetConsoleTitle((LPCWSTR)name);

    // Ajusta o tamanho do Buffer do console
    WindowSize = (SMALL_RECT){0, 0, 1, 1};

    if (!SetConsoleWindowInfo(wHnd, true, &WindowSize))
    {
        printf("SetConsoleWindowInfo() failed! Reason : %d", GetLastError());
        exit(1);
    }

    ScreenBufferSize = (COORD){ScreenWidth + 1, ScreenHeight + 1};

    if (!SetConsoleScreenBufferSize(wHnd, ScreenBufferSize))
    {
        printf("SetConsoleScreenBufferSize() failed! Reason : %d", GetLastError());
        exit(1);
    }

    if (!SetConsoleActiveScreenBuffer(wHnd))
    {
        printf("SetConsoleActiveScreenBuffer() failed! Reason : %d", GetLastError());
        exit(1);
    }

    // Muda a fonte do console para uma fonte monospace
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = char_width;
    cfi.dwFontSize.Y = char_height;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    // Ajusta o tamanho e posicao da janela
    WindowSize.Top = 0;
    WindowSize.Left = 0;
    WindowSize.Right = ScreenWidth;
    WindowSize.Bottom = ScreenHeight;

    if (!SetConsoleWindowInfo(wHnd, true, &WindowSize))
    {
        printf("SetConsoleWindowInfo() failed! Reason : %d", GetLastError());
        exit(1);
    }
    if (!SetConsoleMode(rHnd, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
    {
        printf("SetConsoleMode() failed! Reason : %d", GetLastError());
        exit(1);
    }

    SetWindowLong(cWind, GWL_STYLE, GetWindowLong(cWind, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

void GameRun(float target_FPS)
{
    // Variaveis responsaveis pelo gerenciamento da framerate
    // (BUG : Para numeros grandes a framerate parece fica dividida pela metade)
    struct timeval frameStart, frameEnd;
    uint64_t targetFrameTimeMicrosseconds = 1000000.0 / target_FPS;
    float delta = 0;

    // Funcao de inicializacao, executada um vez
    OnCreate();

    while (1)
    {
        delta = fabs(delta);
        gettimeofday(&frameStart, NULL);

        // Funcao de update, executada a cada frame
        OnUpdate(delta);

        // Escreve o Buffer na tela
        WriteConsoleOutput(wHnd, Buffer, (COORD){ScreenWidth, ScreenHeight}, (COORD){0, 0}, &WindowSize);

        // Atulizar input
        bool newKeyData[256];
        for (int i = 0; i < 256; i++)
        {
            newKeyData[i] = (GetKeyState(i) < 0);

            Keys[i].JustPressed = false;
            Keys[i].JustReleased = false;

            if (newKeyData[i] != KeyData[i])
            {
                Keys[i].Pressed = newKeyData[i];

                if (newKeyData[i])
                {
                    Keys[i].JustPressed = true;
                    Keys[i].JustReleased = false;
                }
                else
                {
                    Keys[i].JustPressed = false;
                    Keys[i].JustReleased = true;
                }

                KeyData[i] = newKeyData[i];
            }
        }

        // O bug da framerate parece acontecer por causa da percisao da funcao sleep
        // Mas como eu nao sei como ajeitar eu so vou botar um hardcap e deixar por isso msm

        //gettimeofday(&frameEnd, NULL);

        //delta = frameEnd.tv_usec - frameStart.tv_usec;

        // Faz com que o delta tente atingir o FPS alvo

        //gettimeofday(&frameStart, NULL);

        //if (delta < targetFrameTimeMicrosseconds)
        {
            Sleep(4); // Cap de framerate
        }

        gettimeofday(&frameEnd, NULL);

        delta = (frameEnd.tv_sec - frameStart.tv_sec) * 1000000 + (frameEnd.tv_usec - frameStart.tv_usec);

        // Converte o delta de microssegundos pra segundos
        delta /= 1000000.0;
    }
}

// Funcoes Utilitarias
Vector2 VectorAdition(Vector2 vec1, Vector2 vec2)
{
    Vector2 result = (Vector2){vec1.X + vec2.X, vec1.Y + vec2.Y};
    return result;
}

Vector2 VectorSubtraction(Vector2 vec1, Vector2 vec2)
{
    Vector2 result = (Vector2){vec1.X - vec2.X, vec1.Y - vec2.Y};
    return result;
}

Vector2 VectorTimesScalar(Vector2 vec1, float scalar)
{
    Vector2 result = (Vector2){vec1.X * scalar, vec1.Y * scalar};
    return result;
}

Vector2 VectorDotMatrix(Vector2 target_vector, Matrix2x2 target_matrix)
{
    float x = target_vector.X * target_matrix.a00 + target_vector.Y * target_matrix.a01;
    float y = target_vector.X * target_matrix.a10 + target_vector.Y * target_matrix.a11;
    Vector2 product = (Vector2){x, y};
    return product;
}

Vector2 NormalizeVector(Vector2 target)
{
    float len = VectorLength(target);
    Vector2 normalized = (Vector2){target.X / len, target.Y / len};
    return normalized;
}

Vector2 RotateVector(Vector2 target, float angle)
{
    Matrix2x2 rotationMatix = (Matrix2x2){cosf(angle), sinf(angle), -sinf(angle), cosf(angle)};
    Vector2 rotated = VectorDotMatrix(target, rotationMatix);
    return rotated;
}

void CopyVector(Vector2 *vec1, Vector2 *vec2)
{
    vec1->X = vec2->X;
    vec1->Y = vec2->Y;
}

float DotProduct(Vector2 vec1, Vector2 vec2)
{
    return vec1.X * vec2.X + vec1.Y * vec2.Y;
}

float VectorLength(Vector2 target)
{
    return sqrt(target.X * target.X + target.Y * target.Y);
}

bool VectorCompare(Vector2 vec1, Vector2 vec2)
{
    return (vec1.X == vec2.X && vec1.Y == vec2.Y);
}

int ScreenToBuffer(Vector2 pos)
{
    int X = (int)(pos.X);
    int Y = (int)(pos.Y);

    if (X >= ScreenWidth || X < 0)
        return -1;
    if (Y >= ScreenHeight || Y < 0)
        return -1;

    return X + Y * ScreenWidth;
}

void WriteOnBuffer(char write_char, Vector2 pos, Color foreground, Color background)
{
    int cursorPos = ScreenToBuffer(pos);

    if (cursorPos < BufferSize && cursorPos >= 0)
    {
        Buffer[cursorPos].Char.AsciiChar = write_char;
        Buffer[cursorPos].Attributes = ColorToAtribute(foreground, 0) + ColorToAtribute(background, 1);
    }
}

int ColorToAtribute(Color color, bool foreground)
{
    int atribute = color.Blue * 0x0001 + color.Green * 0x0002 + color.Red * 0x0004 + color.Intensity * 0x0008;
    if (foreground)
        atribute *= 0x0010;
    return atribute;
}

void SetRngSeed(int seed)
{
    srand(seed);
}

float RandomFloatInRange(float min, float max)
{
    float x = ((float)rand() / (float)(RAND_MAX));
    return min * (1 - x) + max * x;
}

int RandomIntInRange(int min, int max)
{
    return min + rand() % (max - min);
}

// Funcoes de desenho
void FillScreen(char fill_char, Color foreground, Color background)
{
    for (int i = 0; i < ScreenHeight; i++)
    {
        for (int j = 0; j < ScreenWidth; j++)
        {
            WriteOnBuffer(fill_char, (Vector2){j, i}, foreground, background);
        }
    }
}

void WriteText(char *w_string, Vector2 pos, Color foreground, Color background)
{
    for (int i = 0; i < strlen(w_string); i++)
    {
        WriteOnBuffer(w_string[i], (Vector2){pos.X + i, pos.Y}, foreground, background);
    }
}

void DrawRect(char fill_char, Rect target_rect, Color foreground, Color background, int width)
{
    for (int i = 0; i < target_rect.leny; i++)
    {
        for (int j = 0; j < target_rect.lenx; j++)
        {
            WriteOnBuffer(fill_char, (Vector2){j + target_rect.posx, i + target_rect.posy}, foreground, background);
        }
    }
}

void DrawLine(char fill_char, Vector2 ini_pos, Vector2 fin_pos, Color foreground, Color background, int width)
{
    int xi = (int)ini_pos.X;
    int xf = (int)fin_pos.X;
    int yi = (int)ini_pos.Y;
    int yf = (int)fin_pos.Y;
    int dx = abs(xf - xi);
    int dy = abs(yf - yi);
    int control = 0;
    int incX = 0;
    int incY = 0;

    if (xf > xi)
        incX = 1;
    else
        incX = -1;

    if (yf > yi)
        incY = 1;
    else
        incY = -1;

    WriteOnBuffer(fill_char, ini_pos, foreground, background);
    Vector2 line = (Vector2){xi, yi};

    if (dx == 0)
    {

        if (yf > yi)
        {
            while (line.Y != yf)
            {
                line.Y++;
                WriteOnBuffer(fill_char, line, foreground, background);
            }
        }
        else
        {
            while (line.Y != yf)
            {

                line.Y--;
                WriteOnBuffer(fill_char, line, foreground, background);
            }
        }
    }
    else if (dy == 0)
    {
        if (xf > xi)
        {
            while (line.X != xf)
            {

                line.X++;
                WriteOnBuffer(fill_char, line, foreground, background);
            }
        }
        else
        {
            while (line.X != xf)
            {

                line.X--;
                WriteOnBuffer(fill_char, line, foreground, background);
            }
        }
    }
    else
    {
        if (dx >= dy)
        {

            control = dx / 2;
            WriteOnBuffer(fill_char, ini_pos, foreground, background);
            while (line.X != xf)
            {
                line.X += incX;
                control = control - dy;
                if (control < 0)
                {
                    line.Y += incY;
                    control += dx;
                }
                WriteOnBuffer(fill_char, line, foreground, background);
            }
        }
        else
        {
            control = dy / 2;
            WriteOnBuffer(fill_char, ini_pos, foreground, background);
            while (line.Y != yf)
            {
                line.Y += incY;
                control = control - dx;
                if (control < 0)
                {
                    line.X += incX;
                    control += dy;
                }
                WriteOnBuffer(fill_char, line, foreground, background);
            }
        }
    }
}

void DrawCircle(char fill_char, Vector2 center, int radius, Color foreground, Color background, int width)
{
    float theta = 0;
    Vector2 point;
    Vector2 lastPoint = (Vector2){center.X + radius * cos(0), center.Y + radius * sin(0)};
    int step = radius * 4;

    while (theta < M_PI)
    {
        point = (Vector2){center.X + radius * cos(theta), center.Y + radius * sin(theta)};
        lastPoint = (Vector2){point.X, 2 * center.Y - point.Y};
        DrawLine(fill_char, lastPoint, point, foreground, background , 1);
        theta += M_PI / step;
    }
}

bool RectRectCollision(Rect rect1, Rect rect2)
{
    if (rect1.posx < rect2.posx + rect2.lenx &&
        rect1.posx + rect1.lenx > rect2.posx &&
        rect1.posy < rect2.posy + rect2.leny &&
        rect1.posy + rect1.leny > rect2.posy)
    {
        return true;
    }

    return false;
}
