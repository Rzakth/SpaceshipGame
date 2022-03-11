#pragma once

struct vect3D
{ float x; float y; float z; };

struct vect2D
{ float x; float y; };

vect3D addVector(vect3D v1, vect3D v2);

vect3D subVector(vect3D v1, vect3D v2);

vect3D scaleVector(vect3D v, float size);

vect3D normalizeVector(vect3D v);

vect3D crossVector(vect3D u, vect3D v);

float getVectorLength(vect3D v);

float getVectorDistance(vect3D v1, vect3D v2);