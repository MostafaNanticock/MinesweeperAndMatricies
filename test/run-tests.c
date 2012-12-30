// This is the test for matricies
#include <stdio.h>
#include <stdlib.h>
#include "testmatrix.h"
#include "testvector.h"

static void printres (int result);

int main (int argc, char **argv) {
	int result;

	printf ("Starting Vector tests:\n\n");
	result = testVector();
	printres (result);
	
	printf ("\n");
	
	printf ("Starting Matrix tests:\n\n");
	result = testmatrix();	
	printres (result);

	return EXIT_SUCCESS;
}

static void printres (int result) {
	printf ("\n");
	if (result) {
		printf ("All tests Passed!!!\n");
	} else {
		printf ("Tests did not Pass\n");
	}
}
