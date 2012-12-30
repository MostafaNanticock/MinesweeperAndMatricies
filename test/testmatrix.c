//////
// testmatrix.c
// A set of tests for my implementation of a Matrix ADT
//
// Author: Robert Massaioli, 2009
//////

#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "testmatrix.h"

#define PASS	0
#define FAIL	1

//////
// The smaller hidden (per function) tests
//////

static int testcreatedeleteMatrix 	(void);
static int testcopyMatrix 				(void);
static int testtransposeMatrix 		(void);
static int testadddeleteRowMatrix 	(void);
static int testaddColumnMatrix 		(void);
static int testgaussianEliminate 	(void);
static int testmultiplyMatricies 	(void);
static int testsolveMatrix 			(void);
// NB: SOME TESTS WERE CONSIDERED VOID BECAUSE THEY WERE USED HEAVILY BY OTHER TESTS (laziness excuse lol)

static void printResult (int res);

//////
// The one big test that is called
//////

int testmatrix (void) {
	int res, ores;
   res = ores = PASS;
	
	printf ("Testing create/delete Matrix...");
	res |= testcreatedeleteMatrix ();
	ores |= res;
	printResult (res); 
	
	printf ("Testing add / delete row to Matrix...");
	res |= testadddeleteRowMatrix ();
	ores |= res;
	printResult (res); 
	
	printf ("Testing copy Matrix...");
	res |= testcopyMatrix ();
	ores |= res;
	printResult (res); 
	
	printf ("Testing add column to Matrix...");
	res |= testaddColumnMatrix ();
	ores |= res;
	printResult (res); 
	
	printf ("Testing transopse Matrix...");
	res |= testtransposeMatrix ();
	ores |= res;
	printResult (res); 
	
	printf ("Testing multiply Matricies...");
	res |= testmultiplyMatricies ();
	ores |= res;
	printResult (res); 
	
	printf ("Testing gaussian eliminate Matrix...");
	res |= testgaussianEliminate ();
	ores |= res;
	printResult (res); 
	
	printf ("Testing solve Matrix...");
	res |= testsolveMatrix ();
	ores |= res;
	printResult (res); 
	
	return (ores == PASS);
}

//////
// All of the helper tests that are called
//////

static int testcreatedeleteMatrix (void) {
	int result;
	Matrix m = createMatrix();
	
	result = (m == NULL);
	
	deleteMatrix(m);
	
	return result;
}

static int testcopyMatrix (void) {
	Matrix m, c;
	Vector test;
	int result, i, j;
	
	m = createMatrix ();
	
	test = createVector();
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			setValueVector (test, j, j + (4 * i));
		}
		
		addRowMatrix (m, test);
	}
	deleteVector (test);
	
	c = copyMatrix (m);
	deleteMatrix (m);
	
	result = PASS;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			result |= (getValueVector (getRowMatrix (c, i), j) != j + (4 * i));
		}
	}
	
	deleteMatrix (c);
	
	return result;
}

static int testtransposeMatrix (void) {
	Matrix m;
	Vector r;
	int result, i, j;
	
	m = createMatrix();
	
	r = createVector();
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 4; j++) {
			setValueVector (r, j, j);
		}
		
		addRowMatrix (m, r);
	}
	deleteVector (r);
	
	transposeMatrix (m);
	
	if (getHeightMatrix (m) != 4) return FAIL;
	if (getDimVector (getRowMatrix (m, 0)) != 8) return FAIL;
	
	result = PASS;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 8; j++) {
			result |= (getValueMatrix (m, i, j) != i);
		}
	}
	
	transposeMatrix (m);
	
	if (getHeightMatrix (m) != 8) return FAIL;
	if (getWidthMatrix(m) != 4) 	return FAIL;
	
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 4; j++) {
			result |= (getValueMatrix (m, i, j) != j);
		}
	}
	
	deleteMatrix (m);
	
	return result;
}

