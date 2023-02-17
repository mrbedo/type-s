#ifndef CVECTOR_H
#define CVECTOR_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//  Filename:  CVector.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Modified by Bedros Magardichian
//
//  (c) Bedoworks Visual Studios 2003
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//  Description:
//
//
//  This class provides vector math functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CVector class
//
//	OpenGL Game Programming
//	by Kevin Hawkins and Dave Astle
//
//	Some operators of the CVector class based on
//	operators of the CVector class by Bas Kuenen.
//	Copyright (c) 2000 Bas Kuenen. All Rights Reserved.
//	homepage: baskuenen.cfxweb.net
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "CVector3.h"

#define PI	      (3.1415926535897932384626433832795028841971693993751f)
#define DEG2RAD(a)	(PI/180*(a))
#define RAD2DEG(a)	(180/PI*(a))
#define MIN(x,y)    (((x) < (y)) ? (x) : (y))
#define MAX(x,y)    (((x) > (y)) ? (x) : (y))
#define MID(x,y,z)  MAX((x), MIN((y), (z)))
      
#undef SGN
#define SGN(x)      (((x) >= 0) ? 1 : -1)

typedef float scalar_t;

class CVector
{
public:
     scalar_t x;
     scalar_t y;
     scalar_t z;											// x,y,z coordinates

public:
     CVector(scalar_t a = 0, scalar_t b = 0, scalar_t c = 0) : x(a), y(b), z(c) {}
     CVector(const CVector &vec) : x(vec.x), y(vec.y), z(vec.z) {}
	 CVector(const CVector3 &vec) : x(vec.x), y(vec.y), z(vec.z) {}

	 ~CVector(){}

	scalar_t &operator[](const long idx)					// vector index
	{
		return *((&x)+idx);
	}

