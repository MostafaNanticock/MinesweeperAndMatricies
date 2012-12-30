// This is an implementation of a Vector ADT
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "vector.h"

struct vector {
	double *values;
	int length;			// the dimension of the vector
};

Vector createVector (void) {
	struct vector* vec = malloc (sizeof (struct vector));

	assert (vec != NULL);
	
	vec->values = malloc (sizeof (double));
	vec->values[0] = 0;
	vec->length = 0;
	
	return vec;
}

void deleteVector (Vector toDelete) {
	if (toDelete->values != NULL) {
		free (toDelete->values);
	}
	
	free (toDelete);
}

Vector copyVector (Vector toCopy) {
	Vector clone = createVector ();
	int i;
	
	clone->length = getDimVector (toCopy);
	clone->values = realloc (clone->values, clone->length * sizeof (double));
	
	if (clone->length > 0) {
		//memcpy (clone->values, toCopy->values, clone->length * sizeof (double));
		for (i = 0; i < clone->length; i++) {
			clone->values[i] = toCopy->values[i];
		}
	}
	
	return clone;
}

int areEqualVector (Vector a, Vector b) {
	int i, result = 0;
	
	if (a->length == b->length) {
		result = 1; // true
		
		for (i = 0; (i < a->length) && result; i++) {
			if (a->values[i] != b->values[i]) result = 0;	// false
		}
	}
	
	return result;
}

int getDimVector (Vector v) {
	return v->length;
}

void setDimVector (Vector v, int dim) {
	if (dim != getDimVector (v)) {
		v->values = realloc (v->values, (dim) * sizeof (double));

		if (dim > v->length) {
			memset (v->values + getDimVector(v) * sizeof (double), 0, (dim - getDimVector(v)) * sizeof (double));
		}
		
		v->length = dim;
	}
}

double getValueVector (Vector v, int index) {
	assert (index >= 0);
	assert (index < getDimVector (v));
	
	return v->values[index];
}

void setValueVector (Vector v, int index, double value) {
	int i;
	assert (index >= 0);

	if (index >= getDimVector(v)) {
		// then the size of the vector needs to be increased
		v->values = realloc (v->values, (index + 1) * sizeof (double));
		//memset (v->values + getDimVector(v) * sizeof (double), 0, (index - getDimVector(v)) * sizeof (double));
		for (i = getDimVector(v); i <= index; i++) {
			v->values[i] = 0;
		} 
		
		v->length = index + 1;
	}
	
	v->values[index] = value;
}

void addVector (Vector result, Vector toAdd) {
	int i, len;
	
	assert (getDimVector (result) == getDimVector (toAdd));
	
	len = getDimVector (result);
	for (i = 0; i < len; i++) {
		result->values[i] += toAdd->values[i];
	}
}

// multiply a vector by a scalar
void multiplyVector (Vector result, double value) {
	int i, len;
	
	len = getDimVector (result);
	for (i = 0; i < len; i++) {
		result->values[i] *= value;
	}
}

double dotProduct (Vector x, Vector y) {
	int i, len;
	double dp;
	
	assert (getDimVector (x) == getDimVector (y));
	
	len = getDimVector (x);
	dp = 0;
	for (i = 0; i < len; i++) {
		dp += x->values[i] * y->values[i];
	}
	
	return dp;
}

double lengthVector (Vector v) {
	int i, len;
	double result;
	
	len = getDimVector (v);
	result = 0;
	for (i = 0; i < len; i++) {
		result += v->values[i] * v->values[i];
	}
	
	result = sqrt (result);
	
	return result;
}

// the projection of a onto b
Vector projection (Vector a, Vector b) {
	assert (a != NULL); assert (b != NULL);
	
	double len = lengthVector (b);
	double scalar;	
	Vector result = copyVector (b);

	scalar = dotProduct (a, b) / (len * len);
	multiplyVector (result, scalar);
	
	return result;
}

void roundVector (Vector v) {
	int i;
	
	for (i = 0; i < getDimVector (v); i++) {
		v->values[i] = floor (v->values[i] + 0.5);
	}
}

void ceilVector (Vector v) {
	int i;
	
	for (i = 0; i < getDimVector (v); i++) {
		v->values[i] = ceil (v->values[i]);
	}
}

void floorVector (Vector v) {
	int i;
	
	for (i = 0; i < getDimVector (v); i++) {
		v->values[i] = floor (v->values[i]);
	}
}
