#ifndef CVECTOR3
#define CVECTOR3

// This is our 3D point class.  This will be used to store the vertices of our model.
// This is our basic 3D point/vector class
class CVector3
{
public:
	float x, y, z;	
	/*	CVector3(const CVector & vec)
	{
		x=vec.x;
		y=vec.y;
		z=vec.z;
	}	*/
	CVector3(float X=0, float Y=0, float Z=0): x(X),y(Y),z(Z){}
 	~CVector3() {}

	friend CVector3 operator+(const CVector3& l, const CVector3& r)
	{
		return CVector3(l.x + r.x, l.y + r.y, l.z + r.z);		// Return the added vectors result.
	}

	CVector3& operator+=(const CVector3& vVector)
	{
		return *this= *this+vVector;
	}

	CVector3 operator-(const CVector3& vVector) const
	{
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);		// Return the subtracted vectors result
	}
	
	// Here we overload the * operator so we can multiply by scalars
	CVector3 operator*(float num)
	{
		return CVector3(x * num, y * num, z * num);			// Return the scaled vector
	}

	// Here we overload the *= operator so we can multiply by scalars
	CVector3& operator*=(float num)
	{
		return *this= *this * num;
	}

	// Here we overload the / operator so we can divide by a scalar
	CVector3 operator/(float num)
	{
		return CVector3(x / num, y / num, z / num);			// Return the scale vector
	}
					
};
#endif

