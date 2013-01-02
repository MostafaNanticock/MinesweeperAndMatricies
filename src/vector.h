// This is an implementation of a Vector ADT
#ifndef __RMVECTORADT
#define __RMVECTORADT

#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>

template<class T>
class Vector
{
   public:
      /**
       * Construct a vector of size 1.
       */
      Vector()
         : values(1)
      {
         values[0] = 0;
      }

      Vector(const Vector& vectorSource)
      {
         for(
               typename std::vector<T>::const_iterator it = vectorSource.values.begin();
               it != vectorSource.values.end();
               ++it
            )
         {
            values.push_back(*it);
         }
      }

      void copy(const Vector<T>* vectorSource)
      {
         values.clear();
         for(int i = 0; i < vectorSource->getDimension(); ++i)
         {
            values.push_back(vectorSource->getValue(i));
         }
      }

      int getDimension() const
      {
         return values.size();
      }

      void setDimension(int dimension)
      {
         values.resize(dimension);
      }

      /**
       * Compare, elementwise, with another vector and return true if every element is 
       * identical.
       */
      bool equal(Vector* vector)
      {
         if(values.size() != vector->values.size()) return false;

         int vectorLength = values.size();
         for(int i = 0; i < vectorLength; ++i)
         {
            if(values[i] != vector->values[i]) return false;
         }

         return true;
      }

      T getValue(int index) const
      {
         return values[index];
      }

      void setValue(int index, T value)
      {
         // Automatically resize the vector.
         if(index >= values.size())
         {
            setDimension(index + 1);
         }

         values[index] = value;
      }
      
      /**
       * Add the given vector to the current vector. The current vector will be modified
       * to contain the final result.
       * \param toAdd the vector to add to this vector.
       */
      void add(Vector* toAdd)
      {
         const int vectorLength = values.size();
         assert(vectorLength == toAdd->values.size());

         for(int i = 0; i < vectorLength; ++i)
         {
            values[i] += toAdd->values[i];
         }
      }

      void multiply(T value)
      {
         const int vectorLength = values.size();
         for(int i = 0; i < vectorLength; ++i)
         {
            values[i] *= value;
         }
      }

      double length()
      {
         int vectorLength = values.size();
         double result = 0;

         for (int i = 0; i < vectorLength; ++i) {
            result += values[i] * values[i];
         }
         
         return sqrt(result);
      }

      double dot(Vector* b)
      {
         const int vectorLength = values.size();
         assert(vectorLength == b->values.size());

	      double dp = 0;
         for (int i = 0; i < vectorLength; ++i) {
            dp += values[i] * b->values[i];
         }
	
         return dp;
      }

      Vector* projection(Vector* b)
      {
         Vector* result = new Vector();
         *result = *b;

         double length = b->length();
	      double scalar = dot(b) / (length * length);
         result->multiply(scalar);
	
         return result;
      }

      void round()
      {
         for (int i = 0; i < values.size(); ++i) {
            values[i] = ::floor (values[i] + 0.5);
         }
      }

      void ceil()
      {
         for (int i = 0; i < values.size(); ++i) {
            values[i] = ::ceil(values[i]);
         }
      }

      void floor()
      {
         for (int i = 0; i < values.size(); ++i) {
            values[i] = ::floor(values[i]);
         }
      }

   private:
      std::vector<T> values;
};

/*
double dotProduct (Vector x, Vector y);

// the projection of a onto b
Vector projection (Vector a, Vector b);
*/

#endif
