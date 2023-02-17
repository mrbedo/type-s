#ifndef CFRUSTUM_H
#define CFRUSTUM_H

// This will allow us to create an object to keep track of our frustum
class CFrustum 
{
private:
	enum FrustumSide	{ RIGHT, LEFT, BOTTOM, TOP, BACK, FRONT }; 
	enum PlaneData		{ X, Y, Z, D };

	float m_Frustum[6][4];									// This holds the A B C and D values for each side of our frustum.

public:
	void CalculateFrustum();
	// Call this every time the camera moves to update the frustum

	bool PointInFrustum(float x, float y, float z);
	// This takes a 3D point and returns TRUE if it's inside of the frustum

	bool SphereInFrustum(float x, float y, float z, float radius);
	// This takes a 3D point and a radius and returns TRUE if the sphere is inside of the frustum

	bool CubeInFrustum( float x, float y, float z, float size );
	// This takes the center and half the length of the cube.
};
#endif
