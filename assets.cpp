#include <iostream>
#include <fstream>
#include <sstream>
#include <gl/freeglut.h>
#include <vector>
#include "vec.h"
#include "assets.h"
#include "../gltools.h"

GLint imageWidth, imageHeight, imageComponents;
GLenum eFormat;


GLbyte* pBytes0;

GLuint getTexture(const char* texturePath)
{
	GLuint texture;
	
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, texture);

	pBytes0 = gltLoadTGA(texturePath, &imageWidth, &imageHeight, &imageComponents, &eFormat);

	glTexImage2D(GL_TEXTURE_2D, 0, imageComponents, imageWidth, imageHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	free(pBytes0);

	return texture;
};

model loadObject(const char* objPath)
{
	FILE* file = fopen(objPath, "r");

	model object;

	while (1)
	{

		if (file == NULL) {
			std::cout << ("Impossible to open the file !\n");
			return object;
		}

		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0)
		{
			vect3D vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			object.objectVertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vect2D textCo;
			fscanf(file, "%f %f\n", &textCo.x, &textCo.y);
			object.objectTextCord.push_back(textCo);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vect3D normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			object.objectNormals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			triFace triangleFace;
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &triangleFace.vIndex[0], &triangleFace.vtIndex[0], &triangleFace.vnIndex[0], &triangleFace.vIndex[1], &triangleFace.vtIndex[1], &triangleFace.vnIndex[1], &triangleFace.vIndex[2], &triangleFace.vtIndex[2], &triangleFace.vnIndex[2]);
			triangleFace.vIndex[0]--; triangleFace.vtIndex[0]--; triangleFace.vnIndex[0]--, triangleFace.vIndex[1]--, triangleFace.vtIndex[1]--, triangleFace.vnIndex[1]--, triangleFace.vIndex[2]--, triangleFace.vtIndex[2]--, triangleFace.vnIndex[2]--;
			object.triFaces.push_back(triangleFace);
		}
	}
	return object;
}

void drawObject(model object, GLuint texture, vect3D color)
{

	glEnable(GL_TEXTURE_2D);
	glColor3f(color.x, color.y, color.z);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLES);
	for each (triFace face in object.triFaces)
	{
		glNormal3f(object.objectNormals[face.vnIndex[0]].x, object.objectNormals[face.vnIndex[0]].y, object.objectNormals[face.vnIndex[0]].z);
		glTexCoord2f(object.objectTextCord[face.vtIndex[0]].x, object.objectTextCord[face.vtIndex[0]].y);
		glVertex3f(object.objectVertices[face.vIndex[0]].x, object.objectVertices[face.vIndex[0]].y, object.objectVertices[face.vIndex[0]].z);

		glNormal3f(object.objectNormals[face.vnIndex[1]].x, object.objectNormals[face.vnIndex[1]].y, object.objectNormals[face.vnIndex[1]].z);
		glTexCoord2f(object.objectTextCord[face.vtIndex[1]].x, object.objectTextCord[face.vtIndex[1]].y);
		glVertex3f(object.objectVertices[face.vIndex[1]].x, object.objectVertices[face.vIndex[1]].y, object.objectVertices[face.vIndex[1]].z);

		glNormal3f(object.objectNormals[face.vnIndex[2]].x, object.objectNormals[face.vnIndex[2]].y, object.objectNormals[face.vnIndex[2]].z);
		glTexCoord2f(object.objectTextCord[face.vtIndex[2]].x, object.objectTextCord[face.vtIndex[2]].y);
		glVertex3f(object.objectVertices[face.vIndex[2]].x, object.objectVertices[face.vIndex[2]].y, object.objectVertices[face.vIndex[2]].z);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}