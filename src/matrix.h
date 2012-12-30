// An implementation of a matrix ADT
// The matrix should be able to be created, deleted
// Have rows and coloumns added
// Be gaussian eliminated

#ifndef __RMMATRIXADT
#define __RMMATRIXADT

#include "vector.h"

#define INFINITE_SOLUTIONS 	1
#define NO_SOLUTIONS				-1
#define UNIQUE_SOLUTION			0

typedef struct matrix* Matrix;

Matrix createMatrix (void);
Matrix copyMatrix (Matrix m);
void transposeMatrix (Matrix m);
void deleteMatrix (Matrix toDelete);

void addRowMatrix (Matrix m, Vector r);
Vector getRowMatrix (Matrix m, int row);
void deleteRowMatrix (Matrix m, int row);
void addColumnMatrix (Matrix m, Vector r);
void clearMatrix (Matrix m);

double getValueMatrix (Matrix m, int row, int col);

void gaussianEliminate (Matrix mat);
Matrix multiplyMatricies (Matrix x, Matrix y);
Vector solveMatrix (Matrix m, int *result);

int getHeightMatrix (Matrix m);
int getWidthMatrix (Matrix m);

#endif
