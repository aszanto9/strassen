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

struct Matrix {
    int dimension;
    vector<vector<int>> matrix;
};

void initMatrix(Matrix* M, int dimension){
    M->dimension = dimension;
    M->matrix.resize(dimension);
    for (int i = 0; i < dimension; i++){
        M->matrix[i].resize(dimension);
    }
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


bool isEqual(Matrix* A, Matrix* B){
    if (A->dimension != B->dimension) {
        return false;
    }
    else
    {
        for (int i = 0; i < A->dimension; ++i) {
            for (int j = 0; j < A->dimension; ++j) {
                if (A->matrix[i][j] != B->matrix[i][j]){
                    return false;
                }
            }
        }
        return true;
    }
}

void add(Matrix* A, Matrix* B, Matrix* C, int topA, int leftA, int topB, int leftB, int topC, int leftC, int dimension){
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            C->matrix[topC + i][leftC + j] = A->matrix[topA + i][leftA + j] + B->matrix[topB + i][leftB + j];
}

void subtract(Matrix* A, Matrix* B, Matrix* C, int topA, int leftA, int topB, int leftB, int topC, int leftC, int dimension){
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            C->matrix[topC + i][leftC + j] = A->matrix[topA + i][leftA + j] - B->matrix[topB + i][leftB + j];
}

void convMult(Matrix* A, Matrix* B, Matrix* C, int topA, int leftA, int topB, int leftB, int topC, int leftC, int dimension) {

	for (int i = 0; i < dimension; ++i)
		for (int j = 0; j < dimension; ++j)
			for (int k = 0; k < dimension; ++k)
				C->matrix[topC + i][leftC + j] += A->matrix[topA + i][leftA + k] * B->matrix[topB + k][leftB + j];
}

void multiply(Matrix*, Matrix*, Matrix*, int , int , int , int , int , int , int , int);

void strassenMult(Matrix* A, Matrix* B, Matrix* C, int topA, int leftA, int topB, int leftB, int topC, int leftC, int dimension) {
    Matrix* T1 = new Matrix();
    initMatrix(T1, dimension / 2); // TODO deal with non-power of 2 case
    Matrix* T2 = new Matrix();
    initMatrix(T2, dimension / 2); // TODO deal with non-power of 2 case
    
    // C12 = A21 - A11
    subtract(A, A, C, dimension / 2, 0, 0, 0, 0, dimension / 2, dimension/2);
    printMatrix(*C);
    // C21 = B11 + B12
    add(B,B,C,0,0,0,dimension/2,dimension/2,0,dimension/2);
    printMatrix(*C);
    // C22 = C12 * C21
    multiply(C,C,C,0,dimension/2,dimension/2,0,dimension/2,dimension/2,dimension/2,2);
    printMatrix(*C);
    //C12 = A12 - A22
    subtract(A,A,C,0,dimension/2,dimension/2,dimension/2,0,dimension/2,dimension/2);
    printMatrix(*C);
    //C21 = B21 + B22
    add(B, B, C, dimension/2,0,dimension/2,dimension/2,dimension/2,0,dimension/2);
    printMatrix(*C);
    
    
}

void multiply(Matrix* A, Matrix* B, Matrix* C, int topA, int leftA, int topB, int leftB, int topC, int leftC, int dimension, int threshold){
    if (dimension >= threshold)
        strassenMult(A, B, C, topA, leftA, topB, leftB, topC, leftC, dimension);
    else
        convMult(A, B, C, topA, leftA, topB, leftB, topC, leftC, dimension);
}




void testInitMatrix(){
    Matrix* m = new Matrix();
    initMatrix(m,3);
    printMatrix(*m);
}

void testConvMult(){
    Matrix* A = new Matrix();
    initMatrix(A,3);
    Matrix* B = new Matrix();
    initMatrix(B,3);
    Matrix* C = new Matrix();
    initMatrix(C,3);
    Matrix* D = new Matrix();
    initMatrix(D,3);

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
    
    convMult(A,B,D,0,0,0,0,0,0,3);
 
    assert(isEqual(D, C));
    cout << "All tests pass!" << endl;

}

void testStrasMult(){
    Matrix* A = new Matrix();
    initMatrix(A,2);
    Matrix* B = new Matrix();
    initMatrix(B,2);
    Matrix* C = new Matrix();
    initMatrix(C,2);
    Matrix* D = new Matrix();
    initMatrix(D,2);
    
    A->matrix = {{1,2},{3,1}};
    B->matrix = {{2,1},{3,1}};
    
    C->matrix = {{0,1},{2,6}};
    printMatrix(*C);
    
    multiply(A, B, D, 0, 0, 0, 0, 0, 0, 2, 2);
    
    

}



int main(){
    testStrasMult();
	//testConvMult();
    
    return 0;
}


