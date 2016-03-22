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

void testInitMatrix(){
    Matrix* m = new Matrix();
    initMatrix(m,3);
    for (int i = 0; i < 3; i ++){
        for (int j = 0; j < 3; j++){
            cout << m->matrix[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

Matrix conventional(Matrix A, Matrix B, int dimension) {

	Matrix* AB = new Matrix();
	initializ(AB, dimension);

	for (int i = 0; i < dimension; ++i)
	{
		for (int j = 0; j < dimension; ++i)
		{
			for (int i = 0; k < dimension; ++i)
			{	
				AM->matrix[i][j] += A->matrix[i][k]*B->matrix[k][j]
			}
		}
	}
	return AB*
}



int main(){
        return 0;
        
}


