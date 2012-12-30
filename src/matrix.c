#include <stdlib.h>
#include <assert.h>
#include "vector.h"
#include "matrix.h"

struct matrix {
	Vector *rows;
	int rowCount;
};

Matrix createMatrix (void) {
	struct matrix *m = malloc (sizeof (struct matrix));
	
	assert (m != NULL);
	
	m->rows = NULL;
	m->rowCount = 0;
	
	return m;
}

Matrix copyMatrix (Matrix m) {
	Matrix new = createMatrix();
	int i;
	
	for (i = 0; i < getHeightMatrix (m); i++) {
		addRowMatrix (new, m->rows[i]);
	}
	
	return new;
}

void transposeMatrix (Matrix m) {
	Matrix t = copyMatrix (m);
	int i;
	
	clearMatrix (m);
	
	for (i = 0; i < getHeightMatrix (t); i++) {
		addColumnMatrix (m, t->rows[i]);
	}

   deleteMatrix(t);
}

void deleteMatrix (Matrix toDelete) {
	clearMatrix (toDelete);
	
	free (toDelete);
}

void addRowMatrix (Matrix m, Vector r) {
	assert (r != NULL); assert (m != NULL);
	
	m->rowCount++;
	if (m->rows == NULL) {	
		// create a row
		m->rows = malloc (m->rowCount * sizeof (Vector*));
	} else {
		assert (getDimVector(r) == getDimVector (m->rows[0]));	// making sure that the vector being added is the same dimension as the previous vectors
		m->rows = realloc (m->rows, m->rowCount * sizeof (Vector));
	}
	
	m->rows[m->rowCount - 1] = copyVector (r);
}

Vector getRowMatrix (Matrix m, int row) {
	Vector res = NULL;
	
	assert (row >= 0); 
	
	if (row < getHeightMatrix (m)) {
		res = m->rows[row];
	}
	
	return res;
}

void deleteRowMatrix (Matrix m, int row) {
	int i;
	
	deleteVector (m->rows[row]);
	
	for (i = row + 1; i < m->rowCount; i++) {
		m->rows[i - 1] = m->rows[i];
	}
	
	m->rowCount--;
}

void addColumnMatrix (Matrix m, Vector r) {
	int i, rowLen;
	Vector temp;
	assert (m != NULL); assert (r != NULL);
	
	if (m->rows == NULL) {
		m->rowCount = getDimVector (r);
		m->rows = malloc (m->rowCount * sizeof (Vector));
		
		temp = createVector();
		for (i = 0; i < m->rowCount; i++) {
			setValueVector (temp, 0, getValueVector (r, i));
			m->rows[i] = copyVector(temp);
		}
		deleteVector (temp);
	} else {
		assert (getDimVector (r) == getHeightMatrix (m));
		
		rowLen = getDimVector (m->rows[0]);
		for (i = 0; i < m->rowCount; i++) {
			setValueVector (m->rows[i], rowLen, getValueVector (r, i));
		}
	}
}

void clearMatrix (Matrix m) {
	int i;
	
	if (m->rows != NULL) {
		for (i = 0; i < getHeightMatrix (m); i++) {
			deleteVector (m->rows[i]);
		}
	
		free (m->rows);
	}
	
	m->rows = NULL;
}

double getValueMatrix (Matrix m, int row, int col) {
	assert (row >= 0); assert (col >= 0);
	assert (row < getHeightMatrix (m)); assert (col < getWidthMatrix (m));
	
	return getValueVector (getRowMatrix (m, row), col);
}

Matrix multiplyMatricies (Matrix x, Matrix y) {
	Matrix result = createMatrix();
	Vector temp = createVector();
	int i, j;
	transposeMatrix (y);
	
	for (j = 0; j < getHeightMatrix (x) ; j++) {
		for (i = 0; i < getHeightMatrix (y) ; i++) {
			setValueVector (temp, i, dotProduct (x->rows[j], y->rows[i]));
		}
		addRowMatrix (result, temp);
	}
	
	deleteVector (temp);
	
	transposeMatrix (y);
	
	return result;
}

