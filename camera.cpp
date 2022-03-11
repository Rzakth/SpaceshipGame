#include "vec.h"
#include <math.h>
#include "camera.h"

#define PI 3.14159265

camera cam;

float resetRotating(float rotation)
{
	if (rotation > 359) {
		return 0;
	}
	else if (rotation < 0) {
		return 359;
	}
	return rotation;
}

void InitializeCamera(vect3D position, vect3D direction, vect3D upVetor, vect3D rightVector)
{
	cam.position = position;
	cam.viewDirection = direction;
	cam.rightVector = rightVector;
	cam.upVector = upVetor;
	cam.rotation.x = cam.rotation.y = cam.rotation.z = 0.0;
};

void MoveCamera(vect3D direction)
{
	cam.position = addVector(cam.position, direction);
};

void MoveCameraForward(float speed)
{
	cam.position = addVector(cam.position, scaleVector(cam.viewDirection, speed));
};

void MoveCameraBackward(float speed)
{
	cam.position = addVector(cam.position, scaleVector(cam.viewDirection, -speed));
};

void MoveCameraRight(float speed)
{
	cam.position = addVector(cam.position, scaleVector(cam.rightVector, speed));
};

void MoveCameraLeft(float speed)
{
	cam.position = addVector(cam.position, scaleVector(cam.rightVector, -speed));
};

void MoveCameraUp(float speed)
{
	cam.position = addVector(cam.position, scaleVector(cam.upVector, speed));
};

void MoveCameraDown(float speed)
{
	cam.position = addVector(cam.position, scaleVector(cam.upVector, -speed));
};

void RotateX(float angle)
{;
	cam.rotation.x = resetRotating(cam.rotation.x += angle);

	cam.viewDirection = normalizeVector(addVector(scaleVector(cam.viewDirection , cos(angle * (PI/180))) , scaleVector(cam.upVector , sin(angle * (PI / 180)))));

	cam.upVector= scaleVector(crossVector(cam.viewDirection, cam.rightVector) , -1);
};

void RotateY(float angle)
{
	cam.rotation.y  = resetRotating(cam.rotation.y += angle);

	cam.viewDirection = normalizeVector(subVector(scaleVector(cam.viewDirection , cos(angle * (PI / 180))) , scaleVector(cam.rightVector , sin(angle * (PI / 180)))));

	cam.rightVector = crossVector(cam.viewDirection, cam.upVector);

};

void RotateZ(float angle)
{
	cam.rotation.z  = resetRotating(cam.rotation.z += angle);

	cam.rightVector = normalizeVector(addVector(scaleVector(cam.rightVector , cos(angle * (PI / 180))) , scaleVector(cam.upVector , sin(angle * (PI / 180)))));

	cam.upVector = scaleVector(crossVector(cam.viewDirection, cam.rightVector) , -1);
};