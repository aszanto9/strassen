#include <iostream>
#include <set>
#include <thread>
#include <vector>
#include <ctime>
#include <cmath>
#include <cassert>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <random>

using namespace std;

typedef struct Matrix {
    int dimension;
    vector<vector<int>> matrix;
} Matrix;

void initMatrix(Matrix* M, int dimension){
    M->dimension = dimension;
    M->matrix.resize(dimension);
    for (int i = 0; i < dimension; i++){
        M->matrix[i].resize(dimension);
    }
}


Matrix* convMult(Matrix& A, Matrix& B, int dimension) {

	Matrix* AB = new Matrix();
	initMatrix(AB, dimension);

	for (int i = 0; i < dimension; ++i)	{
		for (int j = 0; j < dimension; ++j)	{
			for (int k = 0; k < dimension; ++k) {	
				AB.matrix[i][j] += A.matrix[i][k] * B.matrix[k][j];	
			}
		}
	}
	return AB;
}


void printMatrix(Matrix& A){
	for (int i = 0; i < A.dimension; i ++){
        for (int j = 0; j < A.dimension; j++){
            cout << A.matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


bool isEqual(Matrix& A, Matrix& B){
	if (A.dimension != B.dimension) {
		return false;
	}
	else
	{ 
		for (int i = 0; i < A.dimension; ++i) {
			for (int j = 0; j < A.dimension; ++j) {
				if (A.matrix[i][j]!=B.matrix[i][j]){
					return false;
				}
			}
		}
		return true;
	}
}



void testInitMatrix(){
    Matrix* m = new Matrix();
    initMatrix(m,3);
    printMatrix(m);
}

void testConvMult(){
    Matrix* A = new Matrix();
    initMatrix(A,3);
    Matrix* B = new Matrix();
    initMatrix(B,3);
    Matrix* C = new Matrix();
    initMatrix(C,3);

    A->matrix[0][0] = 1;
    A->matrix[0][1] = 2;
    A->matrix[0][2] = -3;
    A->matrix[1][0] = 1;
    A->matrix[1][1] = 3;
    A->matrix[1][2] = 0;
    A->matrix[2][0] = 2;
    A->matrix[2][1] = -1;
    A->matrix[2][2] = 1;

    B->matrix[0][0] = 2;
    B->matrix[0][1] = 1;
    B->matrix[0][2] = 0;
    B->matrix[1][0] = -2;
    B->matrix[1][1] = -1;
    B->matrix[1][2] = 1;
    B->matrix[2][0] = 2;
    B->matrix[2][1] = 1;
    B->matrix[2][2] = -3;

    C->matrix[0][0] = -8;
    C->matrix[0][1] = -4;
    C->matrix[0][2] = 11;
    C->matrix[1][0] = -4;
    C->matrix[1][1] = -2;
    C->matrix[1][2] = 3;
    C->matrix[2][0] = 8;
    C->matrix[2][1] = 4;
    C->matrix[2][2] = -4;

 
    assert (isEqual(convMult(*A,*B,3),C));

}



int main(){
	testConvMult();
    return 0;
}


