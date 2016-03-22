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
                if (k == 0) 
                    C->matrix[topC + i][leftC + j] = A->matrix[topA + i][leftA + k] * B->matrix[topB + k][leftB + j]; 
				else
                    C->matrix[topC + i][leftC + j] += A->matrix[topA + i][leftA + k] * B->matrix[topB + k][leftB + j];
}

void multiply(Matrix*, Matrix*, Matrix*, int , int , int , int , int , int , int , int); // forward declare for mutual recursion

void strassenMult(Matrix* A, Matrix* B, Matrix* C, int topA, int leftA, int topB, int leftB, int topC, int leftC, int dimension) {
    Matrix* T1 = new Matrix();
    initMatrix(T1, dimension/2); // TODO deal with non-power of 2 case
    Matrix* T2 = new Matrix();
    initMatrix(T2, dimension/2); // TODO deal with non-power of 2 case
    
    // C12 = A21 - A11
    subtract(A, A, C, dimension/2, 0, 0, 0, 0, dimension/2, dimension/2);
    // C21 = B11 + B12
    add(B,B,C,0,0,0,dimension/2,dimension/2,0,dimension/2);
    // C22 = C12 * C21
    multiply(C,C,C,0,dimension/2,dimension/2,0,dimension/2,dimension/2,dimension/2,2);
    //C12 = A12 - A22
    subtract(A,A,C,0,dimension/2,dimension/2,dimension/2,0,dimension/2,dimension/2);
    //C21 = B21 + B22
    add(B, B, C, dimension/2,0,dimension/2,dimension/2,dimension/2,0,dimension/2);
    //C11 = C12 * C21
    multiply(C,C,C,0,dimension/2,dimension/2,0,0,0,dimension/2,2);
    //C12 = A11 + A22
    add(A, A, C, 0, 0, dimension/2, dimension/2, 0, dimension/2, dimension/2);
    //C21 = B11 + B22
    add(B,B,C,0,0,dimension/2,dimension/2,dimension/2,0,dimension/2);
    //T1 = C12*C21
    multiply(C,C,T1,0,dimension/2,dimension/2,0,0,0,dimension/2,2);
    //C11 = T1 + C11
    add(T1,C,C,0,0,0,0,0,0,dimension/2);
    //C22 = T1 + C22
    add(T1,C,C,0,0,dimension/2,dimension/2,dimension/2,dimension/2,dimension/2);
    //T2 = A21 + A22
    add(A,A,T2,dimension/2,0,dimension/2,dimension/2,0,0,dimension/2);
    //C21 = T2 * B11
    multiply(T2,B,C,0,0,0,0,dimension/2,0,dimension/2,2);
    //C22 = C22 - C21
    subtract(C,C,C,dimension/2,dimension/2,dimension/2,0,dimension/2,dimension/2,dimension/2);
    //T1 = B21 - B11
    subtract(B,B,T1,dimension/2,0,0,0,0,0,dimension/2);
    //T2 = A22 * T1
    multiply(A,T1,T2,dimension/2,dimension/2,0,0,0,0,dimension/2,2);
    //C21 = C21 + T2
    add(C,T2,C,dimension/2,0,0,0,dimension/2,0,dimension/2);
    //C11 = C11 + T2
    add(C,T2,C,0,0,0,0,0,0,dimension/2);
    //T1 = B12 - B22
    subtract(B,B,T1,0,dimension/2,dimension/2,dimension/2,0,0,dimension/2);
    //C12 = A11 * T1
    multiply(A,T1,C,0,0,0,0,0,dimension/2,dimension/2,2);
    //C22 = C22 + C12
    add(C,C,C,dimension/2,dimension/2,0,dimension/2,dimension/2,dimension/2,dimension/2);
    //T2 = A11 + A12
    add(A,A,T2,0,0,0,dimension/2,0,0,dimension/2);
    //T1 = T2 * B22
    multiply(T2,B,T1,0,0,dimension/2,dimension/2,0,0,dimension/2,2);
    //C12 = C12 + T1
    add(C,T1,C,0,dimension/2,0,0,0,dimension/2,dimension/2);
    //C11 = C11 - T1
    subtract(C,T1,C,0,0,0,0,0,0,dimension/2);

    free(T1);
    free(T2);
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

    A->matrix = {{1, 2, -3}, {1, 3, 0}, {2, -1, 1}};
    B->matrix = {{2, 1, 0}, {-2, -1, 1}, {2, 1, -3}};
    C->matrix = {{-8, -4, 11}, {-4, -2, 3}, {8, 4, -4}};
    
    convMult(A,B,D,0,0,0,0,0,0,3);
 
    assert(isEqual(D, C));
    free(A);
    free(B);
    free(C);

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
    C->matrix = {{8,3},{9,4}};
    
    multiply(A, B, D, 0, 0, 0, 0, 0, 0, 2, 2);
    assert(isEqual(D, C));
    free(A);
    free(B);
    free(C);
    
    

}



int main(){
    testStrasMult();
	testConvMult();
    
    return 0;
}


