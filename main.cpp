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
    vector<vector<long long>> matrix;
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
    subtract(A, A, C, topA + dimension/2, leftA, topA, leftA, topC, leftC + dimension/2, dimension/2);
    // C21 = B11 + B12
    add(B,B,C,topB,leftB,topB,leftB + dimension/2,topC + dimension/2,leftC,dimension/2);
    // C22 = C12 * C21
    multiply(C,C,C,topC,leftC + dimension/2,topC + dimension/2,leftC,topC + dimension/2,leftC + dimension/2,dimension/2,2);
    //C12 = A12 - A22
    subtract(A,A,C,topA,leftA + dimension/2,topA + dimension/2,leftA + dimension/2,topC,leftC + dimension/2,dimension/2);
    //C21 = B21 + B22
    add(B, B, C, topB + dimension/2,leftB,topB + dimension/2,leftB + dimension/2,topC + dimension/2,leftC,dimension/2);
    //C11 = C12 * C21
    multiply(C,C,C,topC,leftC + dimension/2,topC + dimension/2,leftC,topC,leftC,dimension/2,2);
    //C12 = A11 + A22
    add(A, A, C, topA, leftA, topA + dimension/2, leftA + dimension/2, topC, leftC + dimension/2, dimension/2);
    //C21 = B11 + B22
    add(B,B,C,topB,leftB,topB + dimension/2,leftB + dimension/2,topC + dimension/2,leftC,dimension/2);
    //T1 = C12*C21
    multiply(C,C,T1,topC,leftC + dimension/2,topC + dimension/2,leftC,0,0,dimension/2,2);
    //C11 = T1 + C11
    add(T1,C,C,0,0,topC,leftC,topC,leftC,dimension/2);
    //C22 = T1 + C22
    add(T1,C,C,0,0,topC + dimension/2,leftC + dimension/2,topC + dimension/2,leftC + dimension/2,dimension/2);
    //T2 = A21 + A22
    add(A,A,T2,topA + dimension/2,leftA,topA + dimension/2,leftA + dimension/2,0,0,dimension/2);
    //C21 = T2 * B11
    multiply(T2,B,C,0,0,topB,leftB,topC + dimension/2,leftC,dimension/2,2);
    //C22 = C22 - C21
    subtract(C,C,C,topC + dimension/2,leftC + dimension/2,topC + dimension/2,leftC,topC + dimension/2,leftC + dimension/2,dimension/2);
    //T1 = B21 - B11
    subtract(B,B,T1,topB + dimension/2,leftB,topB,leftB,0,0,dimension/2);
    //T2 = A22 * T1
    multiply(A,T1,T2,topA + dimension/2,leftA + dimension/2,0,0,0,0,dimension/2,2);
    //C21 = C21 + T2
    add(C,T2,C,topC + dimension/2,leftC,0,0,topC + dimension/2,leftC,dimension/2);
    //C11 = C11 + T2
    add(C,T2,C,topC,leftC,0,0,topC,leftC,dimension/2);
    //T1 = B12 - B22
    subtract(B,B,T1,topB,leftB + dimension/2,topB + dimension/2,leftB + dimension/2,0,0,dimension/2);
    //C12 = A11 * T1
    multiply(A,T1,C,topA,leftA,0,0,topC,leftC + dimension/2,dimension/2,2);
    //C22 = C22 + C12
    add(C,C,C,topC + dimension/2,leftC + dimension/2,topC,leftC + dimension/2,topC + dimension/2,leftC + dimension/2,dimension/2);
    //T2 = A11 + A12
    add(A,A,T2,topA,leftA,topA,leftA + dimension/2,0,0,dimension/2);
    //T1 = T2 * B22
    multiply(T2,B,T1,0,0,topB + dimension/2,leftB + dimension/2,0,0,dimension/2,2);
    //C12 = C12 + T1
    add(C,T1,C,topC,leftC + dimension/2,0,0,topC,leftC + dimension/2,dimension/2);
    //C11 = C11 - T1
    subtract(C,T1,C,topC,leftC,0,0,topC,leftC,dimension/2);

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
    cout << "all conventional tests pass" << endl;

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
    
    Matrix* E = new Matrix();
    initMatrix(E,8);
    Matrix* F = new Matrix();
    initMatrix(F,8);
    Matrix* G = new Matrix();
    initMatrix(G,8);
    Matrix* H = new Matrix();
    initMatrix(H,8);
    
    E->matrix = {{9, 3, 5, 7, 5, 7, 13, 18}, {1, 9, 0, 4, 17, 2, 5, 8}, {4, 8, 6, 7,5, 12, 3, 2},
                    {11, 2, 4, 11, 19, 13, 3, 4}, {17, 17, 16, 1, 1, 6, 3,1}, {15, 11, 5, 20, 17, 3, 7, 16},
                    {10, 7, 2, 6, 18, 15, 17,15}, {17, 4, 16, 19, 2, 15, 11, 0}};
    F->matrix = {{3, 9, 0, 1, 11, 12, 19, 11}, {5, 10, 8, 14, 12, 10, 14, 12}, {13,
        11, 12, 4, 4, 13, 5, 7}, {8, 0, 18, 8, 0, 2, 8, 16}, {6, 0, 7, 18,
            2, 5, 20, 6}, {17, 17, 0, 17, 5, 4, 9, 1}, {12, 8, 3, 9, 10, 3, 6,
                3}, {15, 3, 18, 12, 7, 14, 17, 16}};
    
    G->matrix = {{738, 443, 608, 669, 456, 561, 841, 646}, {396, 197, 422, 640, 269,
        330, 701, 430}, {486, 416, 342, 541, 278, 330, 534, 377}, {614, 420,
            476, 781, 322, 438, 928, 549}, {511, 628, 380, 486, 524, 636, 758,
                556}, {802, 455, 936, 961, 548, 737, 1305, 1034}, {931, 618, 635,
                    1076, 588, 639, 1198, 718}, {830, 712, 613, 679, 488, 593, 852,
                        711}};
    
    multiply(E,F,H,0,0,0,0,0,0,8,2);
    assert(isEqual(H,G));
    free(E);
    free(F);
    free(G);
    free(H);
    
    Matrix* I = new Matrix();
    initMatrix(I,4);
    Matrix* J = new Matrix();
    initMatrix(J,4);
    Matrix* K = new Matrix();
    initMatrix(K,4);
    Matrix* L = new Matrix();
    initMatrix(L,4);
    
    I->matrix = {{16, 9, 0, 5}, {19, 6, 9, 4}, {12, 11, 16, 12}, {20, 6, 16, 2}};
    J->matrix = {{7, 12, 11, 13}, {8, 5, 13, 20}, {18, 13, 7, 4}, {6, 1, 19, 11}};
    
    K->matrix = {{214, 242, 388, 443}, {367, 379, 426, 447}, {532, 419, 615,
        572}, {488, 480, 448, 466}};
    
    multiply(I,J,L,0,0,0,0,0,0,4,2);
    
    assert(isEqual(K, L));
    free(I);
    free(J);
    free(K);
    free(L);
    
    cout << "all Strassen tests pass" << endl;
}



int main(){
    testStrasMult();
	testConvMult();
    
    return 0;
}


