#pragma once
#include "Header/Struct.h"
struct Matrix2x2
{
	float m[2][2];

};


void MatrixScreenPrintf(int x, int y, Matrix2x2 matrix);
void VectorScreenPrintf(int x, int y, Vector2 vector);


Matrix2x2 Add(Matrix2x2 matrix1, Matrix2x2 matrix2);

Matrix2x2 Subtract(Matrix2x2 matrix1, Matrix2x2 matrix2);

Matrix2x2 Multiply(Matrix2x2 matrix1, Matrix2x2 matrix2);

Vector2 Multiply(Vector2 vector, Matrix2x2 matrix2);
Matrix2x2 Multiply(float matrix1, Matrix2x2 matrix2);

Matrix2x2 MakeRotateMatrix(float theta);
