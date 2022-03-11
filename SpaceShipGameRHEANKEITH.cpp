//SPACE SHIP GAME CW2
//CODE BY RHEAN KEITH
#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include "shared/freeglut.h"
#include <vector>
#include <iostream>
#include <thread>

#ifdef WIN32
#include "gltools.h"
#include <windows.h>
#endif

#include "3DExample1\\camera.h"
#include "3DExample1\\vec.h"
#include "3DExample1\\assets.h"
#include <sstream>


#define OBJECTS = "3DExample1\\objects\\";
#define TEXTURES = "3DExample1\\textures\\";

using namespace std;

int screenWidth = 800;
int screenHeight = 600;

bool isLoading = true;
bool done = false;

asset orbit, world, earth, diamond, cylinder, loop;

GLfloat  EarthLight[] = { 253.0f, 184.0f, 19.0f, 1.0f };
GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat  greenLight[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat	 lightPos[] = { 4000.0f, 4000.0f, -4000.0f, 100.0f };
GLfloat	 greenLightPos[] = { 0.0f, 1000.0f, -4000.0f, 100.0f };

GLfloat materialSpecular[] = { 4.0, 4.0, 4.0, 4.0 };
GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

vect3D viewPoint;
vect3D color = { 0.8, 0.8, 0.8 };
vect3D dcolor = { 0.8, 0.0, 0.0 };
GLuint hudTexture;
bool hudWarn = false;

int score = 0;

string loadingStage = "Loading";
float shipSpeed = 10.0f;
FLOAT initSpeed = 70;
float elapsedTime = 0.0f;

struct hoop
{vect3D position; vect3D color; float rotationY; };

struct keypress
{bool W = false;bool S = false;bool A = false;bool D = false;bool Z = false; bool X = false; bool Q = false; bool E = false; bool UP = false; bool DOWN = false; bool LEFT = false; bool RIGHT = false; bool SHIFT = false; };

keypress keysPressed;

vector<hoop> hoops;
bool isHud = false;
vect3D startPosition;

int getRandomNumber(int max, int min)
{
	return (rand() % (max + 1 - min)) + min;
}

void drawText(string text, void* font, vect3D color, int x, int y)
{
	glColor3f(color.x, color.y, color.z);
	glPushMatrix();
	glRasterPos2i(x, y);
	for (string::iterator i = text.begin(); i != text.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
	glPopMatrix();
}

void resetWorld()
{

	score = 0;
	cam.position = startPosition;
	hoop hp;
	hoops.clear();
	for (size_t i = 8000; i > 5000; i -= 500)
	{
		hp.position = vect3D{ (float)getRandomNumber(500, -500), (float)getRandomNumber(500, -500), (float)getRandomNumber(-i, -i + 500) };
		hp.color = vect3D{ 1.0, 1.0, 0.0 };
		hp.rotationY = getRandomNumber(359, 0);
		hoops.push_back(hp);
	}

	hoops[hoops.size() - 1].color = vect3D{ 0.0, 1.0, 0.0 };
}

void loadAssets()
{
	loadingStage = "Waiting for World to Load...";
	world.model = earth.model = orbit.model = loadObject(".\\objects\\spear.obj");
	orbit.position.x = orbit.position.y = 0.0; orbit.position.z = -20.0;
	orbit.rotation.y = orbit.rotation.z = 0.0; orbit.rotation.x = 20.0;
	orbit.color = {0.1, 0.1, 0.1};
	loop.color = {1.0, 0.0, 0.0};
	resetWorld();
	world.position.x = world.position.y = world.position.z = 0.0;

	earth.position.x = earth.position.y = earth.position.z = 0.0;
	loadingStage = "Loading Hoops...";
	loop.model = loadObject(".\\objects\\hoop.obj");
	loadingStage = "Loading Diamonds...";
	diamond.model = loadObject(".\\objects\\crystal.obj");
	cam.position.z = 5000;
	loadingStage = "Loading Done...";
	done = true;

}

void processCamera()
{
	vect3D viewPoint = addVector(cam.position, cam.viewDirection);
	gluLookAt(cam.position.x, cam.position.y, cam.position.z, viewPoint.x, viewPoint.y, viewPoint.z, cam.upVector.x, cam.upVector.y, cam.upVector.z);
};

void drawScoreText()
{
	drawText(loadingStage, GLUT_BITMAP_9_BY_15, vect3D{ 1.0, 1.0, 1.0 }, 100, screenHeight - 100);
	drawText("Crystals Collected : " + to_string(score), GLUT_BITMAP_TIMES_ROMAN_24, vect3D{ 0.3, 0.6, 71.516 }, 50, 50);
}

void Hud()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glColor3f(0.8,0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, hudTexture);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(0.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(screenWidth, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(screenWidth, screenHeight);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(0.0, screenHeight);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);

};

//display method
void RenderScene(void)
{


	// Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	processCamera();

	if (isLoading && !done) {
		glPushMatrix();
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glScalef(100, 50000, 100);
		drawObject(cylinder.model, cylinder.texture, color);
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glScalef(10000, 10000, 10000);
		drawObject(world.model, world.texture, color);
		glPopMatrix();


		glPushMatrix();
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glTranslatef(2000.0, 1000.0, -4000.0);
		glScalef(100, 100, 100);
		drawObject(earth.model, earth.texture, color);
		glPopMatrix();
		glPushMatrix();
		glPushMatrix();


		glTranslatef(-2000.0, 0.0, -9000.0);
		glRotatef(orbit.rotation.x, 1.0, 0.0, 0.0);
		glRotatef(elapsedTime*4, 0.0, 1.0, 0.0);
		glScalef(200, 200, 200);
		drawObject(orbit.model, orbit.texture, orbit.color);
		glPopMatrix();

		for each (hoop var in hoops)
		{

			glPushMatrix();
			glTranslatef(var.position.x, var.position.y, var.position.z);
			glRotatef(var.rotationY + (elapsedTime  * 10), 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glScalef(100, 100, 100);
			drawObject(loop.model, loop.texture, loop.color);
			glPopMatrix();

			glPushMatrix();
			glLightfv(GL_LIGHT1, GL_POSITION, greenLightPos);
			glTranslatef(var.position.x, var.position.y, var.position.z);
			glRotatef(orbit.rotation.y - (elapsedTime  * 20), 0.0, 1.0, 0.0);
			glScalef(50, 50, 50);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
			drawObject(diamond.model, diamond.texture, var.color);
			glDisable(GL_BLEND);
			glPopMatrix();
		}
	}


	//2d Projection - Start
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, screenWidth, screenHeight, 0.0, 0.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);

	drawScoreText();
	if (isHud) {
		Hud();
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
};

void CrystalCollision(vect3D pos) {

	int activeHoop = hoops.size() - (score + 1);
	if (hoops.size() > activeHoop) {
		float distance = getVectorDistance(hoops[activeHoop].position, pos);


		if (distance < 100)
		{
			hoops[activeHoop].color = vect3D{ 1.0, 1.0, 0.0 };
			score++;
			activeHoop = hoops.size() - (score + 1);
			if (hoops.size() > activeHoop){
				hoops[activeHoop].color = vect3D{ 0.0, 1.0, 0.0 };
			}
			else 
			{
				resetWorld();
			}
		}
	}
}

bool isEndofSpace(vect3D pos) {

	float distance = getVectorDistance(world.position, pos);

	if (distance > 8500)
	{
		return true;
	}
	return false;
}


void setupGame()
{
	vect3D position; position.x = position.y = 0.0; position.z = 40000.0;
	vect3D viewDirection; viewDirection.x = viewDirection.y = 0.0; viewDirection.z = -1.0;
	vect3D upVector; upVector.x = upVector.z = 0.0; upVector.y = 1.0;
	vect3D rightVector;  rightVector.z = rightVector.y = 0.0; rightVector.x = 1.0;
	InitializeCamera(position, viewDirection, upVector, rightVector);

	cylinder.model = loadObject(".\\objects\\cylinder.obj");
	cylinder.texture = world.texture = getTexture(".\\textures\\starField.tga");
	orbit.texture = getTexture(".\\textures\\jupiterC.tga");
	earth.texture = getTexture(".\\textures\\Earth.tga");
	diamond.texture = getTexture(".\\textures\\diamond.tga");
	hudTexture = getTexture(".\\textures\\hud.tga");
	loop.texture = getTexture(".\\textures\\Stars.tga");


	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);

    glCullFace(GL_FRONT_AND_BACK);

 	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, EarthLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, redLight);
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);


	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, greenLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, greenLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT1);

	// Enable colour tracking
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialSpecular);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);

	// Black blue background
	glClearColor(0.0f, 0.0f, 0.03f, 1.0f);

};

