#include "MatrixCalc.h"
#include <Novice.h>
#include <corecrt_math.h>

static const int kRowHeight = 20;
static const int kColumnWidth = 64;

void MatrixScreenPrintf(int x, int y, Matrix2x2 matrix) {
	for (int row = 0; row < 2; ++row)
	{
		for (int column = 0; column < 2; ++column)
		{
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%8.02f", matrix.m[row][column]);
		}
	}

}

void VectorScreenPrintf(int x, int y, Vector2 vector) {
	Novice::ScreenPrintf(x, y, "%8.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%8.02f", vector.y);


}

Matrix2x2 Add(Matrix2x2 matrix1, Matrix2x2 matrix2)
{

	return Matrix2x2{ {
	{matrix1.m[0][0] + matrix2.m[0][0],matrix1.m[0][1] + matrix2.m[0][1] } ,
	{matrix1.m[1][0] + matrix2.m[1][0],matrix1.m[1][1] + matrix2.m[1][1] }
	} };
}

Matrix2x2 Subtract(Matrix2x2 matrix1, Matrix2x2 matrix2)
{
	return Matrix2x2{ {
	{matrix1.m[0][0] - matrix2.m[0][0],matrix1.m[0][1] - matrix2.m[0][1] } ,
	{matrix1.m[1][0] - matrix2.m[1][0],matrix1.m[1][1] - matrix2.m[1][1] }
		} };
}

Matrix2x2 Multiply(Matrix2x2 matrix1, Matrix2x2 matrix2)
{
	return Matrix2x2{ {
	{(matrix1.m[0][0] * matrix2.m[0][0]) + (matrix1.m[0][1] * matrix2.m[1][0]),(matrix1.m[0][0] * matrix2.m[0][1]) + (matrix1.m[0][1] * matrix2.m[1][1])} ,
	{(matrix1.m[1][0] * matrix2.m[0][0]) + (matrix1.m[1][1] * matrix2.m[1][0]),(matrix1.m[1][0] * matrix2.m[0][1]) + (matrix1.m[1][1] * matrix2.m[1][1]) }
		} };
}

Matrix2x2 Multiply(float matrix1, Matrix2x2 matrix2)
{
	return Matrix2x2{ {
		{matrix1 * matrix2.m[0][0],matrix1 * matrix2.m[0][1]},
		{matrix1 * matrix2.m[1][0],matrix1 * matrix2.m[1][1]}

		} };
}

Matrix2x2 MakeRotateMatrix(float theta)
{

	return Matrix2x2{ {
		{cosf(theta),sinf(theta)},
		{-sinf(theta),cosf(theta)}

		} };
}

Vector2 Multiply(Vector2 vector, Matrix2x2 matrix2)
{
	return Vector2{ (vector.x * matrix2.m[0][0]) + (vector.y * matrix2.m[1][0]),(vector.x * matrix2.m[0][1]) + (vector.y * matrix2.m[1][1]) };
}



