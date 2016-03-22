Matrix conventional(Matrix A, Matrix B, int dimension) {

	Matrix* AB = new Matrix();
	initialize(AB, dimension);

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