void reshape(int w, int h)
{
	GLfloat fAspect;

	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
	screenWidth = w; screenHeight = h;
	//Getting the Aspect Ratio
	fAspect = (GLfloat)w / (GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// field of view of 45 degrees, near and far planes 1.0 and 1000
	//that znear and zfar should typically have a ratio of 1000:1 to make sorting out z depth easier for the GPU
	gluPerspective(55.0f, fAspect, 1.0, 100000.0); //may need to make larger depending on project
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.00, 0.00, 0.0, 0.00, 0.00, -1.00, 0.00, 1.00, 0.00);
};


void keyInputs()
{
	vect3D prePos = cam.position;
	if (keysPressed.W)
	{
		MoveCameraForward(shipSpeed);
	};
	if (keysPressed.S)
	{
		MoveCameraBackward(shipSpeed);
	};
	if (keysPressed.A)
	{
		MoveCameraLeft(shipSpeed);
	};
	if (keysPressed.D)
	{
		MoveCameraRight(shipSpeed);
	};
	if (keysPressed.Z)
	{
		RotateY(1.0);
	};
	if (keysPressed.X)
	{
		RotateY(-1.0);
	};
	if (keysPressed.UP)
	{
		MoveCameraUp(shipSpeed);
	};
	if (keysPressed.DOWN)
	{
		MoveCameraDown(shipSpeed);
	};
	if (keysPressed.LEFT)
	{
		RotateZ(1.0);
	};
	if (keysPressed.RIGHT)
	{
		RotateZ(-1.0);
	};
	if (keysPressed.Q) {
		RotateX(1.0);
	}
	if (keysPressed.E) {
		RotateX(-1.0);
	}

	if (isEndofSpace(cam.position)) {
		cam.position = prePos;
	}
	CrystalCollision(cam.position);
}



void keyUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w': case 'W':
		keysPressed.W = false;
		break;
	case 's': case 'S':
		keysPressed.S = false;
		break;
	case 'a': case 'A':
		keysPressed.A = false;
		break;
	case 'd': case 'D':
		keysPressed.D = false;
		break;
	case 'z': case 'Z':
		keysPressed.Z = false;
		break;
	case 'x': case 'X':
		keysPressed.X = false;
		break;
	case 'q': case 'Q':
		keysPressed.Q = false;
		break;
	case 'e': case 'E':
		keysPressed.E = false;
		break;
	case 'h': case 'H':
		isHud = !isHud;
		break;
	case 'r': case 'R':
		resetWorld();
		break;
	}
}


void keyboardInput(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w': case 'W':
		keysPressed.W = true;
		break;
	case 's': case 'S':
		keysPressed.S = true;
		break;
	case 'a': case 'A':
		keysPressed.A = true;
		break;
	case 'd': case 'D':
		keysPressed.D = true;
		break;
	case 'z': case 'Z':
		keysPressed.Z = true;
		break;
	case 'x': case 'X':
		keysPressed.X = true;
		break;
	case 'q': case 'Q':
		keysPressed.Q = true;
		break;
	case 'e': case 'E':
		keysPressed.E = true;
		break;
	}
	
};


void Timer(int value)
{
	if (isLoading)
	{
		cam.position.z -= initSpeed;

		if (cam.position.z < -35000)
		{
			cam.position.z = 50000;
		}

		if (done) {
			initSpeed -= 0.3;
			if (initSpeed < 1)
			{
				startPosition = cam.position;
				isLoading = false;
				loadingStage = "";
			}
		}
	}
	elapsedTime += 0.25;
	keyInputs();
	glutSwapBuffers();
	glutPostRedisplay();
	glutTimerFunc(25, Timer, 1);
};


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("3D Space Game by RHEAN KEITH");
	glutKeyboardFunc(keyboardInput);
	glutKeyboardUpFunc(keyUp);
	glutReshapeFunc(reshape);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(25, Timer, 1);
	setupGame();
	thread t1(loadAssets);
	glutMainLoop();
	return 0;
}