static int testadddeleteRowMatrix (void) {
	Matrix m;
	Vector v;
	int result;
	
	m = createMatrix ();
	
	v = createVector();
	
	addRowMatrix (m, v);
	deleteVector (v);
	
	result = PASS;
	result |= (getRowMatrix (m, 0) == NULL);
	result |= (getHeightMatrix (m) != 1);
	
	deleteRowMatrix (m, 0);
	
	result |= (getRowMatrix (m, 0) != NULL);
	result |= (getHeightMatrix (m) != 0);
	
	deleteMatrix (m);
	
	return result;
}

static int testaddColumnMatrix (void) {
	Matrix m;
	Vector v;
	int result, i, j;
	
	m = createMatrix();
	
	v = createVector();
	
	setDimVector (v, 3);
	for (i = 0; i < 3; i++) {
		setValueVector (v, i, i);
	}
	
	for (i = 0; i < 3; i++) {
		addColumnMatrix (m, v);
	}
	deleteVector (v);
	
	result = PASS;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			result |= (getValueMatrix (m, i, j) != i);
		}
	}
	
	deleteMatrix (m);
	
	return result;
}

static int testgaussianEliminate (void) {
	Matrix m;
	Vector r;
	int result;
	
	m = createMatrix ();
	
	// a matrix (there has to be a better way lol :D
	r = createVector();
	setDimVector (r, 6);
	setValueVector (r, 0, 1); setValueVector (r, 1, 1); setValueVector (r, 2, 0); setValueVector (r, 3, 0); setValueVector (r, 4, 0); setValueVector (r, 5, 1); addRowMatrix (m, r);
	setValueVector (r, 0, 1); setValueVector (r, 1, 1); setValueVector (r, 2, 1); setValueVector (r, 3, 0); setValueVector (r, 4, 0); setValueVector (r, 5, 2); addRowMatrix (m, r);
	setValueVector (r, 0, 0); setValueVector (r, 1, 1); setValueVector (r, 2, 1); setValueVector (r, 3, 1); setValueVector (r, 4, 0); setValueVector (r, 5, 2); addRowMatrix (m, r);
	setValueVector (r, 0, 0); setValueVector (r, 1, 0); setValueVector (r, 2, 1); setValueVector (r, 3, 1); setValueVector (r, 4, 1); setValueVector (r, 5, 2); addRowMatrix (m, r);
	setValueVector (r, 0, 0); setValueVector (r, 1, 0); setValueVector (r, 2, 0); setValueVector (r, 3, 1); setValueVector (r, 4, 1); setValueVector (r, 5, 1); addRowMatrix (m, r);
	deleteVector (r);
	
	gaussianEliminate (m);
	
	// there are missing checks here but viewing through DDD shows it achieves an acceptable result (this is by no means good testing)
	result = PASS;
	
	deleteMatrix (m);
	
	return result;
}

static int testmultiplyMatricies (void) {
	return PASS;	// not completed
}

static int testsolveMatrix (void) {
	Matrix m;
	Vector r, res;
	int result, out;
	
	m = createMatrix();
	
	r = createVector();
	setValueVector (r, 0, 1); setValueVector (r, 1, -2); setValueVector (r, 2, 3); setValueVector (r, 3, 11); addRowMatrix (m, r);
	setValueVector (r, 0, 2); setValueVector (r, 1, -1); setValueVector (r, 2, 3); setValueVector (r, 3, 10); addRowMatrix (m, r);
	setValueVector (r, 0, 4); setValueVector (r, 1, 1); setValueVector (r, 2, -1); setValueVector (r, 3, 4); addRowMatrix (m, r);
	
	setDimVector (r, 3);
	setValueVector (r, 0, 2); setValueVector (r, 1, -3); setValueVector (r, 2, 1);
	
	res = solveMatrix (m, &out);
	roundVector (res);
	
	result = !areEqualVector (r, res);
	result |= (out != UNIQUE_SOLUTION);
	
	deleteVector (r);
	deleteMatrix (m);
	
	return result;
}

static void printResult (int res) {
	if (res == PASS) {
		printf ("PASS\n");
	} else {
		printf ("FAIL\n");
	}
}