    const CVector &operator=(const CVector &vec)			// vector assignment
	{
		x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    const bool operator==(const CVector &vec) const			// vecector equality
    {
		return ((x == vec.x) && (y == vec.y) && (z == vec.z));
    }
 
	const bool operator!=(const CVector &vec) const			// vecector inequality
	{
        return !(*this == vec);
    }
 
	const bool operator<(const CVector &vec) const			// vecector equality
    {
		return ((x < vec.x) && (y < vec.y) && (z < vec.z));
    }

	const bool operator>(const CVector &vec) const			// vecector equality
    {
		return ((x > vec.x) && (y > vec.y) && (z > vec.z));
    }


	const CVector operator+(const CVector &vec) const		// vector add
    {
        return CVector(x + vec.x, y + vec.y, z + vec.z);
    }

    const CVector operator+() const							// vector add (opposite of negation)
    {    
        return CVector(*this);
    }

    const CVector& operator+=(const CVector& vec)			// vector increment
    {    x += vec.x;
         y += vec.y;
         z += vec.z;
         return *this;
    }

    const CVector operator-(const CVector& vec) const		// vector subtraction
    {    
         return CVector(x - vec.x, y - vec.y, z - vec.z);
    }
    
    const CVector operator-() const							// vector negation
    {    
         return CVector(-x, -y, -z);
    }
 
	const CVector &operator-=(const CVector& vec)		    // vector decrement
    {
         x -= vec.x;
         y -= vec.y;
         z -= vec.z;
         return *this;
    }
 
	const CVector &operator*=(const scalar_t &s)			// scalar self-multiply
    {
         x *= s;
         y *= s;
         z *= s;
         return *this;
    }
    
	const CVector &operator/=(const scalar_t &s)			// scalar self-divecide
    {
         const float recip = 1/s;							// for speed, one divecision
         x *= recip;
         y *= recip;
         z *= recip;
         return *this;
    }

    const CVector operator*(const scalar_t &s) const		// post multiply by scalar
    {
         return CVector(x*s, y*s, z*s);
    }
  
	friend inline const CVector operator*(const scalar_t &s, const CVector &vec)     // pre multiply by scalar
    {
         return vec*s;
    }
	const CVector operator*(const CVector& vec) const       // multiply by vector
	{
		return CVector(x*vec.x, y*vec.y, z*vec.z);
	}
	const CVector operator/(scalar_t s) const				// divide by scalar
    {
         s = 1/s;
         return CVector(s*x, s*y, s*z);
    }
    const CVector CrossProduct(const CVector &vec) const	// cross product
    {
         return CVector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
    }
 
	const CVector operator^(const CVector &vec) const       // cross product
    {
         return CVector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
    }
  
	const scalar_t DotProduct(const CVector &vec) const     // dot product
    {
         return x*vec.x + y*vec.x + z*vec.z;
    }

    const scalar_t operator%(const CVector &vec) const		// dot product
    {
         return x*vec.x + y*vec.x + z*vec.z;
    }
 
	const scalar_t Length() const							// length of vector
    {
         return (scalar_t)sqrt((double)(x*x + y*y + z*z));
    }
    
	const CVector UnitVector() const						// return the unit vector
    {
         return (*this) / Length();
    }
 
	void Normalize()										// normalize this vector
    {
         (*this) /= Length();
    }
   
	const scalar_t operator!() const
    {
         return sqrtf(x*x + y*y + z*z);
    }

	const CVector operator | (const scalar_t length) const  // return vector with specified length
    {
         return *this * (length / !(*this));
    }
 
	const CVector& operator |= (const float length)			// set length of vector equal to length
    {
         return *this = *this | length;
    }
 
	const float inline Angle(const CVector& normal) const	// return angle between two vectors
    {
         return acosf(*this % normal);
    }

    const CVector inline Reflection(const CVector& normal) const // reflect this vector off surface with normal vector
    {    
         const CVector vec(*this | 1);						// normalize this vector
         return (vec - normal * 2.0 * (vec % normal)) * !*this;
    }

	const CVector inline Rotate(const float angle, const CVector& normal) const	// rotate angle degrees about a normal
	{	
		const float cosine = cosf(angle);
		const float sine = sinf(angle);
		return CVector(*this * cosine + ((normal * *this) * (1.0f - cosine)) *
			          normal + (*this ^ normal) * sine);
	}
	
	// Performs the specified operation on the specified vertex of the vector
	CVector vertexOp(char vert, char opp, float num)
	{
		switch(vert)
		{
		case 'x':
			switch(opp)
			{
			case '+': return CVector(x + num, y, z); break;			// Return the scaled vector
			case '-': return CVector(x - num, y, z); break;			// Return the scaled vector
			case '*': return CVector(x * num, y, z); break;			// Return the scaled vector
			case '/': return CVector(x / num, y, z); break;			// Return the scaled vector
			default: return CVector(x,y,z); break;
			}break;
		case 'y':
			switch(opp)
			{
			case '+': return CVector(x, y + num, z); break;			// Return the scaled vector
			case '-': return CVector(x, y - num, z); break;			// Return the scaled vector
			case '*': return CVector(x, y * num, z); break;			// Return the scaled vector
			case '/': return CVector(x, y / num, z); break;			// Return the scaled vector
			default: return CVector(x,y,z); break;
			}break;
		case 'z':
			switch(opp)
			{
			case '+': return CVector(x, y, z + num); break;			// Return the scaled vector
			case '-': return CVector(x, y, z - num); break;			// Return the scaled vector
			case '*': return CVector(x, y, z * num); break;			// Return the scaled vector
			case '/': return CVector(x, y, z / num); break;			// Return the scaled vector
			default: return CVector(x,y,z); break;
			}break;
			default: return CVector(x,y,z); break;
		}
	}

	//Support Conversion for CVector3
	const CVector &operator=(const CVector3 &vec)			// vector assignment
	{
		x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    const bool operator==(const CVector3 &vec) const			// vecector equality
    {
		return ((x == vec.x) && (y == vec.y) && (z == vec.z));
    }
 
	const bool operator!=(const CVector3 &vec) const			// vecector inequality
	{
        return !(*this == vec);
    }
 
	const bool operator<(const CVector3 &vec) const			// vecector equality
    {
		return ((x < vec.x) && (y < vec.y) && (z < vec.z));
    }

	const bool operator>(const CVector3 &vec) const			// vecector equality
    {
		return ((x > vec.x) && (y > vec.y) && (z > vec.z));
    }


	const CVector operator+(const CVector3 &vec) const		// vector add
    {
        return CVector(x + vec.x, y + vec.y, z + vec.z);
    }

    const CVector& operator+=(const CVector3 &vec)			// vector increment
    {    x += vec.x;
         y += vec.y;
         z += vec.z;
         return *this;
    }

    const CVector operator-(const CVector3 &vec) const		// vector subtraction
    {    
         return CVector(x - vec.x, y - vec.y, z - vec.z);
    }
     
	const CVector &operator-=(const CVector3 &vec)		    // vector decrement
    {
         x -= vec.x;
         y -= vec.y;
         z -= vec.z;
         return *this;
    }

	//Use by CMatrix
	float CVector::abs() const
	{
		return sqrt(abs2());
	}

	float CVector::abs2() const
	{
		return x*x+y*y+z*z;
	}

	const CVector normal() const
	{
		CVector ret;
		float a = abs();
		if(a > 0.0)
		{
			ret.x = x / a;
			ret.y = y / a;
			ret.z = z / a;
		}
		else //very small vector->badly defined direction->setting to +x
		{
			ret.x = 1.0;
		}
		return ret;
	}


};
#endif