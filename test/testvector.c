#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "testvector.h"

#define PASS	0
#define FAIL	1

static int testcreatedeleteVector 	(void);
static int testcopyVector 		(void);
static int testareEqualVector (void);

static int testsetgetValueVector 		(void);

static int testaddVector 		(void);
static int testmultiplyVector (void);
static int testdotProduct 		(void);
static int testlengthVector 	(void);
static int testgetsetDimVector 	(void);

// the projection of a onto b
static int testprojection 		(void);

static void printResult (int res);

int testVector (void) {
	int ores, res;
	ores = PASS;
	
	printf ("Testing create/delete Vector...");
	res = testcreatedeleteVector ();
	ores |= res;
	printResult (res); 
	
	printf ("Testing are equal Vector...");
	res = testareEqualVector ();
	ores |= res;
	printResult (res);
	
	printf ("Testing get/set Vector...");
	res = testsetgetValueVector 		();
	ores |= res;
	printResult (res); 
	
	printf ("Testing copy Vector...");
	res = testcopyVector		();
	ores |= res;
	printResult (res); 

	printf ("Testing add Vector...");
	res = testaddVector 		();
	ores |= res;
	printResult (res); 
	
	printf ("Testing multiply Vector...");
	res = testmultiplyVector();
	ores |= res;
	printResult (res); 
	
	printf ("Testing dot product Vector...");
	res = testdotProduct 	();
	ores |= res;
	printResult (res); 
	
	printf ("Testing length Vector...");
	res = testlengthVector 	();
	ores |= res;
	printResult (res); 
	
	printf ("Testing dimension Vector...");
	res = testgetsetDimVector 	();
	ores |= res;
	printResult (res); 
	
	printf ("Testing project (a proj b) Vector...");
	res = testprojection		();
	ores |= res;
	printResult (res); 
	
	return (ores == PASS);
}

static int testcreatedeleteVector 	(void) {
	Vector t = createVector();
	int result = FAIL;
	
	if (t != NULL) result = PASS;

	deleteVector (t);
	
	return result;
}

static int testcopyVector 		(void) {
	int result;
	
	Vector c, t = createVector ();
	
	setValueVector (t, 0, 10);
	setValueVector (t, 5, 9.5);
	setValueVector (t, 10, 8.75);
	
	c = copyVector (t);
	deleteVector (t);
	
	result = PASS;
	result |= !(getValueVector (c, 0) == 10);
	result |= !(getValueVector (c, 3) == 0);
	result |= !(getValueVector (c, 5) == 9.5);
	result |= !(getValueVector (c, 7) == 0);
	result |= !(getValueVector (c, 10) == 8.75);
	
	deleteVector (c);
	
	return result;
}

static int testareEqualVector (void) {
	Vector a, b;
	int result, output;
	
	output = PASS;
	
	a = createVector();
	b = createVector();
	
	// test 1
	setDimVector (a, 5);
	setDimVector (b, 5);
	result = areEqualVector (a, b);
	output |= !result;		// it should return true
	
	// test 2
	setValueVector (a, 0, 5);
	setValueVector (b, 0, 5);
	result = areEqualVector (a, b);
	output |= !result;
	
	// test 2
	setValueVector (a, 4, 9.5);
	setValueVector (b, 4, 10.5);
	result = areEqualVector (a, b);
	output |= result;
	
	deleteVector (a);
	deleteVector (b);
	
	return output;
}

// so long as the input of set is the output of get all is well
static int testsetgetValueVector 		(void) {
	int i, result;
	Vector t = createVector();
	
	for (i = 0; i < 100; i++) {
		setValueVector (t, i, i);
	}
	
	result = PASS;
	
	for (i = 0; i < 100; i++) {
		result |= (getValueVector (t, i) != i);
	}
	
	deleteVector (t);
	
	return result;	
}

static int testaddVector 		(void) {
	Vector a, b;
	int output, i;
	
	output = PASS;
	
	a = createVector();
	b = createVector();
	
	// test 1
	// set a to all zeroes
	setValueVector (a, 3, 0);
	
	// set b to some values
	setValueVector (b, 3, 10);
	setValueVector (b, 2, 1);
	setValueVector (b, 1, 50);
	setValueVector (b, 0, 9.957);
	
	addVector (a, b);
	output |= !areEqualVector (a, b);
	
	// test 2 - another random test
	addVector (a, b);
	for (i = 0; i < getDimVector (a); i++) {
		output |= (getValueVector (a, i) != getValueVector (b, i) * 2);
	}
	
	deleteVector (a);
	deleteVector (b);
	
	return output;
}

static int testmultiplyVector (void) {
	Vector a = createVector ();
	int i, result;
	
	setDimVector (a, 3);
	setValueVector (a, 0, 50);
	setValueVector (a, 1, 500);
	setValueVector (a, 2, 5000);
	
	multiplyVector (a, 0.1);
	
	result = PASS;
	for (i = 0; i < getDimVector (a); i++) {
		result |= (getValueVector (a, i) != 5 * pow (10, i));
	}
	
	deleteVector (a);
	
	return result;
}

static int testdotProduct 		(void) {
	Vector a, b;
	int result;
	
	a = createVector(); b = createVector();
	
	setDimVector (a, 3); setDimVector (b, 3);
	
	setValueVector (a, 0, 3); setValueVector (b, 0, 10);
	setValueVector (a, 1, 4); setValueVector (b, 1, 12);
	setValueVector (a, 2, 5); setValueVector (b, 2, 15);
	
	result = (dotProduct (a, b) != (3 * 10 + 4 * 12 + 5 * 15));
	
	deleteVector (a); deleteVector (b);
	
	return result;
}

static int testlengthVector 	(void) {
	Vector a;
	int result;
	
	a = createVector();
	
	setDimVector (a, 3); 
	
	setValueVector (a, 0, 3); 
	setValueVector (a, 1, -4); 
	setValueVector (a, 2, 5); 
	
	result = (lengthVector (a) != sqrt (3*3 + 4*4 + 5*5));
	
	deleteVector (a);
	
	return result;
}

static int testgetsetDimVector 	(void) {
	Vector v = createVector();
	int result;
	
	setDimVector (v, 5000);
	result = (getDimVector (v) != 5000);
	
	deleteVector (v);
	return result;
}

static int testprojection 		(void) {
	Vector a, b, c;
	int result;
	
	a = createVector(); b = createVector ();
	
	setValueVector (a, 0, 1); setValueVector (a, 1, -3); setValueVector (a, 2, 2);
	setValueVector (b, 0, -4); setValueVector (b, 1, 1); setValueVector (b, 2, 5);
	
	c = projection (a, b);
	
	multiplyVector (b, 1.0 / 14.0);
	
	deleteVector (a);
	
	result = !areEqualVector (c, b);
	
	deleteVector (c); deleteVector (b);
	
	return result;
}

//////
// The actual helper functions
//////

static void printResult (int res) {
	if (res == PASS) {
		printf ("PASS\n");
	} else {
		printf ("FAIL\n");
	}
}
