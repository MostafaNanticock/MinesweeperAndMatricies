// This is an implementation of a Vector ADT
#ifndef __RMVECTORADT
#define __RMVECTORADT

typedef struct vector* Vector;

Vector createVector (void);
void deleteVector (Vector toDelete);
Vector copyVector (Vector toCopy);

int areEqualVector (Vector a, Vector b);

double getValueVector (Vector v, int index);
void setValueVector (Vector v, int index, double value);

void addVector (Vector result, Vector toAdd);
void multiplyVector (Vector result, double value);
double dotProduct (Vector x, Vector y);
double lengthVector (Vector v);

int getDimVector (Vector v);
// if the vector is being made larger then all new spaces are zeroes
// if the vector is geiing smaller then old values are preserved
void setDimVector (Vector v, int dim);

// the projection of a onto b
Vector projection (Vector a, Vector b);

void roundVector (Vector v);
void ceilVector (Vector v);
void floorVector (Vector v);

#endif
