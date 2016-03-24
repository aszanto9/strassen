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
#include <random>
#include <algorithm>

using namespace std;
int threads = 0;

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

int findOptDim(int n, int threshold){
    int counter = 0;
    while (n > threshold){
        if (n%2 == 0)
            n /= 2;
        else
            n = (n+1)/2;
        counter ++;
    }
    return n*pow(2,counter);
}

void initPadding(Matrix* M, int newdim){
    M->dimension = newdim;

    M->matrix.resize(newdim);
    for (int i = 0; i < newdim; i++){
        M->matrix[i].resize(newdim);
    }
}

void removePadding(Matrix* M, int newdim){
    M->dimension = newdim;

    M->matrix.resize(newdim);
    for (int i = 0; i < newdim; i++){
        M->matrix[i].resize(newdim);
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
    for (int j = 0; j < dimension; ++j)
		for (int i = 0; i < dimension; ++i)
			for (int k = 0; k < dimension; ++k)
                if (k == 0) 
                    C->matrix[topC + i][leftC + j] = A->matrix[topA + i][leftA + k] * B->matrix[topB + k][leftB + j]; 
				else
                    C->matrix[topC + i][leftC + j] += A->matrix[topA + i][leftA + k] * B->matrix[topB + k][leftB + j];
}

void multiply(Matrix*, Matrix*, Matrix*, int , int , int , int , int , int , int , int); // forward declare for mutual recursion

void strassenMult(Matrix* A, Matrix* B, Matrix* C, int topA, int leftA, int topB, int leftB, int topC, int leftC, int dimension) {
    
    {
    // 1. C12 = A21 - A11
    thread t1(subtract, A, A, C, topA + dimension/2, leftA, topA, leftA, topC, leftC + dimension/2, dimension/2);
    // 2. C21 = B11 + B12
    add(B,B,C,topB,leftB,topB,leftB + dimension/2,topC + dimension/2,leftC,dimension/2);
    t1.join();
    threads++;
    }

    // 3. C22 = C12 * C21
    multiply(C,C,C,topC,leftC + dimension/2,topC + dimension/2,leftC,topC + dimension/2,leftC + dimension/2,dimension/2,2);
    
    {
    // 4. C12 = A12 - A22
    thread t2(subtract, A,A,C,topA,leftA + dimension/2,topA + dimension/2,leftA + dimension/2,topC,leftC + dimension/2,dimension/2);
    // 5. C21 = B21 + B22
    add(B, B, C, topB + dimension/2,leftB,topB + dimension/2,leftB + dimension/2,topC + dimension/2,leftC,dimension/2);
    t2.join();
    threads++;
    }

    //6. C11 = C12 * C21
    multiply(C,C,C,topC,leftC + dimension/2,topC + dimension/2,leftC,topC,leftC,dimension/2,2);
    
    Matrix* T2 = new Matrix();
    initMatrix(T2, dimension/2); // TODO deal with non-power of 2 case
    {
        
    // 7. C12 = A11 + A22
    thread t3(add,A, A, C, topA, leftA, topA + dimension/2, leftA + dimension/2, topC, leftC + dimension/2, dimension/2);
    //8. C21 = B11 + B22
    thread t4(add,B,B,C,topB,leftB,topB + dimension/2,leftB + dimension/2,topC + dimension/2,leftC,dimension/2);
    //12. T2 = A21 + A22
    add(A,A,T2,topA + dimension/2,leftA,topA + dimension/2,leftA + dimension/2,0,0,dimension/2);
    t3.join();
    t4.join();
    threads++;
    threads++;
    }
    

    Matrix* T1 = new Matrix();
    initMatrix(T1, dimension/2); // TODO deal with non-power of 2 case
    //9. T1 = C12*C21
    multiply(C,C,T1,topC,leftC + dimension/2,topC + dimension/2,leftC,0,0,dimension/2,2);
    
    {
    //10. C11 = T1 + C11
    thread t5(add,T1,C,C,0,0,topC,leftC,topC,leftC,dimension/2);
    //11. C22 = T1 + C22
    thread t6(add,T1,C,C,0,0,topC + dimension/2,leftC + dimension/2,topC + dimension/2,leftC + dimension/2,dimension/2);
    //13. C21 = T2 * B11
    multiply(T2,B,C,0,0,topB,leftB,topC + dimension/2,leftC,dimension/2,2);
    t5.join();
    t6.join();
    threads++;
    threads++;
    }

    {
    //14. C22 = C22 - C21
    thread t7(subtract,C,C,C,topC + dimension/2,leftC + dimension/2,topC + dimension/2,leftC,topC + dimension/2,leftC + dimension/2,dimension/2);
    //15. T1 = B21 - B11
    subtract(B,B,T1,topB + dimension/2,leftB,topB,leftB,0,0,dimension/2);
    t7.join();
    threads++;
    }

    //16. T2 = A22 * T1
    multiply(A,T1,T2,topA + dimension/2,leftA + dimension/2,0,0,0,0,dimension/2,2);
    
    {
    //17. C21 = C21 + T2
    thread t8(add,C,T2,C,topC + dimension/2,leftC,0,0,topC + dimension/2,leftC,dimension/2);
    //18. C11 = C11 + T2
    thread t9(add,C,T2,C,topC,leftC,0,0,topC,leftC,dimension/2);
    //19. T1 = B12 - B22
    subtract(B,B,T1,topB,leftB + dimension/2,topB + dimension/2,leftB + dimension/2,0,0,dimension/2);
    t8.join();
    t9.join();
    }
    threads++;
    threads++;
    
    //20. C12 = A11 * T1
    multiply(A,T1,C,topA,leftA,0,0,topC,leftC + dimension/2,dimension/2,2);
    
    {
    //21. C22 = C22 + C12
    thread t10(add,C,C,C,topC + dimension/2,leftC + dimension/2,topC,leftC + dimension/2,topC + dimension/2,leftC + dimension/2,dimension/2);
    //22. T2 = A11 + A12
    add(A,A,T2,topA,leftA,topA,leftA + dimension/2,0,0,dimension/2);
    t10.join();
    threads++;
    }

    //23. T1 = T2 * B22
    multiply(T2,B,T1,0,0,topB + dimension/2,leftB + dimension/2,0,0,dimension/2,2);
    {
    //24. C12 = C12 + T1
    thread t11(add,C,T1,C,topC,leftC + dimension/2,0,0,topC,leftC + dimension/2,dimension/2);
    //25. C11 = C11 - T1
    subtract(C,T1,C,topC,leftC,0,0,topC,leftC,dimension/2);
    t11.join();
    }
    
    threads++;
    
    
    delete(T1);
    delete(T2);
}

void multiply(Matrix* A, Matrix* B, Matrix* C, int topA, int leftA, int topB, int leftB, int topC, int leftC, int dimension, int threshold){
    if (dimension > threshold)
        strassenMult(A, B, C, topA, leftA, topB, leftB, topC, leftC, dimension);
    else
        convMult(A, B, C, topA, leftA, topB, leftB, topC, leftC, dimension);
}

// final multiplication method- returns null if A and B are not of the same dimension
Matrix* multiply(Matrix* A, Matrix* B, int threshold){
    if (A->dimension != B->dimension)
        return NULL;
    int dimension = A->dimension;
    Matrix* C = new Matrix();
    initMatrix(C, A->dimension);
    
    int padding = findOptDim(dimension, threshold);
    initPadding(A, padding);
    initPadding(B, padding);
    initPadding(C, padding);
    multiply(A,B,C,0,0,0,0,0,0,padding,threshold);
    
    removePadding(A, dimension);
    removePadding(B, dimension);
    removePadding(C, dimension);
    return C;
}

void populateRandomMatrix(Matrix* M, int low, int high){
    random_device r;
    mt19937 mtgen(r());
    uniform_int_distribution<> dist(low,high);
    for (int i = 0; i < M->dimension; i++)
        for (int j = 0; j < M->dimension; j++)
            M->matrix[i][j] = dist(mtgen);
}

void findOptimalThreshold() {
    for (int i = 2; i <= 512; i*=2){
        
        
        
        double total = 0;
        //cout << "multiplying matrices, n = " << i << endl;
        for (int j = 0; j < 5; j ++){
            Matrix* m1 = new Matrix();
            Matrix* m2 = new Matrix();
            initMatrix(m1, 512);
            initMatrix(m2, 512);
            //cout << "populating m1" << endl;
            populateRandomMatrix(m1, 0, 1);
            //cout << "populating m2" << endl;
            populateRandomMatrix(m2, 0, 1);
            clock_t start;
            start = clock();
            
            Matrix* m3 = multiply(m1, m2, i);
            total += (std::clock() - start) / (double)(CLOCKS_PER_SEC);
            delete(m1);
            delete(m2);
            delete(m3);
        }
        cout << i << "\t" << total / 5 << endl;
        //cout << "finished multiplying.\n" << endl;
        
    }

    
}


void testRandMatrix(){
    Matrix* m = new Matrix();
    initMatrix(m,10);
    populateRandomMatrix(m, 0,1);
    printMatrix(*m);
    free(m);
}

void testInitMatrix(){
    Matrix* m = new Matrix();
    initMatrix(m,3);
    printMatrix(*m);
    free(m);
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
    //First test
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


    //second test powers of 2
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


    //third test non powers of 2, positive and negative
    Matrix* M = new Matrix();
    initMatrix(M,14);
    Matrix* N = new Matrix();
    initMatrix(N,14);
    Matrix* O = new Matrix();
    initMatrix(O,14);
    
    M->matrix = {{-12, -2, -18, 12, 6, -13, -15, 12, -17, -18, 
  5, -19, -13, -7}, {14, -11, 20, 9, 2, -4, 11, -5, 12, 11, 1, 15, 0, 
  11}, {7, 18, 6, 12, 13, -14, 4, 9, 3, 13, -1, -12, 11, 
  18}, {17, -12, 19, 1, -16, 10, 20, -15, 1, 
  13, -15, -12, -13, -4}, {-4, -12, -9, 7, 14, 4, 6, 15, 4, 11, 
  19, -8, 11, -20}, {0, -3, 9, -4, 0, 19, -9, 15, 20, 0, -3, 8, -13, 
  17}, {2, 19, 15, -2, 16, -7, -11, 17, 8, -20, 17, -13, 9, 3}, {-15, 
  13, 10, 14, -1, 19, 19, -6, 1, -3, -6, -10, -19, 
  11}, {-13, -18, -12, -6, -5, -5, 14, -8, 5, 6, 10, 19, -15, 
  13}, {9, -18, 6, 10, 8, -3, -18, -5, 18, 17, 16, 
  6, -18, -12}, {-13, -15, -2, -13, 0, 20, -1, -16, 1, 3, 15, 13, 6, 
  10}, {4, -9, 5, 11, -17, -4, -1, 10, 17, 8, 17, 1, -7, 18}, {1, -18,
   1, -2, 7, -7, -1, 2, -20, 7, 7, -13, -16, 9}, {10, -1, 
  16, -20, -4, -1, -17, -1, -15, 16, 1, 9, 16, 13}};
    
    N->matrix = {{-10, 6, 4, -16, 9, 12, 0, -2, 4, -15, 18, 10, -3, -6}, {12, 14, 
  0, -3, 2, -17, 8, -19, 20, -14, -20, -6, 7, 17}, {7, -15, -14, 
  13, -12, 18, 3, 6, -18, -12, -10, -6, 19, -13}, {7, 19, 12, 4, -1, 
  5, -16, 13, 4, 12, -18, -4, 16, 8}, {-18, 8, -15, -4, 7, 0, -3, 
  6, -14, -18, 8, 10, 14, 6}, {-11, 17, 4, -11, -5, -3, 6, -20, 4, 
  18, -5, -16, 2, 8}, {6, 6, -9, 18, -4, 5, -9, 18, -19, 4, -5, 15, 3,
   16}, {-11, -7, -10, 1, 12, -9, -8, -1, -20, 6, 
  14, -13, -18, -15}, {-19, 8, -15, -8, -6, 13, -20, -17, 3, 
  11, -4, -15, 2, -7}, {-7, -9, -5, 2, -15, 17, -19, 5, -15, -5, 11, 
  17, 12, -15}, {16, 14, 12, -12, -13, 0, -16, -12, 20, -20, -8, 16, 
  16, 4}, {3, 7, 13, 11, 6, -1, -17, -10, -14, -5, -14, 15, 
  11, -12}, {4, 16, -2, -9, -6, 7, 6, 17, 13, -9, 10, -16, 
  0, -19}, {18, -17, -12, 2, 7, -15, 9, -7, 10, 17, 14, 10, 20, 3}};
    
    O->matrix = {{161, -75, 489, -193, 625, -1012, 465, 281, 539, 134, 12, -279, -850,
   743}, {14, -221, -371, 435, -298, 1035, -882, 325, -850, -113, 30, 
  722, 1105, -620}, {285, -62, -805, -38, 56, 6, -120, 464, 119, -446,
   439, 159, 789, -54}, {-221, -608, -278, 489, -550, 1175, 56, 
  555, -859, 510, 91, 253, 74, 101}, {-732, 846, 23, -431, -415, 
  536, -998, 536, -378, -436, 
  299, -36, -223, -310}, {-579, -412, -577, -131, 
  175, -138, -271, -1222, -402, 928, 142, -504, 247, -367}, {146, 
  323, -604, -596, 193, -431, 352, -458, 720, -973, -76, -787, 216, 
  15}, {461, 69, -348, 659, -245, -385, 192, -273, -144, 
  1045, -1049, -193, 719, 1361}, {359, -451, 189, 
  623, -120, -91, -831, -163, -482, 613, -111, 1260, 406, 165}, {-833,
   15, 247, -398, -465, 1107, -1383, -416, -393, -414, -64, 578, 
  585, -630}, {238, 149, 256, -152, -532, 20, -73, -552, 305, 
  280, -72, 372, 634, -103}, {390, -357, -54, -38, -297, 
  300, -938, -422, 177, 599, 85, 229, 540, -450}, {192, -1007, -74, 
  185, 80, -51, 182, 568, -490, -76, 777, 943, 96, 
  149}, {366, -870, -119, -188, -165, 237, 567, -60, -6, -821, 888, 
  346, 393, -1238}};
    
    Matrix* P = multiply(M,N,2);
    
    assert(isEqual(O, P));
    
    free(M);
    free(N);
    free(O);
    free(P);

    
    cout << "all Strassen tests pass" << endl;
}

void testfindOptDim(){
    assert(findOptDim(258,17) == 272);
    assert(findOptDim(513,15) == 576);
    assert(findOptDim(1025,15) == 1152);

           cout << "all findOptDim tests pass" << endl;
}

void testInitPadding(){
    Matrix* A = new Matrix();
    initMatrix(A,2);
    Matrix* B = new Matrix();
    initMatrix(B,4);
    A->matrix = {{1,1},{1,1}};
    B->matrix = {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}};

    initPadding(A,4);
    assert(isEqual(A,B));
    cout << "all initPadding tests pass" << endl;
}


void testPowers2(){
    for (int i = 256; i <= 256; i *= 2){
        Matrix* m1 = new Matrix();
        initMatrix(m1, i);
        Matrix* m2 = new Matrix();
        initMatrix(m2, i);
        cout << "populating m1" << endl;
        populateRandomMatrix(m1, -10, 10);
        cout << "populating m2" << endl;
        populateRandomMatrix(m2, -10, 10);
        
        cout << "multiplying matrices, n = " << i << endl;
        
        multiply(m1, m2, 15);
        cout << "finished multiplying.\n" << endl;
        free(m1);
        free(m2);
    }

}
int main(){
    //testStrasMult();
//    testConvMult();
//    testfindOptDim();
//    testInitPadding();
//    testRandMatrix();
    testPowers2();
    //findOptimalThreshold();
    cout << threads;
    
        return 0;
}


