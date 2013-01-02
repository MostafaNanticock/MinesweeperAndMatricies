#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "testvector.h"

static bool testcreatedeleteVector 	(void);
static bool testcopyVector 		(void);
static bool testareEqualVector (void);

static bool testsetgetValueVector 		(void);

static bool testaddVector 		(void);
static bool testmultiplyVector (void);
static bool testdotProduct 		(void);
static bool testlengthVector 	(void);
static bool testgetsetDimVector 	(void);

// the projection of a onto b
static bool testprojection 		(void);

static void printResult (int res);

int testVector (void) {
	bool output_result, res;
	output_result = true;
	
	printf ("Testing create/delete Vector...");
	res = testcreatedeleteVector ();
	output_result |= res;
	printResult (res); 
	
	printf ("Testing are equal Vector...");
	res = testareEqualVector ();
	output_result |= res;
	printResult (res);
	
	printf ("Testing get/set Vector...");
	res = testsetgetValueVector 		();
	output_result |= res;
	printResult (res); 
	
	printf ("Testing copy Vector...");
	res = testcopyVector		();
	output_result |= res;
	printResult (res); 

	printf ("Testing add Vector...");
	res = testaddVector 		();
	output_result |= res;
	printResult (res); 
	
	printf ("Testing multiply Vector...");
	res = testmultiplyVector();
	output_result |= res;
	printResult (res); 
	
	printf ("Testing dot product Vector...");
	res = testdotProduct 	();
	output_result |= res;
	printResult (res); 
	
	printf ("Testing length Vector...");
	res = testlengthVector 	();
	output_result |= res;
	printResult (res); 
	
	printf ("Testing dimension Vector...");
	res = testgetsetDimVector 	();
	output_result |= res;
	printResult (res); 
	
	printf ("Testing project (a proj b) Vector...");
	res = testprojection		();
	output_result |= res;
	printResult (res); 
	
	return output_result;
}

static bool testcreatedeleteVector 	(void) {
	Vector<int>* t = new Vector<int>;
	bool result = false;
	
	if (t != NULL) result = true;

   delete t;
	
	return result;
}

static bool testcopyVector 		(void) {
	int result;
	
	Vector<double> c, t;
	
   t.setValue(0, 10);
   t.setValue(5, 9.5);
   t.setValue(10, 8.75);
	
	c = t;
	
	result = true;
	result |= !(c.getValue(0) == 10);
	result |= !(c.getValue(3) == 0);
	result |= !(c.getValue(5) == 9.5);
	result |= !(c.getValue(7) == 0);
	result |= !(c.getValue(10) == 8.75);
	
	return result;
}

static bool testareEqualVector (void) {
	Vector<double> *a, *b;
	bool result, output;
	
	output = true;
	
	a = new Vector<double>;
	b = new Vector<double>;
	
	// test 1
   a->setDimension(5);
   b->setDimension(5);
	result = a->equal(b);
	output |= !result;		// it should return true
	
	// test 2
   a->setValue(0, 5.0);
   b->setValue(0, 5.0);
   result = a->equal(b);
	output |= !result;
	
	// test 2
	a->setValue(4, 9.5);
	b->setValue(4, 10.5);
   result = a->equal(b);
	output |= result;
	
   delete a;
   delete b;
	
	return output;
}

// so long as the input of set is the output of get all is well
static bool testsetgetValueVector 		(void) {
	Vector<int> t;
	
	for (int i = 0; i < 100; i++) {
      t.setValue(i, i);
	}
	
	bool result = true;
	for (int i = 0; i < 100; i++) {
		result |= (t.getValue(i) != i);
	}
	
	return result;	
}

static bool testaddVector 		(void) {
	Vector<double> *a, *b;
	
	bool output = true;
	
	a = new Vector<double>;
	b = new Vector<double>;
	
	// test 1
	// set a to all zeroes
   a->setValue(3, 0.0);
	
	// set b to some values
	b->setValue(3, 10);
	b->setValue(2, 1);
	b->setValue(1, 50);
	b->setValue(0, 9.957);
	
   a->add(b);
	output |= !a->equal(b);
	
	// test 2 - another random test
   a->add(b);
	for (int i = 0; i < a->getDimension(); i++) {
		output |= (a->getValue(i) != b->getValue(i) * 2);
	}
	
   delete a;
   delete b;
	
	return output;
}

static bool testmultiplyVector (void) {
	Vector<double> *a = new Vector<double>;

	a->setDimension(3);
	a->setValue(0, 50);
	a->setValue(1, 500);
	a->setValue(2, 5000);
	
	a->multiply(0.1);
	
	bool result = true;
	for (int i = 0; i < a->getDimension(); i++) {
		result |= (a->getValue(i) != 5 * pow (10, i));
	}
	
   delete a;
	
	return result;
}

static bool testdotProduct 		(void) {
	Vector<int> *a, *b;
	
	a = new Vector<int>; b = new Vector<int>;
	
	a->setDimension(3); b->setDimension(3);
	
	a->setValue(0, 3); b->setValue(0, 10);
	a->setValue(1, 4); b->setValue(1, 12);
	a->setValue(2, 5); b->setValue(2, 15);
	
	bool result = (a->dot(b) == (3 * 10 + 4 * 12 + 5 * 15));
	
   delete a; delete b;
	
	return result;
}

static bool testlengthVector 	(void) {
	Vector<int>* a;
	
	a = new Vector<int>;
	
	a->setDimension(3); 
	
	a->setValue(0, 3); 
	a->setValue(1, -4); 
	a->setValue(2, 5); 
	
	bool result = (a->length() == sqrt (3*3 + 4*4 + 5*5));
	
   delete a;
	
	return result;
}

static bool testgetsetDimVector 	(void) {
	Vector<int> v;
	
   v.setDimension(5000);
	return v.getDimension() == 5000;
}

static bool testprojection 		(void) {
	Vector<double> *a, *b, *c;
	
	a = new Vector<double>; b = new Vector<double>;
	
	a->setValue(0, 1); a->setValue(1, -3); a->setValue(2, 2);
	b->setValue(0, -4); b->setValue(1, 1); b->setValue(2, 5);
	
	c = a->projection(b);
	
   b->multiply(1.0 / 14.0);
	
   delete a;
	
	bool result = c->equal(b);
	
   delete c; delete b;
	
	return result;
}

//////
// The actual helper functions
//////

static void printResult (int res) {
	if (res == true) {
		printf ("PASS\n");
	} else {
		printf ("FAIL\n");
	}
}