/*

	Wikipedia Sourcecode for Gaussian Elimination

i := 1
j := 1
while (i ≤ m and j ≤ n) do
  Find pivot in column j, starting in row i:
  maxi := i
  for k := i+1 to m do
    if abs(A[k,j]) > abs(A[maxi,j]) then
      maxi := k
    end if
  end for
  if A[maxi,j] ≠ 0 then
    swap rows i and maxi, but do not change the value of i
    Now A[i,j] will contain the old value of A[maxi,j].
    divide each entry in row i by A[i,j]
    Now A[i,j] will have the value 1.
    for u := i+1 to m do
      subtract A[u,j] * row i from row u
      Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
    end for
    i := i + 1
  end if
  j := j + 1
end while

*/

void gaussianEliminate (Matrix mat) {
	int i, j, m, n, k , u, maxi;
	double mulVal;
	Vector temp;
	
	if (mat->rows != NULL) {
		m = getDimVector (mat->rows[0]);
		n = getHeightMatrix (mat);
		
		i = 0; j = 0;
		while ((i < m) && (j < n)) {
			maxi = i;
			for (k = i + 1; k < n; k++) {
				if (abs(getValueVector(mat->rows[k], j)) > abs(getValueVector(mat->rows[maxi], j))) {
					maxi = k;
				}
			}
			
			if (getValueVector(mat->rows[maxi], j) != 0) {
				temp = mat->rows[i]; mat->rows[i] = mat->rows[maxi]; mat->rows[maxi] = temp;
				multiplyVector (mat->rows[i], 1.0 / getValueVector(mat->rows[i], j));
				
				for (u = i + 1; u < n; u++) {
					mulVal = getValueVector(mat->rows[u], j);
					if (mulVal != 0) {
						multiplyVector (mat->rows[i], -mulVal);
						addVector (mat->rows[u], mat->rows[i]);
						multiplyVector (mat->rows[i], 1.0 / getValueVector(mat->rows[i], j));	
					}
				}
				
				i++;
			}
			
			j++;
		}
	}
}

// tries to solve the matrix and returns its success
// FAIL:
//		return NULL and result = (INFINITE_SOLUTIONS or NO_SOLUTIONS)
// SUCCESS:
//		return VectorSolution and result = (UNIQUE_SOLUTION)
Vector solveMatrix (Matrix m, int* result) {
	Vector solution = createVector();
	int maxVar, deleteRow;
	int i, j;
	double var;
	
	assert (m != NULL);								// assert that the matrix exist
	assert (m->rows != NULL);						// that it has numbers in it
	assert (getDimVector (m->rows[0]) > 1);	// and that is has enough numbers to be solvable
	
	*result = INFINITE_SOLUTIONS;
	
	gaussianEliminate (m);
	// i think that after you gaussian eliminate then and identical rows will have one brought to zero, 
	// delete them for this to make handling that case better (also, the algorithm will push those rows to the bottom)
   deleteRow = 1;
	for (i = getHeightMatrix(m) - 1; (i >= 0) && deleteRow; i--) {
		deleteRow = 1;
		for (j = 0; (j < getWidthMatrix (m)) && deleteRow; j++) {
			if (getValueMatrix (m, i, j) != 0) {
				deleteRow = 0;
				if (j == getWidthMatrix (m) - 1) {
					*result = NO_SOLUTIONS;
					return NULL;
				}
			}
		}
		if (deleteRow) deleteRowMatrix (m, i);
	}	
			
	// for every row
	//		if every item in that row is zero then delete the row (and if any row only has the last row with an item then NO_SOLUTIONS)
	
	if (getHeightMatrix (m) == getDimVector (m->rows[0]) - 1) {
		maxVar = getDimVector (m->rows[0]) - 1;
		setDimVector (solution, maxVar);
	
		for (j = maxVar - 1; j >= 0; j--) {
			var = getValueVector (m->rows[j], maxVar);
		
			for (i = j + 1; i < maxVar; i++) {
				var -= getValueVector (solution, i) * getValueVector (m->rows[j], i);
			}
		
			setValueVector (solution, j, var);
		}
	
		*result = UNIQUE_SOLUTION;
	}
	
	return solution;
}

int getHeightMatrix (Matrix m) {
	return m->rowCount;
}

int getWidthMatrix (Matrix m) {
	int width = 0;
	
	if (m->rows != NULL) {
		width = getDimVector (getRowMatrix (m, 0));
	}
	
	return width;
}
