#include <math.h>
#include "vec.h"

vect3D addVector(vect3D v1, vect3D v2)
{
	vect3D result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
};

vect3D subVector(vect3D v1, vect3D v2)
{
	vect3D result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
};

vect3D multiplyVector(vect3D v1, vect3D v2)
{
	vect3D result;
	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;
	return result;
};

vect3D scaleVector(vect3D v, float size)
{
	vect3D result;
	result.x = v.x * size;
	result.y = v.y * size;
	result.z = v.z * size;
	return result;
};


vect3D normalizeVector(vect3D v)
{
	vect3D result;
	float l = getVectorLength(v);
	if (l == 0.0f) return vect3D{ 0.0f,0.0f,0.0f };
	result.x = v.x / l;
	result.y = v.y / l;
	result.z = v.z / l;
	return result;
};

vect3D crossVector(vect3D u, vect3D v)
{
	vect3D result;
	result.x = u.y * v.z - u.z * v.y;
	result.y = u.z * v.x - u.x * v.z;
	result.z = u.x * v.y - u.y * v.x;
	return result;
};

float getVectorLength(vect3D v)
{
	return (float)(sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)));
};

float getVectorDistance(vect3D v1, vect3D v2)
{
	return (float)(sqrt((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) * (v2.z - v1.z)));
};

