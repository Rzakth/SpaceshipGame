#pragma once
#include "vec.h"

struct camera
{vect3D position; vect3D viewDirection; vect3D upVector; vect3D rightVector; vect3D rotation; };

extern camera cam;

void InitializeCamera(vect3D position, vect3D direction, vect3D upVetor, vect3D rightVector);

void MoveCamera(vect3D direction);

void MoveCameraForward(float speed);

void MoveCameraBackward(float speed);

void MoveCameraRight(float speed);

void MoveCameraLeft(float speed);

void MoveCameraUp(float speed);

void MoveCameraDown(float speed);


void RotateX(float angle);

void RotateY(float angle);

void RotateZ(float angle);