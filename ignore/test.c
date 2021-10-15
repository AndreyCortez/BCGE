#include <stdio.h>
#include <stdlib.h>

float **MatrixMultiplication(int mat1_r, int mat1_c, int mat2_r, int mat2_c, float** mat1, float** mat2)
{
    float **result;
    result = (float **)malloc(sizeof(float *) * mat1_r);

    for (int i = 0; i < mat1_r; i++)
    {
        result[i] = (float *)malloc(sizeof(float) * mat2_c);
    }
    
    for (int i = 0; i < mat1_r; i++)
    {
        for (int j = 0; j < mat2_c; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < mat2_c; k++)
            {
                result[i][j] += mat1[i][k] * mat2[k][j];
                //printf("%f ", mat1[i][j]);
            }
            printf("%f ", result[i][j]);
        }
        printf("\n");
    }

    return result;
}

int main()
{
    float mat1[4][4] = {
        {1, 1, 1, 1},
        {2, 2, 2, 2},
        {3, 3, 3, 3},
        {4, 4, 4, 4}};

    float mat2[4][4] = {
        {1, 1, 1, 1},
        {2, 2, 2, 2},
        {3, 3, 3, 3},
        {4, 4, 4, 4}};

    int dim1[2] = {4, 4};
    int dim2[2] = {4, 4};


    MatrixMultiplication(4, 4, 4, 4, (float **)mat1, (float **)mat2);

    return 0;
}