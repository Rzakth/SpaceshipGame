#pragma once
#include <gl/freeglut.h>

using namespace std;

struct triFace
{ int vIndex[3]; int vtIndex[3]; int vnIndex[3]; };

struct model
{ vector<triFace> triFaces; vector<vect3D> objectVertices; vector<vect3D> objectNormals; vector<vect2D> objectTextCord; };

struct asset
{ char* name; vect3D position; vect3D rotation; model model; GLuint texture; vect3D color; };

model loadObject(const char* objPath);

GLuint getTexture(const char* texturePath);

void drawObject(model object, GLuint texture, vect3D color);