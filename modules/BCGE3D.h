#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../BCGE.h"

typedef struct
{
    float X;
    float Y;
    float Z;
} Vector3;

typedef struct
{
    float X;
    float Y;
    float Z;
    float W;
} Vector4;

typedef float Matrix3x3[3][3];
typedef float Matrix4x4[4][4];

typedef struct
{
    int V1;
    int V2;
    int V3;
} Face;

typedef struct
{
    Vector3 Postition;
    Vector3 Target;
    float FOV;
    float AspectRatio;
    float Far;
    float Near;
} Camera;

typedef struct
{
    char *name;
    Vector3 *Vertices;
    Face *Faces;
    int Num_Faces;
    Vector3 Position;
    Vector3 Rotation;
} Mesh;

void InitMesh(Mesh *mesh, Vector3 *vertices, Face *faces, char *name, int number_of_faces)
{
    mesh->name = name;
    mesh->Vertices = vertices;
    mesh->Faces = faces;
    mesh->Num_Faces = number_of_faces;
}

Vector4 Vector4TimesMatrix4x4(Vector4 vector, Matrix4x4 matrix)
{
    Vector4 target;
    target.X = vector.X * matrix[0][0] + vector.Y * matrix[1][0] + vector.Z * matrix[2][0] + vector.W * matrix[3][0];
    target.Y = vector.X * matrix[0][1] + vector.Y * matrix[1][1] + vector.Z * matrix[2][1] + vector.W * matrix[3][1];
    target.Z = vector.X * matrix[0][2] + vector.Y * matrix[1][2] + vector.Z * matrix[2][2] + vector.W * matrix[3][2];
    target.W = vector.X * matrix[0][3] + vector.Y * matrix[1][3] + vector.Z * matrix[2][3] + vector.W * matrix[3][3];
    return target;
}

Vector4 Vector4TimesScalar(Vector4 vector, float scalar)
{
    Vector4 target;
    target.X = vector.X * scalar;
    target.Y = vector.Y * scalar;
    target.Z = vector.Z * scalar;
    target.W = vector.W * scalar;
    return target;
}

Vector3 Vector3TimesMatrix3x3(Vector3 vector, Matrix3x3 matrix)
{
    Vector3 target;
    target.X = vector.X * matrix[0][0] + vector.Y * matrix[1][0] + vector.Z * matrix[2][0];
    target.Y = vector.X * matrix[0][1] + vector.Y * matrix[1][1] + vector.Z * matrix[2][1];
    target.Z = vector.X * matrix[0][2] + vector.Y * matrix[1][2] + vector.Z * matrix[2][2];
    return target;
}

Vector3 Vector3Adittion(Vector3 vec1, Vector3 vec2)
{
    Vector3 target;
    target.X = vec1.X + vec2.X;
    target.Y = vec1.Y + vec2.Y;
    target.Z = vec1.Z + vec2.Z;
    return target;
}

Vector3 Vector3TimesScalar(Vector3 vector, float scalar)
{
    Vector3 target;
    target.X = vector.X * scalar;
    target.Y = vector.Y * scalar;
    target.Z = vector.Z * scalar;
    return target;
}

Matrix3x3 matRotX;
Matrix3x3 matRotY;
Matrix3x3 matRotZ;

Vector3 Vector3Rotate(Vector3 angle)
{
}

Vector4 ProjectPoint(Vector3 point, Camera camera)
{
    Matrix4x4 transform = {{camera.AspectRatio / tanf(camera.FOV / 2), 0, 0, 0},
                           {0, 1 / tanf(camera.FOV / 2), 0, 0},
                           {0, 0, camera.Far / (camera.Far - camera.Near), 1},
                           {0, 0, -(camera.Far * camera.Near) / (camera.Far - camera.Near), 0}};

    Vector4 projection = {point.X, point.Y, point.Z, 1};

    projection = Vector4TimesMatrix4x4(projection, transform);

    if (projection.W != 0)
        projection = Vector4TimesScalar(projection, 1 / projection.W);

    return projection;
}

Mesh LoadObjFile(char *filepath)
{
    FILE *in_file = fopen("name_of_file", "r");

    char line[100];
    while (fgets(line, 100, stdin) != NULL)
    {
        fprintf("The line is: %s\n", line);
    }
    Mesh temp_mesh;
}

void RenderMesh(Mesh mesh, Camera camera)
{
    for (int i = 0; i < mesh.Num_Faces; i++)
    {
        Vector3 v1 = mesh.Vertices[mesh.Faces[i].V1 - 1];
        Vector3 v2 = mesh.Vertices[mesh.Faces[i].V2 - 1];
        Vector3 v3 = mesh.Vertices[mesh.Faces[i].V3 - 1];

        v1 = Vector3Adittion(v1, mesh.Position);
        v2 = Vector3Adittion(v2, mesh.Position);
        v3 = Vector3Adittion(v3, mesh.Position);

        Vector4 v1_ = ProjectPoint(v1, camera);
        Vector4 v2_ = ProjectPoint(v2, camera);
        Vector4 v3_ = ProjectPoint(v3, camera);

        Vector2 v1__ = {(v1_.X + 1.0) * (float)ScreenWidth / 2, (v1_.Y + 1.0) * (float)ScreenHeight / 2};
        Vector2 v2__ = {(v2_.X + 1.0) * (float)ScreenWidth / 2, (v2_.Y + 1.0) * (float)ScreenHeight / 2};
        Vector2 v3__ = {(v3_.X + 1.0) * (float)ScreenWidth / 2, (v3_.Y + 1.0) * (float)ScreenHeight / 2};

        DrawLine(" ", v1__, v2__, White, White);
        DrawLine(" ", v1__, v3__, White, White);
        DrawLine(" ", v2__, v3__, White, White);
    }
}