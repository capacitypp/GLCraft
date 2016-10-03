//3行3列の行列クラス

#ifndef ___Class_Matrix
#define ___Class_Matrix

#include <iostream>

#include "Vector.h"

#define MATRIX_ROWS	3
#define MATRIX_COLS	3

class Matrix {
	double table[MATRIX_ROWS][MATRIX_COLS];
public:
	Matrix(void) {
		for (int i = 0; i < MATRIX_ROWS; i++)
		for (int j = 0; j < MATRIX_COLS; j++)
			table[i][j] = 0.0;
	}
	inline void print(void) {
		for (int i = 0; i < MATRIX_ROWS; i++) {
			for (int j = 0; j < MATRIX_COLS; j++)
				std::cout << table[i][j] << " ";
			std::cout << std::endl;
		}
	}
	inline void operator+=(const Matrix& x) {
		for (int i = 0; i < MATRIX_ROWS; i++)
		for (int j = 0; j < MATRIX_COLS; j++)
			table[i][j] += x.table[i][j];
	}
	inline void operator*=(double k) {
		for (int i = 0; i < MATRIX_ROWS; i++)
		for (int j = 0; j < MATRIX_COLS; j++)
			table[i][j] *= k;
	}
	inline void operator/=(double k) { operator*=(1 / k); }
	inline double* operator[] (int idx) const { return (double*)&table[idx][0]; }
	inline Matrix operator+(const Matrix& x) {
		Matrix matrix(*this);
		matrix += x;
		return matrix;
	}
	inline Matrix operator*(const Matrix& x) const {
		Matrix matrix;
		for (int i = 0; i < MATRIX_ROWS; i++)
		for (int j = 0; j < MATRIX_COLS; j++)
		for (int k = 0; k < MATRIX_COLS; k++)
			matrix[i][j] += table[i][k] * x.table[k][j];
		return matrix;
	}
	inline Vector operator*(const Vector& v) const {
		Vector vector;
		for (int i = 0; i < MATRIX_ROWS; i++)
		for (int j = 0; j < MATRIX_COLS; j++)
			vector[i] += table[i][j] * v[j];
		return vector;
	}
	inline Matrix operator*(double k) const {
		Matrix matrix(*this);
		matrix *= k;
		return matrix;
	}
};

#endif
